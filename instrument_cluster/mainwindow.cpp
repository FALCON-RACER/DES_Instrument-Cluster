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
    , gauge(new CircularGauge(this))
    , emaFilter(new EmaFilter(0.25))
{
    ui->setupUi(this);
    battery = ui->battery;

    setWidgets();
    setScreenOptions();
    setCanBus("can0");
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

    layout->addWidget(gauge);  // add the gauge in the middle

    // Create a vertical layout for the battery widget
    QVBoxLayout *rightLayout = new QVBoxLayout;

    ui->battery->setMaximumWidth(200);
    ui->battery->setAlignment(Qt::AlignTop | Qt::AlignRight);
    rightLayout->addWidget(ui->battery);

    // Add a spacer to take up the empty space at the bottom
    rightLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    // Adding a vertical layout to a horizontal layout
    layout->addLayout(rightLayout);
}

void MainWindow::setScreenOptions() {
    setFixedSize(1280, 400);
    setStyleSheet("background-color:black");
    showFullScreen();
}

void MainWindow::setCanBus(const QString &interfaceName) {

    // Start CANReceiver another thread
    QThread *canThread = new QThread;
    CANReceiver *canReceiver = new CANReceiver(this);

    canReceiver->moveToThread(canThread);

    // Connect CANReceiver's signal and function
    QObject::connect(canReceiver, &CANReceiver::newMessageReceived, this, &MainWindow::updateAnimation);


    // Connect to the CAN bus when the thread is started
    QObject::connect(canThread, &QThread::started, canReceiver, [canReceiver, interfaceName]() {
        canReceiver->connectToBus(interfaceName);
    });


    // Delete CANReceiver object when thread end
    QObject::connect(canThread, &QThread::finished, canReceiver, &QObject::deleteLater);
    QObject::connect(canThread, &QThread::finished, canThread, &QObject::deleteLater);

    canThread->start();
}
