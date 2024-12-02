To install Qt5 from scratch on your system (e.g., Ubuntu, WSL, macOS, or Windows), follow these detailed steps:

---

### **On Ubuntu or WSL**

1. **Update System Packages**:

   ```bash
   sudo apt update
   sudo apt upgrade
   ```

2. **Install Required Dependencies**:

   ```bash
   sudo apt install build-essential libgl1-mesa-dev libxkbcommon-x11-0
   ```

3. **Install Qt5 with Development Tools**:

   ```bash
   sudo apt install qt5-default qttools5-dev-tools qtwebengine5-dev
   ```

4. **Verify Installation**:
   Check the installed version of Qt5:

   ```bash
   qmake --version
   ```

5. **Set Up the Environment (Optional)**:
   If `qmake` isn't found, add Qt5 to your PATH:

   ```bash
   export PATH=/usr/lib/qt5/bin:$PATH
   ```

6. **WSLg**:
   If you are using WSL 2, ensure you have Windows Subsystem for Linux GUI (WSLg) installed and running. It simplifies GUI rendering compared to external X servers.

   Ensure your WSLg environment supports GPU features. Run:

   ```bash
   glxinfo | grep OpenGL
   ```

   If it doesn’t list GPU capabilities or mentions software rendering, your WSLg installation doesn’t have proper GPU support.

---

### **On macOS**

1. **Install Homebrew (if not already installed)**:

   ```bash
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
   ```

2. **Update Homebrew**:

   ```bash
   brew update
   ```

3. **Install Qt5**:

   ```bash
   brew install qt@5
   ```

4. **Set Up the PATH**:
   Add Qt5 binaries to your shell profile (e.g., `.zshrc` or `.bashrc`):

   ```bash
   export PATH="/usr/local/opt/qt@5/bin:$PATH"
   ```

5. **Verify Installation**:
   ```bash
   qmake --version
   ```

---

### **On Windows**

1. **Download Qt5**:
   Go to the [official Qt downloads page](https://download.qt.io/official_releases/qt/) and choose the Qt5 version for your platform.

2. **Run the Installer**:

   - Choose "Open Source" license if applicable.
   - Select Qt5 modules (including `Qt WebEngine` if needed).

3. **Set Up PATH**:
   Add the `bin` directory of Qt5 to your system's PATH variable (e.g., `C:\Qt\5.x.x\msvcXXXX\bin`).

4. **Verify Installation**:
   Open `cmd` or `PowerShell` and run:
   ```cmd
   qmake --version
   ```

---

### **Testing the Installation**

Create a basic Qt project to verify everything works:

1. Create a new file `main.cpp`:

   ```cpp
   #include <QApplication>
   #include <QLabel>

   int main(int argc, char *argv[]) {
       QApplication app(argc, argv);
       QLabel label("Hello, Qt5!");
       label.show();
       return app.exec();
   }
   ```

2. Create a `.pro` file:

   ```pro
   QT += widgets
   CONFIG += c++17
   TARGET = HelloQt
   TEMPLATE = app
   SOURCES += main.cpp
   ```

3. Build and Run:
   ```bash
   qmake
   make
   ./HelloQt
   ```
