#include "render_pdf.h"
#include <QTimer>

// Define the custom structure to hold both the FPDF_FILEWRITE and ofstream
struct FileWriteData {
    FPDF_FILEWRITE fileWrite;
    std::ofstream outputStream;
};

// The WriteBlock callback function, writing data to an output stream
int WriteBlockImpl(FPDF_FILEWRITE* pThis, const void* pData, unsigned long size) {
    // Cast pThis to user data type (FileWriteData)
    FileWriteData* fileWriteData = reinterpret_cast<FileWriteData*>(pThis);

    // Write the data to the output stream
    fileWriteData->outputStream.write(static_cast<const char*>(pData), size);

    // Return 1 if writing succeeded, 0 if failed
    return fileWriteData->outputStream.good() ? 1 : 0;
}

// Constructor
RenderPDF::RenderPDF(const QString& htmlContent, const QString& pdfPath, QWidget* parent)
    : QWidget(parent), m_pdfPath(pdfPath) {
    m_view = new QWebEngineView(this);
    m_page = m_view->page();

    connect(m_page, &QWebEnginePage::loadFinished, this, &RenderPDF::onLoadFinished);

    m_page->setHtml(htmlContent);
}

// Slot: Triggered when HTML content is fully loaded
void RenderPDF::onLoadFinished(bool ok) {
    if (!ok) {
        std::cerr << "Failed to load HTML.\n";
        emit finished();
        return;
    }

    std::cout  << "HTML content loaded successfully.\n";

    QTimer::singleShot(1000, this, [this]() {
        m_view->repaint(); // Force a repaint (trigger the paint event)

        // Capture the rendered image asynchronously
        QTimer::singleShot(10000, this, [this]() {
            // Get the content size from QWebEngineView
            QSizeF contentSizeF = m_view->page()->contentsSize();
            QSize contentSize = contentSizeF.toSize();

            std::cout << "Content size: " << contentSize.width() << "x" << contentSize.height() << std::endl;

            if (contentSize.isEmpty()) {
                std::cerr << "Content size is empty, rendering might fail.\n";
                emit finished();
                return;
            }    

            QImage image(contentSize, QImage::Format_ARGB32);
            image.fill(Qt::red);  // Fill with white background

            QPainter painter(&image);

            // Render the web content into the image
            m_view->resize(contentSize); // Ensure the view matches the content size
            m_view->render(&painter);

            painter.end();

            image.save("image_debug_render.png");

            generatePDF(image);

            emit finished();
        });
    });

}

