#ifndef RENDER_PDF_H
#define RENDER_PDF_H

#include <QObject>
#include <QString>
#include <QImage>
#include <QPainter>
#include <QWebEngineView>
#include <QWebEnginePage>
#include <fpdfview.h>
#include <fpdf_save.h>
#include <fpdf_edit.h>
#include <iostream>
#include <fstream>
#include <cstring>

class RenderPDF : public QWidget {
    Q_OBJECT

public:
    explicit RenderPDF(const QString& htmlContent, const QString& pdfPath, QWidget* parent = nullptr);
    ~RenderPDF();

signals:
    void finished();

private slots:
    void onLoadFinished(bool ok);

private:
    void generatePDF(const QImage& image);
    void savePDF(FPDF_DOCUMENT pdfDoc);

    QWebEngineView* m_view;  // Web engine view to render the content
    QWebEnginePage* m_page;  // Web engine page to load HTML content
    QString m_pdfPath;       // Path to save the generated PDF
};

#endif // RENDER_PDF_H
