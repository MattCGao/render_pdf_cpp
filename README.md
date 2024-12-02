# HTML to PDF Renderer using Qt and PDFium

This project demonstrates a simple implementation of rendering HTML content into a PDF file using **Qt** for rendering HTML to an image and **PDFium** for creating PDF documents.

## Features

- Converts HTML content to a PDF file.
- Utilizes Qt's `QWebEngineView` to render HTML.
- Leverages PDFium for creating and manipulating PDF documents.
- Clean and modular design for easy integration into larger projects.

## Requirements

- **Qt 5** or higher (with WebEngine support).
- **PDFium** library (precompiled binaries or source build).
- A Linux environment (tested under WSL2 with Ubuntu 22.04).

## Build Instructions

1. Clone the repository:
   ```bash
   git clone https://github.com/MattCGao/render_pdf_cpp
   cd render_pdf_cpp
   ```

2. Ensure PDFium binaries are available:

   - Download precompiled binaries from [bblanchon/pdfium-binaries](https://github.com/bblanchon/pdfium-binaries).
   - Set the appropriate paths in your `CMakeLists.txt` for `pdfium`.

3. Build the project using CMake:

   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

4. Run the executable:
   ```bash
   ./render_pdf
   ```

## Example Usage

By default, the program renders a sample HTML string into `output.pdf` in the working directory. Modify the `main()` function in `render_pdf.cpp` to customize the input HTML or output file path.

## Acknowledgments

- **PDFium**: [Official Repository](https://pdfium.googlesource.com/pdfium/).
- **Qt Framework**: [Official Site](https://www.qt.io/).

## License

This project is open-source and available under the [MIT License](LICENSE).

```

```
