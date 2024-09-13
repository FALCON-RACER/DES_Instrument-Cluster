#ifndef CANMANAGER_H
#define CANMANAGER_H

#include <QObject>
#include <QCanBusFrame>
#include "canreceiver.h"

class CANManager : public QObject
{
    Q_OBJECT

public:
    CANManager(QObject *parent = nullptr);
    ~CANManager();
    void start();

signals:
    void newMessageReceived(const QCanBusFrame &frame);

public slots:
    void handleNewMessage(const QCanBusFrame &frame);

private:
    QThread *canThread;
    CANReceiver *canReceiver;
};

#endif
