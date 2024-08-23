#include "batterymonitor.h"
#include <QDebug>

BatteryMonitor::BatteryMonitor(const QString& i2cDevice, uint8_t address, QObject *parent)
    : QObject(parent), ina219(new INA219(i2cDevice.toStdString().c_str(), address))
{
    // QTimer 객체 생성
    timer = new QTimer(this);

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
    qDebug() << "Battery Voltage:" << voltage << "V";

    // QLabel의 텍스트 업데이트
    voltageLabel->setText(QString::number(voltage, 'f', 2) + " V");
}
