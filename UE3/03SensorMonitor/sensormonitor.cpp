#include "sensormonitor.h"

#include <QVBoxLayout>
#include <QTimer>
#include <QTextStream>
#include <QStyle>
#include <QDebug>

SensorMonitor::SensorMonitor(QWidget *parent) : QWidget(parent)
{
    // Initialise variables
    ledStatusRed = false;
    ledStatusOrange = false;
    ledStatusGreen = false;

    // Title
    lblTitle = new QLabel("Sensor-Monitor");
    lblTitle->setStyleSheet("font-size: 26pt;");
    lblTitle->setAlignment(Qt::AlignCenter);

    // Toggle Leds
    lblLEDs = new QLabel("Toggle Leds:");
    lblLEDs->setStyleSheet("font-size: 18pt; color:darkgray;");

    btnLED1 = new QPushButton("LED 1");
    btnLED1->setStyleSheet("color:red;");

    btnLED2 = new QPushButton("LED 2");
    btnLED2->setStyleSheet("color:orange;");

    btnLED3 = new QPushButton("LED 3");
    btnLED3->setStyleSheet("color:green;");


    // Hardware Buttons
    lblHardwareButtons = new QLabel("Hardware Buttons");
    lblHardwareButtons->setStyleSheet("font-size: 18pt; color:darkgray;");

    btnHardwareButton1 = new QPushButton(" 1 ");
    btnHardwareButton1->setDisabled(true);  // button kann nur von der hardware aus gedrückt werden und nicht im GUI
    //btnHardwareButton1->setStyleSheet("background: blue");  // wenn die hardware-Taste gedrückt wird

    btnHardwareButton2 = new QPushButton(" 2 ");
    btnHardwareButton2->setDisabled(true);

    // Read Temperature
    lblTemp = new QLabel("Read Temperature:");
    lblTemp->setStyleSheet("font-size: 18pt; color:darkgray;");

    btnStartTemp = new QPushButton("Start");
    btnStopTemp = new QPushButton("Stop");

    lblFreq = new QLabel("Frequency:");
    lblFreq->setStyleSheet("font-size: 18pt; color:darkgray;");

    spinBoxFrequency = new QSpinBox();
    spinBoxFrequency->setMinimum(0);
    spinBoxFrequency->setValue(1);
    spinBoxFrequency->setSuffix(" Hz");

    temperatureBar = new QProgressBar();
    temperatureBar->setMinimum(0);
    temperatureBar->setMaximum(100);
    temperatureBar->setFormat("Temperature: - °C");
    temperatureBar->setTextVisible(true);
    temperatureBar->setValue(50);
    temperatureBar->setAlignment(Qt::AlignCenter);


    // Read CPU load
    lblCPUload = new QLabel("CPU Usage:");
    lblCPUload->setStyleSheet("font-size: 18pt; color:darkgray;");

    cpuLoadBar = new QProgressBar();
    cpuLoadBar->setMinimum(0);
    cpuLoadBar->setMaximum(100);
    cpuLoadBar->setFormat("CPU Usage: - %");
    cpuLoadBar->setTextVisible(true);
    cpuLoadBar->setValue(0);
    cpuLoadBar->setAlignment(Qt::AlignCenter);


    // QStyle definiert kompleeten Style aller Controls, man kann auch
    // ein Stylesheet (ähnlich wie bei HTML) für gewisse Widgets oder die gesamte Anwendung defineren
    this->setStyleSheet("MainWindow {background-color:white}"
                        "QPushButton {font-size:18pt}"
                        "QProgressBar {font-size:18pt}"
                        "QSpinBox {font-size:18pt}"
                        "QSpinBox::up-button { width: 32px; }"
                        "QSpinBox::down-button { width: 32px; }"
                        "QProgressBar::chunk {background-color:lightblue}");


    // Main Layout
    QVBoxLayout* vBox = new QVBoxLayout();
    vBox->setDirection(QBoxLayout::Direction::TopToBottom);

    // Led button layout
    QVBoxLayout* vLedButtonBox = new QVBoxLayout();
    vLedButtonBox->setDirection(QBoxLayout::Direction::LeftToRight);
    vLedButtonBox->addWidget(btnLED1);
    vLedButtonBox->addWidget(btnLED2);
    vLedButtonBox->addWidget(btnLED3);

    // Led button layout
    QVBoxLayout* vHardwareButtonBox = new QVBoxLayout();
    vHardwareButtonBox->setDirection(QBoxLayout::Direction::LeftToRight);
    vHardwareButtonBox->addWidget(btnHardwareButton1);
    vHardwareButtonBox->addWidget(btnHardwareButton2);

    // Temperature button and label layout
    QVBoxLayout* vTempBox = new QVBoxLayout();
    vTempBox->setDirection(QBoxLayout::Direction::LeftToRight);
    vTempBox->addWidget(btnStartTemp);
    vTempBox->addWidget(btnStopTemp);
    vTempBox->addWidget(lblFreq);
    vTempBox->addWidget(spinBoxFrequency);

    // add all components to the main layout
    vBox->addWidget(lblTitle);
    vBox->addWidget(lblLEDs);
    vBox->addLayout(vLedButtonBox);
    vBox->addWidget(lblHardwareButtons);
    vBox->addLayout(vHardwareButtonBox);
    vBox->addWidget(lblTemp);
    vBox->addLayout(vTempBox);
    vBox->addWidget(temperatureBar);
    vBox->addWidget(lblCPUload);
    vBox->addWidget(cpuLoadBar);

    // set the created layout
    this->setLayout(vBox);

    // connect the slots
    connect(btnLED1,SIGNAL(clicked()),this,SLOT(btnLED1Clicked()));
    connect(btnLED2,SIGNAL(clicked()),this,SLOT(btnLED2Clicked()));
    connect(btnLED3,SIGNAL(clicked()),this,SLOT(btnLED3Clicked()));
    connect(btnStartTemp,SIGNAL(clicked()),this,SLOT(btnStartTempClicked()));
    connect(btnStopTemp,SIGNAL(clicked()),this,SLOT(btnStopTempClicked()));
    connect(spinBoxFrequency,SIGNAL(valueChanged(int)),this,SLOT(spinBoxChanged()));

    // other hardware specific initializations
    readTempTimer = new QTimer(this);
    sensorBoard = nullptr;

#ifdef Q_PROCESSOR_ARM
    sensorBoard = new SensorBoard();

    setHardwareButton1(sensorBoard->getButtonSwitch1());
    setHardwareButton2(sensorBoard->getButtonSwitch2());
    connect(this->sensorBoard, &SensorBoard::buttonSwitch1Changed, this, &SensorMonitor::setHardwareButton1);
    connect(this->sensorBoard, &SensorBoard::buttonSwitch2Changed, this, &SensorMonitor::setHardwareButton2);

    readTempTimer->setInterval(1000/spinBoxFrequency->value());
    connect(readTempTimer, &QTimer::timeout, this, &SensorMonitor::readTemperature);
    connect(sensorBoard, &SensorBoard::temperatureAvailable, this, &SensorMonitor::setTemperatureAvailable);
    connect(sensorBoard, &SensorBoard::cpuLoadAvailable, this, &SensorMonitor::setCPULoadAvailable);
#endif
}

