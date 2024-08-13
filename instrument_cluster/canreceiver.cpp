#include "canreceiver.h"
#include <QDebug>

CANReceiver::CANReceiver(QObject *parent) : QObject(parent), canDevice(nullptr) {}

CANReceiver::~CANReceiver()
{
    disconnectFromBus();
}

// Connect to CAN Bus using specified interface name
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

    connect(canDevice, &QCanBusDevice::framesReceived, this, &CANReceiver::handleNewData);

    return true;
}

// Disconnect with CAN bus device
void CANReceiver::disconnectFromBus()
{
    if (canDevice)
    {
        canDevice->disconnectDevice();
        delete canDevice;
        canDevice = nullptr;
    }
}

// emit signal if new CAN message arrived
void CANReceiver::handleNewData()
{
    while (canDevice->framesAvailable())
    {
        const QCanBusFrame frame = canDevice->readFrame();
        emit newMessageReceived(frame);
    }
}
