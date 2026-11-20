#ifndef SENSORBOARD_H
#define SENSORBOARD_H

/* Stefan Larndorfer, stefan.larndorfer@sequality.at
 * Embedded Visualization - EVI5, FH-Hagenberg
 *
 * Getestet am RaspberryPi3 mit FH-Sensor-Board, Yocto Linux
 * Abhängigkeit: ausführbares home/root/setupLedsSwitch.sh
*/

#include <QFile>
#include <QObject>
#include <QSocketNotifier>
#include <QString>
#include <QStringLiteral>
#include <QStringLiteral>

class SensorBoard : public QObject
{
    Q_OBJECT
public:
    SensorBoard();
    ~SensorBoard();

    void setGreenLED(bool active);
    void setOrangeLED(bool active);
    void setRedLED(bool active);

    int getButtonSwitch1();
    int getButtonSwitch2();

    double getTemperature();

    void getCPUload();

signals:
    void buttonSwitch1Changed(int value);
    void buttonSwitch2Changed(int value);

    void temperatureAvailable(double value);
    void cpuLoadAvailable(double value);

private:
    QString gpioPathPattern;
    QString tempPathPattern;

    QFile* switch1File;
    QFile* switch2File;
    QSocketNotifier* switch1Notifier;
    QSocketNotifier* switch2Notifier;
    int switch1state;
    int switch2state;

    int setupLedsAndSwitches();

    void writeFile(QString path, bool value);
    QString readFile(QString path);

    double parseTemperatureProcessOutput(QString output);

private slots:
    void setupSwitch1Notifier();
    void setupSwitch2Notifier();
    void switch1NotifierActivated();
    void switch2NotifierActivated();

};

#endif // SENSORBOARD_H
