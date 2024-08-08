// CANReceiver.cpp
#include "canreceiver.h"
#include <QDebug>

CANReceiver::CANReceiver(QObject *parent) : QObject(parent), canDevice(nullptr)
{
}

CANReceiver::~CANReceiver()
{
    disconnectFromBus();
}

// 주어진 인터페이스 이름으로 CAN 버스 장치에 연결
QString CANReceiver::connectToBus(const QString &interfaceName)
{
    if (canDevice)
    {
        // qWarning() << "Already connected to CAN bus.";
        // return false;
        return "Already connected to CAN bus.";
    }

    canDevice = QCanBus::instance()->createDevice("socketcan", interfaceName);
    if (!canDevice)
    {
        // qWarning() << "Failed to create CAN device for interface:" << interfaceName;
        // return false;
        return "Failed to create CAN device for interface:";
    }

    if (!canDevice->connectDevice())
    {
        // qWarning() << "Failed to connect to CAN device.";
        delete canDevice;
        canDevice = nullptr;
        return "Failed to connect to CAN device.";
        // return false;
    }

    connect(canDevice, &QCanBusDevice::framesReceived, this, &CANReceiver::handleNewData);
    return "";
}

// CAN 버스 장치와의 연결을 해제
void CANReceiver::disconnectFromBus()
{
    if (canDevice)
    {
        canDevice->disconnectDevice();
        delete canDevice;
        canDevice = nullptr;
    }
}

// 새로운 CAN 메시지가 수신되면 newMessageReceived 신호를 방출
void CANReceiver::handleNewData()
{
    while (canDevice->framesAvailable())
    {
        const QCanBusFrame frame = canDevice->readFrame();
        emit newMessageReceived(frame);
    }
}
