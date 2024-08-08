#ifndef CANRECEIVER_H
#define CANRECEIVER_H

#include <QObject>
// #include <QtNetwork/QUdpSocket>
#include <QCanBus>
#include <QCanBusDevice>
#include <QCanBusFrame>

class CANReceiver : public QObject
{
    Q_OBJECT

public:
    explicit CANReceiver(QObject *parent = nullptr);
    ~CANReceiver();

    QString connectToBus(const QString &interfaceName);
    void disconnectFromBus();

signals:
    void newMessageReceived(const QCanBusFrame &frame);

private slots:
    void handleNewData();

private:
    QCanBusDevice *canDevice;
};

#endif // CANRECEIVER_H
