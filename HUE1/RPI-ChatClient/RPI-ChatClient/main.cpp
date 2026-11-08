#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setStyleSheet(R"(
    QWidget {
        font-family: "Arial", "Noto Color Emoji";
        font-size: 16pt;
    }
    )");

    MainWindow w;

    w.showFullScreen();
    // w.show();
    return QApplication::exec();
}
