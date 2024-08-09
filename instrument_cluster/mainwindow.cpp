#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "canreceiver.h"

#include <QDebug>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // CANReceiver를 별도의 스레드에서 실행하도록 설정
    QThread *canThread = new QThread;

    // CANReceiver *canReceiver = new CANReceiver();
    CANReceiver *canReceiver = new CANReceiver(this);

    canReceiver->moveToThread(canThread);

    // CANReceiver의 시그널과 메인 윈도우 슬롯 연결
    QObject::connect(canReceiver, &CANReceiver::newMessageReceived, this, &MainWindow::updateLabel);


    // 스레드 시작 시 CAN 버스에 연결
    QObject::connect(canThread, &QThread::started, canReceiver, [canReceiver]() {
        canReceiver->connectToBus("can0");
    });


    // 스레드가 종료되면 CANReceiver 객체 삭제
    QObject::connect(canThread, &QThread::finished, canReceiver, &QObject::deleteLater);
    QObject::connect(canThread, &QThread::finished, canThread, &QObject::deleteLater);

    // 스레드 시작
    canThread->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateLabel(const QCanBusFrame &frame)
{
    float data;

    memcpy(&data, frame.payload(), sizeof(data));
    qDebug() << "data : " << data;

    ui->data_label->setText(QString::number(data));

    // QString message = QString("ID: %1 Data: %2")
    //                       .arg(frame.frameId(), 0, 16)
    //                       .arg(frame.payload().toHex().constData());
}
