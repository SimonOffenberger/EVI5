#ifndef SENSORMONITOR_H
#define SENSORMONITOR_H

#include <QWidget>
#include <QPushButton>
#include <QProgressBar>
#include <QLabel>
#include <QSpinBox>

class SensorMonitor : public QWidget
{
    Q_OBJECT
public:
    explicit SensorMonitor(QWidget *parent = nullptr);

protected slots:
    void btnLED1Clicked();
    void btnLED2Clicked();
    void btnLED3Clicked();
    void btnStartTempClicked();
    void btnStopTempClicked();

    void setHardwareButton1(int value);
    void setHardwareButton2(int value);

    void readTemperature();
    void spinBoxChanged();

private:
    QLabel* lblTitle;
    QLabel* lblLEDs;
    QLabel* lblHardwareButtons;
    QLabel* lblCPUload;

    QLabel* lblTemp;
    QLabel* lblFreq;

    QPushButton* btnLED1;
    QPushButton* btnLED2;
    QPushButton* btnLED3;
    QPushButton* btnHardwareButton1;
    QPushButton* btnHardwareButton2;

    QPushButton* btnStartTemp;
    QPushButton* btnStopTemp;

    QSpinBox* spinBoxFrequency;

    QProgressBar* temperatureBar;
    QProgressBar* cpuLoadBar;

    bool ledStatusGreen;
    bool ledStatusOrange;
    bool ledStatusRed;

    QTimer* readTempTimer;

};

#endif // SENSORMONITOR_H
