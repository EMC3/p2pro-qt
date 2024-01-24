/********************************************************************************
** Form generated from reading UI file 'displaywindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DISPLAYWINDOW_H
#define UI_DISPLAYWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "thermalplot.h"

QT_BEGIN_NAMESPACE

class Ui_DisplayWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_2;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QHBoxLayout *horizontalLayout;
    ThermalPlot *plot;
    QVBoxLayout *verticalLayout;
    QToolButton *playPause;
    QToolButton *cmapLock;
    QToolButton *savePhoto;
    QToolButton *settings;
    QToolButton *tbHistory;

    void setupUi(QMainWindow *DisplayWindow)
    {
        if (DisplayWindow->objectName().isEmpty())
            DisplayWindow->setObjectName(QString::fromUtf8("DisplayWindow"));
        DisplayWindow->resize(1327, 932);
        centralwidget = new QWidget(DisplayWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        horizontalLayout = new QHBoxLayout(page);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, -1, 0);
        plot = new ThermalPlot(page);
        plot->setObjectName(QString::fromUtf8("plot"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(plot->sizePolicy().hasHeightForWidth());
        plot->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(plot);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        playPause = new QToolButton(page);
        playPause->setObjectName(QString::fromUtf8("playPause"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/darkicons/play.svg"), QSize(), QIcon::Normal, QIcon::Off);
        playPause->setIcon(icon);
        playPause->setIconSize(QSize(64, 64));
        playPause->setAutoRaise(true);

        verticalLayout->addWidget(playPause);

        cmapLock = new QToolButton(page);
        cmapLock->setObjectName(QString::fromUtf8("cmapLock"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/darkicons/unlocked.svg"), QSize(), QIcon::Normal, QIcon::Off);
        cmapLock->setIcon(icon1);
        cmapLock->setIconSize(QSize(64, 64));
        cmapLock->setAutoRaise(true);

        verticalLayout->addWidget(cmapLock);

        savePhoto = new QToolButton(page);
        savePhoto->setObjectName(QString::fromUtf8("savePhoto"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/darkicons/redcircle.svg"), QSize(), QIcon::Normal, QIcon::Off);
        savePhoto->setIcon(icon2);
        savePhoto->setIconSize(QSize(64, 64));
        savePhoto->setAutoRaise(true);

        verticalLayout->addWidget(savePhoto);

        settings = new QToolButton(page);
        settings->setObjectName(QString::fromUtf8("settings"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/darkicons/gear.svg"), QSize(), QIcon::Normal, QIcon::Off);
        settings->setIcon(icon3);
        settings->setIconSize(QSize(64, 64));
        settings->setAutoRaise(true);

        verticalLayout->addWidget(settings);

        tbHistory = new QToolButton(page);
        tbHistory->setObjectName(QString::fromUtf8("tbHistory"));
        QIcon icon4(QIcon::fromTheme(QString::fromUtf8("utilities-system-monitor")));
        tbHistory->setIcon(icon4);
        tbHistory->setIconSize(QSize(64, 64));

        verticalLayout->addWidget(tbHistory);


        horizontalLayout->addLayout(verticalLayout);

        stackedWidget->addWidget(page);

        verticalLayout_2->addWidget(stackedWidget);

        DisplayWindow->setCentralWidget(centralwidget);

        retranslateUi(DisplayWindow);

        QMetaObject::connectSlotsByName(DisplayWindow);
    } // setupUi

    void retranslateUi(QMainWindow *DisplayWindow)
    {
        DisplayWindow->setWindowTitle(QApplication::translate("DisplayWindow", "P2Pro Qt", nullptr));
        playPause->setText(QApplication::translate("DisplayWindow", "...", nullptr));
        cmapLock->setText(QApplication::translate("DisplayWindow", "...", nullptr));
        savePhoto->setText(QApplication::translate("DisplayWindow", "...", nullptr));
        settings->setText(QApplication::translate("DisplayWindow", "...", nullptr));
        tbHistory->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class DisplayWindow: public Ui_DisplayWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DISPLAYWINDOW_H
