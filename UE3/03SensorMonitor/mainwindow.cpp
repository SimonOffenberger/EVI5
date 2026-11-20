#include "mainwindow.h"

#include "sensormonitor.h"
#include <QApplication>
#include <QStyleFactory>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    sensorMonitor = new SensorMonitor();
    this->setCentralWidget(sensorMonitor);

    // Unter Ubuntu Linux wird es anders "gerendert" als unter Raspberry-Pi
    // Der Style "Fusion" lässt die GUI auch am Raspberry PI gut aussehen
    QApplication::setStyle(QStyleFactory::create("Fusion"));
}

MainWindow::~MainWindow()
{

}
