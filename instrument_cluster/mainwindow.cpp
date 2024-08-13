#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "canreceiver.h"

#include <QDebug>
#include <QThread>
#include <QVBoxLayout>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , gauge(new CircularGauge(this))  // CircularGauge 초기화
    , emaFilter(new EmaFilter(0.25))
{
    ui->setupUi(this);


    // Create a central widget
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Create a layout and add the gauge to it
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->addWidget(gauge);

    setFixedSize(1280, 400);
    setStyleSheet("background-color:black");
    showFullScreen();


    // Set CAN Bus
    // Start CANReceiver another thread
    QThread *canThread = new QThread;
    CANReceiver *canReceiver = new CANReceiver(this);

    canReceiver->moveToThread(canThread);

    // Connect CANReceiver's signal and function
    QObject::connect(canReceiver, &CANReceiver::newMessageReceived, this, &MainWindow::updateAnimation);


    // Connect to the CAN bus when the thread is started
    QObject::connect(canThread, &QThread::started, canReceiver, [canReceiver]() {
        canReceiver->connectToBus("can0");
    });


    // Delete CANReceiver object when thread end
    QObject::connect(canThread, &QThread::finished, canReceiver, &QObject::deleteLater);
    QObject::connect(canThread, &QThread::finished, canThread, &QObject::deleteLater);

    canThread->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateAnimation(const QCanBusFrame &frame)
{
    float rpm;
    memcpy(&rpm, frame.payload(), sizeof(rpm));

    double speed = calculateSpeed(rpm);

    gauge->startAnimation(speed, 280);

    qDebug() << "rpm : " << rpm;
    qDebug() << "speed : " << speed;

    // QString message = QString("ID: %1 Data: %2")
    //                       .arg(frame.frameId(), 0, 16)
    //                       .arg(frame.payload().toHex().constData());
}

double MainWindow::calculateSpeed(double rpm) {

    const double radius = 6.7;
    const double PI = M_PI;

    // cm/s
    double speed = (rpm / 60) * PI * radius;

    return emaFilter->Run(speed);
    // return speed;
}
