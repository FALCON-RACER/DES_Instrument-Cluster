#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCanBusFrame>
#include "circulargauge.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    double calculateSpeed(double rpm);

public slots:
    void updateLabel(const QCanBusFrame &frame);

private:
    Ui::MainWindow *ui;
    CircularGauge *gauge;
};

#endif // MAINWINDOW_H
