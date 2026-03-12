#include "src/mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setStyleSheet(R"(
        QWidget {
            font-family: "Segoe UI", "Arial";
            color: #103447;
            font-size: 13px;
        }

        #filtrationRoot {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #D9F3F7, stop:1 #A9DEE2);
        }

        #alumRoot {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #D8F3F7, stop:1 #A7E0E7);
        }
    )");

    MainWindow window;
    window.show();

    return app.exec();
}
