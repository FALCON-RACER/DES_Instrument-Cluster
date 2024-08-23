#ifndef BATTERYMONITOR_H
#define BATTERYMONITOR_H

#include <QObject>
#include <QTimer>
#include "ina219.h" // INA219 클래스의 헤더 파일
#include <QLabel>

class BatteryMonitor : public QObject
{
    Q_OBJECT
public:
    explicit BatteryMonitor(const QString& i2cDevice, uint8_t address, QLabel *label, QObject *parent = nullptr);
    ~BatteryMonitor();

public slots:
    void updateBatteryVoltage();

private:
    INA219 *ina219;
    QTimer *timer;
    QLabel *voltageLabel;  // QLabel 포인터
};


#endif // BATTERYMONITOR_H
