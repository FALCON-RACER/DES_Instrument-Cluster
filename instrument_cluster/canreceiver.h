#ifndef CANRECEIVER_H
#define CANRECEIVER_H

#include <QObject>
#include <QCanBus>
#include <QCanBusDevice>
#include <QCanBusFrame>
#include <string>

/**
 * @brief The CANReceiver class
 *
 * For the get Can Bus data
 */

#define INTERFACE_NAME "can0"

class CANReceiver : public QObject
{
    Q_OBJECT

public:
    explicit CANReceiver(QObject *parent = nullptr, const QString interfaceName = INTERFACE_NAME);
    ~CANReceiver();

    void connectToBus();
    void disconnectFromBus();

signals:
    void newMessageReceived(const QCanBusFrame &frame);

private slots:
    void handleNewData();

private:
    const QString interfaceName;
    QCanBusDevice *canDevice;
};

#endif
