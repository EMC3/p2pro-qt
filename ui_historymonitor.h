/********************************************************************************
** Form generated from reading UI file 'historymonitor.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HISTORYMONITOR_H
#define UI_HISTORYMONITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qcp/qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_historyMonitor
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *gbHistory;
    QHBoxLayout *horizontalLayout_4;
    QCustomPlot *qcpHistory;
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *gbViewSettings;
    QVBoxLayout *verticalLayout;
    QCheckBox *cbMax;
    QCheckBox *cbMin;
    QCheckBox *cbMarker1;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    QPushButton *pbBack;

    void setupUi(QWidget *historyMonitor)
    {
        if (historyMonitor->objectName().isEmpty())
            historyMonitor->setObjectName(QString::fromUtf8("historyMonitor"));
        historyMonitor->resize(697, 756);
        verticalLayout_2 = new QVBoxLayout(historyMonitor);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        gbHistory = new QGroupBox(historyMonitor);
        gbHistory->setObjectName(QString::fromUtf8("gbHistory"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(gbHistory->sizePolicy().hasHeightForWidth());
        gbHistory->setSizePolicy(sizePolicy);
        horizontalLayout_4 = new QHBoxLayout(gbHistory);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        qcpHistory = new QCustomPlot(gbHistory);
        qcpHistory->setObjectName(QString::fromUtf8("qcpHistory"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(qcpHistory->sizePolicy().hasHeightForWidth());
        qcpHistory->setSizePolicy(sizePolicy1);

        horizontalLayout_4->addWidget(qcpHistory);


        verticalLayout_2->addWidget(gbHistory);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        gbViewSettings = new QGroupBox(historyMonitor);
        gbViewSettings->setObjectName(QString::fromUtf8("gbViewSettings"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(gbViewSettings->sizePolicy().hasHeightForWidth());
        gbViewSettings->setSizePolicy(sizePolicy2);
        verticalLayout = new QVBoxLayout(gbViewSettings);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        cbMax = new QCheckBox(gbViewSettings);
        cbMax->setObjectName(QString::fromUtf8("cbMax"));

        verticalLayout->addWidget(cbMax);

        cbMin = new QCheckBox(gbViewSettings);
        cbMin->setObjectName(QString::fromUtf8("cbMin"));

        verticalLayout->addWidget(cbMin);

        cbMarker1 = new QCheckBox(gbViewSettings);
        cbMarker1->setObjectName(QString::fromUtf8("cbMarker1"));

        verticalLayout->addWidget(cbMarker1);


        horizontalLayout_2->addWidget(gbViewSettings);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        pbBack = new QPushButton(historyMonitor);
        pbBack->setObjectName(QString::fromUtf8("pbBack"));

        horizontalLayout_3->addWidget(pbBack);


        verticalLayout_2->addLayout(horizontalLayout_3);


        retranslateUi(historyMonitor);

        QMetaObject::connectSlotsByName(historyMonitor);
    } // setupUi

    void retranslateUi(QWidget *historyMonitor)
    {
        historyMonitor->setWindowTitle(QApplication::translate("historyMonitor", "Form", nullptr));
        gbHistory->setTitle(QApplication::translate("historyMonitor", "History", nullptr));
        gbViewSettings->setTitle(QApplication::translate("historyMonitor", "View Settings:", nullptr));
        cbMax->setText(QApplication::translate("historyMonitor", "Max - Marker", nullptr));
        cbMin->setText(QApplication::translate("historyMonitor", "Min - Marker", nullptr));
        cbMarker1->setText(QApplication::translate("historyMonitor", "Maker 1", nullptr));
        pbBack->setText(QApplication::translate("historyMonitor", "Back", nullptr));
    } // retranslateUi

};

namespace Ui {
    class historyMonitor: public Ui_historyMonitor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HISTORYMONITOR_H
