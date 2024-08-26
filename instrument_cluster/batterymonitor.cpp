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

    // 타이머 주기 설정 (1000ms = 1초)
    timer->start(1000);
}

BatteryMonitor::~BatteryMonitor() {
    delete ina219;
}

void BatteryMonitor::updateBatteryVoltage() {

    float voltage = ina219->getBatteryVoltage();
    float voltagePercent = (voltage - lowBatteryVoltage) / 0.036;

    qDebug() << "Battery Voltage:" << voltage << "V";

    progressBar->setValue(voltagePercent);
}
