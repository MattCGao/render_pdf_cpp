#include <QApplication>
#include <QFile>
#include <iostream>
#include "render_pdf.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // Example HTML content (or load from file if specified)
    QString htmlContent = R"(
        <html>
        <body>
            <div style="width: 200px; height: 100px">
                <h1>Hello, PDFium!</h1>
                <p>This is an HTML-to-PDF example.</p>
            </div>
        </body>
        </html>
    )";

    QString pdfPath = "output.pdf";

    // Check for command-line arguments
    if (argc >= 3) {
        QFile htmlFile(argv[1]);
        if (!htmlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            std::cerr << "Failed to open HTML file: " << argv[1] << "\n";
            return 1;
        }

        htmlContent = htmlFile.readAll();
        htmlFile.close();
        pdfPath = argv[2];
    }

    // Create the RenderPDF object
    RenderPDF renderPDF(htmlContent, pdfPath);

    // Connect the finished signal to quit the application
    QObject::connect(&renderPDF, &RenderPDF::finished, [&app]() {
        std::cout << "PDF generation completed.\n";
        QCoreApplication::quit();
    });

    // Show the RenderPDF widget (optional for debugging)
    renderPDF.setVisible(false);  // Hide the widget if not needed

    int result = app.exec();

    return result;
}
