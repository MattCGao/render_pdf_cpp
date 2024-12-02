cmake_minimum_required(VERSION 3.10)
project(RenderHTMLToPDF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)


# PDFium include and library paths (fallback if find_package fails)
include_directories("/home/matt/pdfium/include")
link_directories("/home/matt/pdfium/lib")

# PDFium: Uncomment if you have a FindPDFium.cmake module
# find_package(PDFium REQUIRED)

# Qt5 dependencies
set(CMAKE_PREFIX_PATH "/opt/qt5")
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5WebEngineWidgets REQUIRED)

# Automatic Qt Processing
set(CMAKE_AUTOMOC ON)   # Auto-generate moc files for Q_OBJECT
set(CMAKE_AUTORCC ON)   # Auto-generate resource files (.qrc)
set(CMAKE_AUTOUIC ON)   # Auto-generate UI files (.ui)

set_source_files_properties(render_pdf.cpp PROPERTIES SKIP_AUTOMOC ON)

# Define the executable
add_executable(render_pdf main.cpp render_pdf.cpp render_pdf.h)

# Link Qt5 and PDFium libraries
target_link_libraries(render_pdf
    Qt5::Core
    Qt5::Widgets
    Qt5::WebEngineWidgets
    pdfium
)
