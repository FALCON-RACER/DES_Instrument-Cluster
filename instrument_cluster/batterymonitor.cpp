#include "batterymonitor.h"
#include <QDebug>

BatteryMonitor::BatteryMonitor(const QString& i2cDevice, uint8_t address, QProgressBar *progressBar, QObject *parent)
    : QObject(parent)
    , ina219(new INA219(i2cDevice.toStdString().c_str(), address))
    , progressBar(progressBar)
    , lowBatteryVoltage(9.5)
{
    // QTimer 객체 생성
    timer = new QTimer(this);

    updateBatteryVoltage();

    // QTimer의 timeout 신호를 updateBatteryVoltage 슬롯에 연결
    connect(timer, &QTimer::timeout, this, &BatteryMonitor::updateBatteryVoltage);
    timer->start(60000);    // send battery data every 1 min (1000ms = 1 sec)
}

BatteryMonitor::~BatteryMonitor() {
    delete ina219;
}

void BatteryMonitor::updateBatteryVoltage() {

    float voltage = ina219->getBatteryVoltage();
    float voltagePercent = (voltage - lowBatteryVoltage) / 0.029;

    if (voltagePercent >= 100)
        voltagePercent = 100;
    else if (voltagePercent <= 0)
        voltagePercent = 0;


    progressBar->setValue(voltagePercent);

    qDebug() << "Battery Voltage:" << voltage << "V";
}
