#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "canreceiver.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->data_label->setText("whhhyyyyy");
    // qDebug() <<

    CANReceiver canReceiver(ui->data_label);

    // Connect CANReceiver signals to MainWindow slots)
    QObject::connect(&canReceiver, &CANReceiver::newMessageReceived, this, &MainWindow::updateLabel);

    // Initialize CANReceiver and connect to CAN bus
    QString a = canReceiver.connectToBus("can0");
    if (a != "")
    {
        // ui->data_label->setText("Failed to connect to CAN bus.");
        ui->data_label->setText(a);

    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateLabel(const QCanBusFrame &frame)
{
    QString message = QString("ID: %1 Data: %2")
                          .arg(frame.frameId(), 0, 16)
                          .arg(frame.payload().toHex().constData());
    ui->data_label->setText(message);  // Update the label with the received message
}
