#include "sensorboard.h"

#include <QFileInfo>
#include <QDebug>
#include <QProcess>
#include <QTimer>

SensorBoard::SensorBoard()
{
    gpioPathPattern = "/sys/class/gpio/gpio%1/value";
    tempPathPattern
        = "/sys/bus/w1/devices/28-000008f69ca5/w1_slave"; // TODO: change this to your temperature-sensor path
    setupLedsAndSwitches();
}

SensorBoard::~SensorBoard()
{
    switch1File->close();
    switch2File->close();
    switch1File->deleteLater();
    switch2File->deleteLater();
}

int SensorBoard::setupLedsAndSwitches()
{
    int retVal = 0;
    // GPIO-Script setupLedsAndSwitches.sh ausführen
    if (!QFileInfo::exists("/sys/class/gpio/gpio17/")) {
        qDebug() << "executing GPIO setup script";
        retVal = QProcess::execute("/bin/sh", QStringList() << "/home/root/setupLedsSwitch.sh");
        if (retVal!=0) {
            qDebug() << "error at executing setupLedsSwitch.sh: Missing file?";
        }
    }

    // Setup QSocketNotifier für Taster-Events
    this->setupSwitch1Notifier();
    this->setupSwitch2Notifier();

    return retVal;
}

void SensorBoard::setupSwitch1Notifier()
{
    this->switch1File = new QFile(gpioPathPattern.arg(5));
    this->switch1state = this->getButtonSwitch1();

    if (this->switch1File->open(QFile::ReadOnly)) {
        this->switch1File->readAll();
        qDebug() << "Set up SocketNotifier for " << this->switch1File->fileName();
        this->switch1Notifier = new QSocketNotifier(this->switch1File->handle(),QSocketNotifier::Read);
        //this->switch1Notifier->setEnabled(true);

        connect(this->switch1Notifier,SIGNAL(activated(int)), this, SLOT(switch1NotifierActivated()));
    }
}

void SensorBoard::setupSwitch2Notifier()
{
    this->switch2File = new QFile(gpioPathPattern.arg(6));
    this->switch2state = this->getButtonSwitch2();

    if (this->switch2File->open(QFile::ReadOnly)) {
        this->switch2File->readAll();
        qDebug() << "Set up SocketNotifier for " << this->switch2File->fileName();
        this->switch2Notifier = new QSocketNotifier(this->switch2File->handle(),QSocketNotifier::Read);

        connect(this->switch2Notifier,SIGNAL(activated(int)), this, SLOT(switch2NotifierActivated()));
    }
}


void SensorBoard::setGreenLED(bool active)
{
    this->writeFile(gpioPathPattern.arg(17),active);
}

void SensorBoard::setOrangeLED(bool active)
{
    this->writeFile(gpioPathPattern.arg(27),active);
}

void SensorBoard::setRedLED(bool active)
{
    this->writeFile(gpioPathPattern.arg(22),active);
}

int SensorBoard::getButtonSwitch1()
{
    return this->readFile(gpioPathPattern.arg(5)).toInt();
}

int SensorBoard::getButtonSwitch2()
{
    return this->readFile(gpioPathPattern.arg(6)).toInt();
}

