#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "canreceiver.h"
#include "ina219.h"

#include "batterymonitor.h"
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
    battery = ui->battery;

    // Create a central widget
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // // Create a layout and add the gauge to it
    // QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    // layout->addWidget(gauge);

    QHBoxLayout *layout = new QHBoxLayout(centralWidget);

    // 게이지를 중앙에 위치시키기 위한 스페이서 추가
    // layout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
    layout->addWidget(gauge);  // 중앙에 게이지 추가
    // layout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));

    // 우측에 배터리 위젯을 위한 수직 레이아웃 생성
    QVBoxLayout *rightLayout = new QVBoxLayout;
    // ui->Battery->setValue(40);

    ui->battery->setMaximumWidth(200);
    ui->battery->setAlignment(Qt::AlignTop | Qt::AlignRight);
    rightLayout->addWidget(ui->battery);

    // 아래쪽 빈 공간을 차지하게 하는 스페이서 추가
    rightLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    // 수직 레이아웃을 수평 레이아웃에 추가
    layout->addLayout(rightLayout);


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