void SensorMonitor::btnLED1Clicked()
{
    this->ledStatusRed = !this->ledStatusRed;
    if(ledStatusRed) {
        btnLED1->setStyleSheet("background-color: red;");
    } else {
        btnLED1->setStyleSheet("color: red;");
    }
#ifdef Q_PROCESSOR_ARM
    this->sensorBoard->setRedLED(this->ledStatusRed);
#endif
}

void SensorMonitor::btnLED2Clicked()
{
    this->ledStatusOrange = !this->ledStatusOrange;
    if(ledStatusOrange) {
        btnLED2->setStyleSheet("background-color: orange;");
    } else {
        btnLED2->setStyleSheet("color: orange;");
    }
#ifdef Q_PROCESSOR_ARM
    this->sensorBoard->setOrangeLED(this->ledStatusOrange);
#endif
}

void SensorMonitor::btnLED3Clicked()
{
    this->ledStatusGreen = !this->ledStatusGreen;
    if(ledStatusGreen == 1) {
        btnLED3->setStyleSheet("background-color: green;");
    } else {
        btnLED3->setStyleSheet("color: green;");
    }
#ifdef Q_PROCESSOR_ARM
    this->sensorBoard->setGreenLED(this->ledStatusGreen);
#endif
}

void SensorMonitor::readTemperature()
{
    //nicht blockierende Variante
    sensorBoard->getTemperature();  // signal temperatureAvailable wird gefeuert

    //blockierende Variante
//    double temperature = sensorBoard->getTemperature();
//    temperatureBar->setValue(qRound(temperature));
//    temperatureBar->setFormat("Temperature: " + QString::number(temperature,'f',4) + " °C");

    //Aktualisierung CPU-Usage
    // top -n 1 -b | awk '/^%Cpu/{print $2}'
    sensorBoard->getCPUload();
}

void SensorMonitor::setTemperatureAvailable(double value)
{
    temperatureBar->setValue(qRound(value));
    temperatureBar->setFormat("Temperature: " + QString::number(value,'f',4) + " °C");
}

void SensorMonitor::setCPULoadAvailable(double value)
{
    cpuLoadBar->setValue(qRound(value));
    cpuLoadBar->setFormat("CPU Usage: " + QString::number(value,'f',1) + " %");
}

void SensorMonitor::spinBoxChanged()
{
    readTempTimer->setInterval(1000/spinBoxFrequency->value());
}

void SensorMonitor::btnStartTempClicked()
{
    if(!readTempTimer->isActive())
    {
        readTempTimer->start();
    }
}


void SensorMonitor::btnStopTempClicked()
{
    if(readTempTimer->isActive())
    {
        readTempTimer->stop();
        temperatureBar->setFormat("Temperature: - °C");
        temperatureBar->setValue(50);
    }
}

void SensorMonitor::setHardwareButton1(int value)
{
    if (value) {
        this->btnHardwareButton1->setStyleSheet("background:white; color: black");
    } else {
        this->btnHardwareButton1->setStyleSheet("background:black; color: white");
    }
}

void SensorMonitor::setHardwareButton2(int value)
{
    if (value) {
        this->btnHardwareButton2->setStyleSheet("background:white; color: black");
    } else {
        this->btnHardwareButton2->setStyleSheet("background:black; color: white");
    }
}

