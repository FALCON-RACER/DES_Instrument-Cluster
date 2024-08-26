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
class CANReceiver : public QObject
{
    Q_OBJECT

public:
    explicit CANReceiver(QObject *parent = nullptr);
    ~CANReceiver();

    void connectToBus(const QString &interfaceName);
    void disconnectFromBus();

signals:
    void newMessageReceived(const QCanBusFrame &frame);

private slots:
    void handleNewData();

private:
    QCanBusDevice *canDevice;
};

#endif