void RenderPDF::generatePDF(const QImage& image) {
    // Initialize PDFium library
    FPDF_InitLibrary();

    // Create a new PDF document
    FPDF_DOCUMENT pdfDoc = FPDF_CreateNewDocument();
    if (!pdfDoc) {
        std::cerr << "Failed to create PDF document.\n";
        return;
    }

    // Create a new PDF page (A4 size, 612x792 points)
    FPDF_PAGE pdfPage = FPDFPage_New(pdfDoc, 0, 612, 792); // image.width(), image.height()
    if (!pdfPage) {
        std::cerr << "Failed to create PDF page.\n";
        FPDF_CloseDocument(pdfDoc);
        FPDF_DestroyLibrary();
        return;
    }

    // Create a PDFium bitmap from the QImage data
    FPDF_BITMAP pdfBitmap = FPDFBitmap_Create(image.width(), image.height(), FPDFBitmap_BGR);
    if (!pdfBitmap) {
        std::cerr << "Failed to create PDF bitmap.\n";
        FPDF_ClosePage(pdfPage);
        FPDF_CloseDocument(pdfDoc);
        FPDF_DestroyLibrary();
        return;
    }

    // Copy the QImage data into PDFium's bitmap buffer
    void* buffer = FPDFBitmap_GetBuffer(pdfBitmap);
    if (!buffer) {
        std::cerr << "Failed to retrieve bitmap buffer.\n";
        FPDFBitmap_Destroy(pdfBitmap);
        FPDF_ClosePage(pdfPage);
        FPDF_CloseDocument(pdfDoc);
        FPDF_DestroyLibrary();
        return;
    }    

    QImage convertedImage = image.convertToFormat(QImage::Format_RGB888);
    if(convertedImage.isNull()) {
        std::cerr << "Image conversion failed. \n";
        return;
    }
    memcpy(buffer, convertedImage.bits(), convertedImage.sizeInBytes());

    // Debug: Save the image to verify rendering
    convertedImage.save("convertedImage_debug_render.png");


    // Create an image object for the PDF page
    FPDF_PAGEOBJECT imageObj = FPDFPageObj_NewImageObj(pdfDoc);
    if (!imageObj) {
        std::cerr << "Failed to create image object.\n";
        FPDFBitmap_Destroy(pdfBitmap);
        FPDF_ClosePage(pdfPage);
        FPDF_CloseDocument(pdfDoc);
        FPDF_DestroyLibrary();
        return;
    }


    // Attach the bitmap to the image object
    if (!FPDFImageObj_SetBitmap(&pdfPage, 0, imageObj, pdfBitmap)) {
        std::cerr << "Failed to set bitmap for image object.\n";
        FPDFPageObj_Destroy(imageObj);
        FPDFBitmap_Destroy(pdfBitmap);
        FPDF_ClosePage(pdfPage);
        FPDF_CloseDocument(pdfDoc);
        FPDF_DestroyLibrary();
        return;
    }    

    // Create an FS_MATRIX (3x2 matrix) to scale and position the image
    FS_MATRIX matrix;
    matrix.a = 612.0f / image.width();  // Scale width to page
    matrix.d = 792.0f / image.height(); // Scale height to page
    matrix.e = 0.0f;  // No skew in X
    matrix.f = 0.0f;  // No skew in Y
    matrix.b = 0.0f;  // No skew in X direction
    matrix.c = 0.0f;  // No skew in Y direction

    // Set the matrix for the image object
    if (!FPDFPageObj_SetMatrix(imageObj, &matrix)) {
        std::cerr << "Failed to set matrix for image object.\n";
        FPDFPageObj_Destroy(imageObj);
        FPDFBitmap_Destroy(pdfBitmap);
        FPDF_ClosePage(pdfPage);
        FPDF_CloseDocument(pdfDoc);
        FPDF_DestroyLibrary();
        return;
    }
    
    // Insert the image object into the page
    FPDFPage_InsertObject(pdfPage, imageObj);

    // Save the PDF
    savePDF(pdfDoc);

    std::cout << "Saved PDF files, cleanup... " << std::endl;

    // Cleanup
    FPDFBitmap_Destroy(pdfBitmap);
    if (pdfPage != nullptr) {
        FPDF_ClosePage(pdfPage);
    } else {
        std::cerr << "Error: Attempting to close a null page object." << std::endl;
    }
    if (pdfDoc != nullptr) {
        FPDF_CloseDocument(pdfDoc);
    } else {
        std::cerr << "Error: Attempting to close a null document." << std::endl;
    }

    FPDF_DestroyLibrary();

    std::cout << "PDF generation completed successfully.\n";
}

// Save PDF to file
void RenderPDF::savePDF(FPDF_DOCUMENT pdfDoc) {
    // Create and open the output file
    FileWriteData fileWriteData;
    fileWriteData.outputStream.open(m_pdfPath.toStdString(), std::ios::binary);
    if (!fileWriteData.outputStream.is_open()) {
        std::cerr << "Failed to open output file.\n";
        return;
    }

    // Setup the FPDF_FILEWRITE structure
    fileWriteData.fileWrite.version = 1;
    fileWriteData.fileWrite.WriteBlock = WriteBlockImpl;  // Set the callback function

    // Save the PDF document using the custom FileWriteData structure
    // Pass 0 for flags, as we don't need any special flags here
    if (!FPDF_SaveAsCopy(pdfDoc, &fileWriteData.fileWrite, 0)) {
        std::cerr << "Error saving the PDF document.\n";
    }
}

RenderPDF::~RenderPDF() {
    if (m_page) {
        delete m_page;
        m_page = nullptr;
    }

    if (m_view) {
        delete m_view;
        m_view = nullptr;
    }
}