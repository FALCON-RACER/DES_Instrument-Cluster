#include "canmanager.h"
#include <QThread>
#include <QDebug>

CANManager::CANManager(QObject *parent) : QObject(parent) {

    canThread = new QThread(this);
    canReceiver = new CANReceiver();

    canReceiver->moveToThread(canThread);

    connect(canReceiver, &CANReceiver::newMessageReceived, this, &CANManager::handleNewMessage);

    connect(canThread, &QThread::started, canReceiver, &CANReceiver::connectToBus);

    connect(canThread, &QThread::finished, canReceiver, &QObject::deleteLater);
    connect(canThread, &QThread::finished, canThread, &QObject::deleteLater);
}

CANManager::~CANManager() {

    if (canThread->isRunning()) {
        canThread->quit();
        canThread->wait();
    }
}

void CANManager::start() {

    canThread->start();
}


void CANManager::handleNewMessage(const QCanBusFrame &frame) {

    emit newMessageReceived(frame);
}
