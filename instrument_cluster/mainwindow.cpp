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

    setWidgets();
    setScreenOptions();
    setCanBus();
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

    // QString message = QString("ID: %1 Data: %2")
    //                       .arg(frame.frameId(), 0, 16)
    //                       .arg(frame.payload().toHex().constData());
}

double MainWindow::calculateSpeed(double rpm) {

    const double radius = 6.7 / 2;
    const double PI = M_PI;

    double speed = (rpm / 60) * PI * radius * 2;    // cm/s

    return emaFilter->Run(speed);
}

void MainWindow::setWidgets() {

    // Create a central widget
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QHBoxLayout *layout = new QHBoxLayout(centralWidget);

    layout->addWidget(gauge);  // 중앙에 게이지 추가

    // 우측에 배터리 위젯을 위한 수직 레이아웃 생성
    QVBoxLayout *rightLayout = new QVBoxLayout;

    ui->battery->setMaximumWidth(200);
    ui->battery->setAlignment(Qt::AlignTop | Qt::AlignRight);
    rightLayout->addWidget(ui->battery);

    // 아래쪽 빈 공간을 차지하게 하는 스페이서 추가
    rightLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    // 수직 레이아웃을 수평 레이아웃에 추가
    layout->addLayout(rightLayout);
}

void MainWindow::setScreenOptions() {
    setFixedSize(1280, 400);
    setStyleSheet("background-color:black");
    showFullScreen();
}

void MainWindow::setCanBus() {

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
