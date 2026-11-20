#include "sensormonitor.h"

#include <QVBoxLayout>
#include <QTimer>
#include <QTextStream>
#include <QStyle>
#include <QDebug>
#include <QProcess>
#include <QFileInfo>

#include <QtGlobal>  // Plattform-Erkennung
#include <QProcess>  // Prozesse starten
#include <QDebug>    // Debug-Ausgaben
#include <cstdlib>   // std::system()
#include <QApplication>
#include <QThread>

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
    spinBoxFrequency->setMinimum(1); // wichtig, ansonsten Division durch 0 -> Absturz
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
    readTempTimer->setInterval(1000/spinBoxFrequency->value());
    connect(readTempTimer, &QTimer::timeout, this, &SensorMonitor::readTemperature);

}

void SensorMonitor::btnLED1Clicked()
{
    this->ledStatusRed = !this->ledStatusRed;
    if(ledStatusRed) {
        btnLED1->setStyleSheet("background-color: red;");
    } else {
        btnLED1->setStyleSheet("color: red;");
    }

    ///
    /// 01 Plattformspezifischen Code ausführen
    ///

    // Wie erkennt man die aktuelle Plattform?
    // <QtGlobal> , Präprozessor Variablen
    // QT_VERSION_STR
    // Q_PROCESSOR_ARM, Q_PROCESSOR_X86, Q_OS_LINUX

#ifdef Q_OS_LINUX
    qDebug()<< "Running Linux";
#else
    qDebug()<< "Running Windows";
#endif


#ifdef Q_PROCESSOR_ARM

    qDebug()<< " under RPI-3";

#endif

#ifdef Q_PROCESSOR_X86
    qDebug()<< "x86 Arch";
#endif


    // Fallunterscheidungen mit Präprozessor-Defines möglichst gering halten
    // Manchmal jedoch unumgänglich (z.B. unterschiedliche includes/libraries pro Plattform)

    // Besser: Platform-Abstraktion von Qt nutzen, z.B. möglich bei
    // * Pfaden auf Standard-Verzeichnisse (z.B. Home-Verzeichnis), siehe QStandardPaths::standardLocations
    // * Serielle Schnittstellen (RS232, CAN-Bus, UART,...), siehe Qt Serial Bus
    // * Sockets (TCP/UDP) und Datei-Verarbeitung (z.B QSockets, QFile, ...)
    // * Prozesse und Threads starten (z.B. QProcess, QThread)

#ifdef Q_PROCESSOR_ARM
    //system("reboot");
#else
    qApp->quit();
#endif


    ///
    /// 02 Plattformspezifische Systemaufrufe
    ///

    // mit system() einen reboot des Linux-Systems durchführen
    // C-Funktion call  (keine Qt-Funktion)
    //    siehe  https://www.geeksforgeeks.org/system-call-in-c/
    // + Einfach, unkompliziert
    // - Nicht performant, sehr ineffizient
    // - Keine String-Rückgabewerte möglich
    // - Keine Kontrolle über den gestarteten Prozess möglich, system()-Aufruf ist blockierend



    // Besser:  mit QProcess arbeiten
    // --> mehr Kontrolle über den Prozess (synchron, asynchron starten)
    // --> Rückgabewerte lesen, Konsolen-Ausgaben lesen

}

