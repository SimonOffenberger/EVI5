#include "mainwindow.h"

#include <QApplication>

/** @brief Starts the Qt application and shows the chat window in full screen. */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setStyleSheet(R"(
        QWidget {
            font-family: "Liberation Sans";
            font-size: 16pt;
        }
    )");

    MainWindow w;

    w.showFullScreen();
    // w.show();
    return QApplication::exec();
}