/// Hier gibt es eine blockierende und nicht blockierende Variante (im Code als Kommentar gekennzeichnet)
/// Die blockierende Variente liefert einen Rückgabewert zurück.
/// Die nicht blockierende Variante liefert immer 0 als Rückgabewert zurück und
/// emmitiert ein Signal in temperatureAvailable(), sobald das Ergebnis verfügbar ist.
/// Nicht-blockierend aber trotzdem gekapselt in einem Funktionsaufruf ist
/// in C++ Qt möglich mit Futures, z.B. mit QtPromise: https://qtpromise.netlify.app/qtpromise/getting-started.html#example
double SensorBoard::getTemperature()
{
    double retVal = 0.0;

    if (QFileInfo::exists(tempPathPattern))
    {
        QString program = "cat";
        QStringList arguments = {tempPathPattern};
        QProcess* myProcess = new QProcess();
        myProcess->start(program,arguments);
        // nicht blockierende Variante (=asynchrone Ausführung)
        connect(myProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [=]() {
            QString output(
                myProcess
                    ->readAllStandardOutput()); // output should be sth like: 37 02 4b 46 7f ff 09 10 61 : crc=61 YES\n37 02 4b 46 7f ff 09 10 61 t=35437
            myProcess->deleteLater();
            emit temperatureAvailable(parseTemperatureProcessOutput(output));
        });

        // blockierende Variante (=synchrone Ausführung), Problem: Buttons in der GUI sind während waitforFinished() nicht benutzbar da Main-Event-Loop nicht ausgeführt wird.
        //        myProcess->waitForFinished(); // blockiert die Main-Event-Loop bis das Ergebnis vorliegt
        //        QString output(myProcess->readAllStandardOutput()); // output should be sth like: 37 02 4b 46 7f ff 09 10 61 : crc=61 YES\n37 02 4b 46 7f ff 09 10 61 t=35437
        //        retVal = parseTemperatureProcessOutput(output);
        //        myProcess->deleteLater();


    }
    return retVal;
}

void SensorBoard::getCPUload()
{
    // top -n 1 -b | awk '/^%Cpu/{print $2}'
    // Pipes | können nicht als QProcess-Argumente übergeben werden, darum muss das gesamte Kommando der bash übergeben werden, siehe https://stackoverflow.com/questions/20901884/piping-or-command-chaining-with-qprocess
    QString program = "bash";
    QStringList arguments = {"-c", "top -n 1 -b | awk '/^%Cpu/{print $2}'"};
    QProcess* myProcess = new QProcess();
    myProcess->start(program,arguments);
    // nicht blockierende Variante (=asynchrone Ausführung)
    connect(myProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [=]() {
        QString output(
            myProcess
                ->readAllStandardOutput()); // output should be sth like: 37 02 4b 46 7f ff 09 10 61 : crc=61 YES\n37 02 4b 46 7f ff 09 10 61 t=35437
        //qDebug() << "cpuloadavailable: " << output;
        myProcess->deleteLater();
        emit cpuLoadAvailable(output.toDouble());
    });
}

double SensorBoard::parseTemperatureProcessOutput(QString output)
{
    QStringList list = output.split("t="); // get access to the number after "t="
    if (list.size()>1)
    {
        return list[1].toInt()*1.0/1000;
    }
    return 0;
}


void SensorBoard::switch1NotifierActivated()
{
    int buttonValue = getButtonSwitch1();
    if (this->switch1state != buttonValue) // Steigende/fallende Flanke detektieren
    {
        this->switch1state = buttonValue;
        qDebug() << "switch1NotifierActivated: " << buttonValue;
        emit buttonSwitch1Changed(buttonValue);
    }
}

void SensorBoard::switch2NotifierActivated()
{
    int buttonValue = getButtonSwitch2();
    if (this->switch2state != buttonValue) // Steigende/fallende Flanke detektieren
    {
        this->switch2state = buttonValue;
        qDebug() << "switch2NotifierActivated: " << buttonValue;
        emit buttonSwitch2Changed(buttonValue);
    }
}

void SensorBoard::writeFile(QString path, bool value)
{
    QFile file(path);
    if (!file.open(QFile::WriteOnly))
    {
        qCritical() << file.errorString();
        Q_ASSERT(false);
    }
    file.write(QString::number((int)value).toUtf8());
    file.close();
}

QString SensorBoard::readFile(QString path)
{
    QFile file(path);
    QTextStream textStream(&file);
    QString fileContent;
    if (!file.open(QFile::ReadOnly))
    {
        qCritical() << file.errorString();
        Q_ASSERT(false);
    }
    fileContent = textStream.readAll();
    file.close();
    return fileContent;
}





