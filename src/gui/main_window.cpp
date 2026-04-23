#include <QApplication>
#include <QMessageBox>
#include <unistd.h>   // getuid()
#include "gui/main_window.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("TCP SYN Flood Detector");
    app.setApplicationVersion("1.0");

    // Warn if not root (packet capture requires root on Linux)
    if (getuid() != 0) {
        QMessageBox::warning(nullptr, "Privilege Warning",
            "This application requires root privileges for packet capture.\n\n"
            "Please restart with: sudo ./SynFloodDetector\n\n"
            "The UI will launch but capture will fail without root.");
    }

    MainWindow window;
    window.show();
    return app.exec();
}s