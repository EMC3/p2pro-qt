#include "mainwindow.h"

#include <QApplication>
#include <unistd.h>

#include "logger/log.h"
#include "qstylefactory.h"

int main(int argc, char *argv[])
{
    Logger::init(0, false, true);

    QApplication a(argc, argv);

#ifdef ANDROID
// To hack for running on android:
// ->su -c setenforce 0
// ->su -c chown <current user> /dev/video3
    system("su -c setenforce 0");
    system(("su -c chown "+std::to_string(geteuid())+" /dev/video3").c_str());
    QApplication::setStyle(QStyleFactory::create("fusion"));
#endif


    MainWindow w;
    w.show();

    return a.exec();
}
