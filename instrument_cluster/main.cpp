#include "mainwindow.h"
#include "i2cexception.h"

#include "batterymonitor.h"
#include <QApplication>
#include <QShortcut>
#include <QKeySequence>
#include <iostream>

int main(int argc, char *argv[])
{
    try {

        QApplication app(argc, argv);
        MainWindow mainWindow;

        // exit shortcut (Ctrl + Q)
        QShortcut *ctrlQShortcut = new QShortcut(QKeySequence("Ctrl+Q"), &mainWindow);
        QObject::connect(ctrlQShortcut, &QShortcut::activated, &app, &QApplication::quit);

        // exit shortcut (Command + Q for macOS)
        QShortcut *cmdQShortcut = new QShortcut(QKeySequence("Meta+Q"), &mainWindow);
        QObject::connect(cmdQShortcut, &QShortcut::activated, &app, &QApplication::quit);

        BatteryMonitor monitor("/dev/i2c-1", 0x41);

        mainWindow.show();
        return app.exec();

    } catch (const I2CException& e) {
        std::cerr << e.what() << std::endl;
    }
}
