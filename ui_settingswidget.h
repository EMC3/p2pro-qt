/********************************************************************************
** Form generated from reading UI file 'settingswidget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGSWIDGET_H
#define UI_SETTINGSWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SettingsWidget
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QCheckBox *flipV;
    QLabel *label;
    QCheckBox *gbUserMarkerVisible;
    QLabel *label_3;
    QCheckBox *enableCenterTemp;
    QCheckBox *colorInvert;
    QLabel *label_4;
    QCheckBox *flipH;
    QComboBox *colormap;
    QCheckBox *enableMinTemp;
    QCheckBox *enableMaxTemp;
    QLabel *label_2;
    QLineEdit *videoDev;
    QPushButton *delMarkers;
    QComboBox *cbRotation;
    QLabel *label_5;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *closeBtn;

    void setupUi(QWidget *SettingsWidget)
    {
        if (SettingsWidget->objectName().isEmpty())
            SettingsWidget->setObjectName(QString::fromUtf8("SettingsWidget"));
        SettingsWidget->resize(720, 485);
        verticalLayout = new QVBoxLayout(SettingsWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        flipV = new QCheckBox(SettingsWidget);
        flipV->setObjectName(QString::fromUtf8("flipV"));

        gridLayout->addWidget(flipV, 4, 1, 1, 1);

        label = new QLabel(SettingsWidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        gbUserMarkerVisible = new QCheckBox(SettingsWidget);
        gbUserMarkerVisible->setObjectName(QString::fromUtf8("gbUserMarkerVisible"));
        gbUserMarkerVisible->setChecked(true);

        gridLayout->addWidget(gbUserMarkerVisible, 10, 1, 1, 1);

        label_3 = new QLabel(SettingsWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 3, 0, 1, 1);

        enableCenterTemp = new QCheckBox(SettingsWidget);
        enableCenterTemp->setObjectName(QString::fromUtf8("enableCenterTemp"));
        enableCenterTemp->setChecked(true);

        gridLayout->addWidget(enableCenterTemp, 9, 1, 1, 1);

        colorInvert = new QCheckBox(SettingsWidget);
        colorInvert->setObjectName(QString::fromUtf8("colorInvert"));

        gridLayout->addWidget(colorInvert, 2, 1, 1, 1);

        label_4 = new QLabel(SettingsWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 7, 0, 1, 1);

        flipH = new QCheckBox(SettingsWidget);
        flipH->setObjectName(QString::fromUtf8("flipH"));

        gridLayout->addWidget(flipH, 3, 1, 1, 1);

        colormap = new QComboBox(SettingsWidget);
        colormap->setObjectName(QString::fromUtf8("colormap"));

        gridLayout->addWidget(colormap, 1, 1, 1, 1);

        enableMinTemp = new QCheckBox(SettingsWidget);
        enableMinTemp->setObjectName(QString::fromUtf8("enableMinTemp"));
        enableMinTemp->setChecked(true);

        gridLayout->addWidget(enableMinTemp, 7, 1, 1, 1);

        enableMaxTemp = new QCheckBox(SettingsWidget);
        enableMaxTemp->setObjectName(QString::fromUtf8("enableMaxTemp"));
        enableMaxTemp->setChecked(true);

        gridLayout->addWidget(enableMaxTemp, 8, 1, 1, 1);

        label_2 = new QLabel(SettingsWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        videoDev = new QLineEdit(SettingsWidget);
        videoDev->setObjectName(QString::fromUtf8("videoDev"));

        gridLayout->addWidget(videoDev, 0, 1, 1, 1);

        delMarkers = new QPushButton(SettingsWidget);
        delMarkers->setObjectName(QString::fromUtf8("delMarkers"));

        gridLayout->addWidget(delMarkers, 11, 1, 1, 1);

        cbRotation = new QComboBox(SettingsWidget);
        cbRotation->addItem(QString());
        cbRotation->addItem(QString());
        cbRotation->addItem(QString());
        cbRotation->addItem(QString());
        cbRotation->setObjectName(QString::fromUtf8("cbRotation"));

        gridLayout->addWidget(cbRotation, 5, 1, 1, 1);

        label_5 = new QLabel(SettingsWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 5, 0, 1, 1);


        verticalLayout->addLayout(gridLayout);

        verticalSpacer = new QSpacerItem(20, 162, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        closeBtn = new QPushButton(SettingsWidget);
        closeBtn->setObjectName(QString::fromUtf8("closeBtn"));

        horizontalLayout->addWidget(closeBtn);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(SettingsWidget);

        QMetaObject::connectSlotsByName(SettingsWidget);
    } // setupUi

    void retranslateUi(QWidget *SettingsWidget)
    {
        SettingsWidget->setWindowTitle(QApplication::translate("SettingsWidget", "Form", nullptr));
        flipV->setText(QApplication::translate("SettingsWidget", "FlipV [v]", nullptr));
        label->setText(QApplication::translate("SettingsWidget", "Video Device", nullptr));
        gbUserMarkerVisible->setText(QApplication::translate("SettingsWidget", "Show User Marker Names", nullptr));
        label_3->setText(QApplication::translate("SettingsWidget", "Image", nullptr));
        enableCenterTemp->setText(QApplication::translate("SettingsWidget", "Center", nullptr));
        colorInvert->setText(QApplication::translate("SettingsWidget", "Invert Colormap [i]", nullptr));
        label_4->setText(QApplication::translate("SettingsWidget", "Markers", nullptr));
        flipH->setText(QApplication::translate("SettingsWidget", "FlipH [h]", nullptr));
        enableMinTemp->setText(QApplication::translate("SettingsWidget", "Min Temp", nullptr));
        enableMaxTemp->setText(QApplication::translate("SettingsWidget", "Max Temp", nullptr));
        label_2->setText(QApplication::translate("SettingsWidget", "Color Map", nullptr));
        videoDev->setText(QApplication::translate("SettingsWidget", "/dev/video0", nullptr));
        delMarkers->setText(QApplication::translate("SettingsWidget", "Delete all user markers", nullptr));
        cbRotation->setItemText(0, QApplication::translate("SettingsWidget", "0 Degree", nullptr));
        cbRotation->setItemText(1, QApplication::translate("SettingsWidget", "90 Degree", nullptr));
        cbRotation->setItemText(2, QApplication::translate("SettingsWidget", "180 Degree", nullptr));
        cbRotation->setItemText(3, QApplication::translate("SettingsWidget", "270 Degree", nullptr));

        label_5->setText(QApplication::translate("SettingsWidget", "Rotation [r]", nullptr));
        closeBtn->setText(QApplication::translate("SettingsWidget", "Back", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SettingsWidget: public Ui_SettingsWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGSWIDGET_H
