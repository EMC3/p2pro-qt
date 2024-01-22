#include "mainwindow.h"

#include <QApplication>
#include <QTextStream>
#include <unistd.h>

#include "logger/log.h"
#include "qstylefactory.h"
#include "displaywindow.h"

#include <QFile>

int main(int argc, char *argv[])
{
    Logger::init(0, false, true);

    QApplication a(argc, argv);

    QApplication::setStyle(QStyleFactory::create("fusion"));


#ifdef ANDROID
    // To hack for running on android:
    // ->su -c setenforce 0
    // ->su -c chown <current user> /dev/videoX
    system("su -c setenforce 0");
    //system(("su -c chown "+std::to_string(geteuid())+" /dev/video3").c_str());
#endif

    QFile f(":/qdarkstyle/dark/darkstyle.qss");

    if (!f.exists())   {
        printf("Unable to set stylesheet, file not found\n");
    }
    else   {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }

    //MainWindow w;
    DisplayWindow w;
    w.show();

    return a.exec();
}
