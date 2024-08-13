#include "mainwindow.h"

#include <QApplication>
#include <QShortcut>
#include <QKeySequence>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow mainWindow;

    // exit shortcut (Ctrl + Q)
    QShortcut *ctrlQShortcut = new QShortcut(QKeySequence("Ctrl+Q"), &mainWindow);
    QObject::connect(ctrlQShortcut, &QShortcut::activated, &app, &QApplication::quit);

    // exit shortcut (Command + Q for macOS)
    QShortcut *cmdQShortcut = new QShortcut(QKeySequence("Meta+Q"), &mainWindow);
    QObject::connect(cmdQShortcut, &QShortcut::activated, &app, &QApplication::quit);


    mainWindow.show();
    return app.exec();
}
