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
bool CANReceiver::connectToBus(const QString &interfaceName)
{
    if (canDevice)
    {
        qDebug() << "Already connected to CAN bus.";
        return false;
    }

    canDevice = QCanBus::instance()->createDevice("socketcan", interfaceName);
    if (!canDevice)
    {
        qDebug() << "Failed to create CAN device for interface:" << interfaceName;
        return false;
    }

    if (!canDevice->connectDevice())
    {
        qDebug() << "Failed to connect to CAN device.";
        delete canDevice;
        canDevice = nullptr;
        return false;
    }

    qDebug() << "canDevice->busStatus : " << canDevice->busStatus();
    qDebug() << "Frames available: " << canDevice->framesAvailable();
    qDebug() << "CAN device error: " << canDevice->errorString();

    bool a = connect(canDevice, &QCanBusDevice::framesReceived, this, &CANReceiver::handleNewData);

    return true;
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