void SensorMonitor::btnLED2Clicked()
{
    this->ledStatusOrange = !this->ledStatusOrange;
    if(ledStatusOrange) {
        btnLED2->setStyleSheet("background-color: orange;");
    } else {
        btnLED2->setStyleSheet("color: orange;");
    }


    ///
    /// 03 QProcess
    ///    3.1 synchroner Aufruf
    ///    3.2 asynchrone Verarbeitung
    ///    3.3 QProcess mit pipes
    ///

       // 3.1 QProcess Standard-Output String auslesen und anzeigen aus einem Systembefehl, synchrone Verarbeitung
        // Freier Speicherplatz aller eingebundenen Filesysteme
        // Linux Befehl: df -ha


        //QProcess wurde ordnungsgemäß beendet (synchron)
    QProcess process1;
    QStringList args1;
    args1 << "-ha";
    process1.start("df",args1);

    if(!process1.waitForFinished()){
        qDebug() << "error after waiting process";
    }
    // Prozess wurde beendet synchron
    QByteArray arr =  process1.readAllStandardOutput();

    QString strout(arr);

    //qDebug() << "synchrone Ausführung";
    //qDebug().noquote() << strout;



    // 3.2 QProcess wie 3.1 mit asynchroner Verarbeitung
    // Linux Befehl: df -ha, mehr Zeit braucht z.B. sleep 5

    // QProcess * process2 = new QProcess;

    // // zu finished-Signal des process2 verbinden, siehe Qt-Hilfe https://doc.qt.io/qt-5/qprocess.html#finished
    // connect(process2, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
    // [=](int exitCode, QProcess::ExitStatus exitStatus){

    //     QByteArray ba = process2->readAllStandardOutput();
    //     QString process2Output(ba);
    //     qDebug() << "aynchrone Ausfuehrung:";
    //     qDebug().noquote() << process2Output;

    //     process2->deleteLater(); // damit wir kein Speicherloch bekommen
    // });

    //  process2->start("df",{"-ha"});


    // Optional für interessierte: Mit QtPromise (https://qtpromise.netlify.app/) kann man asynchrone Funktionsaufrufe noch besser
    // in Applikationen integrieren.

    // 3.3 QProcess mit Pipes verarbeiten, integer parsen, asynchron
    // Typische Befehle mit Pipes:  grep, tail, zB.: df -h | grep root
    // Freier Speicherpaltz in KB des root-Filesystems
    // Linux Befehl: df -k / | tail -1 | awk '{print $4}'


     QProcess* process3 = new QProcess();  // Objekt wird am heap angelegt, weil wir bei der asynchronen Verarbeitung diesen Block in dieser Methode verlassen
     process3->start("bash",{"-c", "df -k / | tail -1 | awk '{print $4}'"});
     connect(process3, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
             [=](int exitCode, QProcess::ExitStatus exitStatus){

                 QByteArray ba = process3->readAllStandardOutput();
                 QString process3Output(ba);
                 int freeSpaceInKB = process3Output.toInt();
                 qDebug() << "QProcess mit Pipe - freier Speicher in KB:" << freeSpaceInKB;
                 process3->deleteLater(); // damit wir kein Speicherloch bekommen
             });



}

void SensorMonitor::btnLED3Clicked()
{
    this->ledStatusGreen = !this->ledStatusGreen;
    if(ledStatusGreen == 1) {
        btnLED3->setStyleSheet("background-color: green;");
    } else {
        btnLED3->setStyleSheet("color: green;");
    }

    ///
    /// 4.0 LED3 am Sensor-Board einschalten und Taster auslesen
    ///
    ///

    if (!QFileInfo::exists("/sys/class/gpio/gpio17/")) {
        qDebug() << "executing GPIO setup script";
        QProcess::execute("/bin/sh", QStringList() << "/home/root/setupLedsSwitch.sh");
    }

    //Mit QProcess SYSFS-GPIO-Setup-Script ausführen
    //Linux shell script: /home/root/setupLedsSwitch.sh
    //Download http://www.sequality.at/download/evi5/setupLedsSwitch.sh
    //Kopieren von Ubuntu auf RPI-3: scp ./setupLedsSwitch.sh root@192.168.17.1:/home/root


    // Einschalten des grünen LED3 mittels SysFS Schnittstelle und QFile
    QString greenLedPath = "/sys/class/gpio/gpio17/value";
    QFile file(greenLedPath);
    if (!file.open(QFile::WriteOnly))
    {
        qCritical() << file.errorString();
        Q_ASSERT(false);
    }
    //file.write("1");  // minimal Variante: Led einschalten
    static bool ledStatusGreen = true; // damit wir hin und her schalten können
    file.write(QString::number((int)ledStatusGreen).toUtf8());
    ledStatusGreen = !ledStatusGreen;
    file.close();

    // Taster USER1 am SensorBoard mittels SysFS Schnittstelle und QFile auslesen


    // Taster USER1 am SensorBoard mittels SysFS Schnittstelle und QFile auslesen
    QString user1ButtonPath = "/sys/class/gpio/gpio5/value";
    QFile file2(user1ButtonPath);
    if (!file2.open(QFile::ReadOnly))
    {
        qCritical() << file2.errorString();
        Q_ASSERT(false);
    }
    QTextStream in(&file2);
    QString fileContent = in.readAll();
    int fileValue = fileContent.toInt();
    file2.close(); // Datei schließen
    qDebug() << "user1 button value: " << fileValue;
    this->setHardwareButton1(fileValue);


    // Problem hier beim Auslesen des GPIO-Signals: Wie bekommt man ein Signal, wenn der Benutzer den Hardware-Button drückt?
    // Möglichkeiten:
    //  - Busy Waiting Abfrage (schlechteste Performance)
    //  - Polling mit Timer (hohe Latenz, schlechte Performance)
    //  - Signal des Betriebssystems nutzen: sockets mit poll() oder QSocketNotifier (niedrige Latenz, gute Performance)
    //  - Bereitstellung über Device-Treiber (z.B. als Key-Event)


    // Nächster Schritt: SensorBoard Klasse für sämtliche Zugriffe nutzen (siehe 03SensorMonitor)
}

void SensorMonitor::readTemperature()
{
    qDebug() << "readTemperature";

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

