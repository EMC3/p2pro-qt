#include "mainwindow.h"

#include <QApplication>

#include "logger/log.h"

int main(int argc, char *argv[])
{
    Logger::init(0, false, true);


    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
