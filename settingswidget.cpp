#include "settingswidget.h"
#include "ui_settingswidget.h"
#include "colormapmanager.h"
#include <QStandardPaths>
#include <filesystem>
#include "logger/log.h"
#include <QFile>

SettingsWidget::SettingsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SettingsWidget)
{
    ui->setupUi(this);

    auto cmm = ColorMapManager::get();
    for(const auto & entry : cmm->cmaps){
        ui->colormap->addItem(QString::fromStdString(entry.first));
    }

    loadConfig();

    applySettings();
}

void SettingsWidget::loadConfig(){
    QString pth = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    QFile configFile(pth+"/conf.json");
    if(!configFile.exists()){
        WARN << "Config file does not exist. abort load.";
        return;
    }

    if(!configFile.open(QFile::ReadOnly)){
        ERR << "Config file could not be opened. abort load.";
        return;
    }

    auto contents = configFile.readAll();
    configFile.close();


    QJsonParseError err;
    auto doc = QJsonDocument::fromJson(contents, &err);
    if(err.error != QJsonParseError::NoError){
        ERR << "Failed to read config file";
        return;
    }

    QJsonObject root = doc.object();
    ui->colormap->setCurrentText(root.value("cm_name").toString());
    ui->colorInvert->setChecked(root.value("cm_invert").toBool());
    ui->videoDev->setText(root.value("videodev").toString());
    ui->flipH->setChecked(root.value("im_fliph").toBool());
    ui->flipV->setChecked(root.value("im_flipv").toBool());
    ui->enableMinTemp->setChecked(root.value("mkr_min").toBool(true));
    ui->enableMaxTemp->setChecked(root.value("mkr_max").toBool(true));
    ui->enableCenterTemp->setChecked(root.value("mkr_center").toBool(true));
}

SettingsWidget::~SettingsWidget()
{
    delete ui;
}

void SettingsWidget::applySettings(){
    selectedColormap = ui->colormap->currentText().toStdString();
    invertColormap = ui->colorInvert->isChecked();
    selectedVideoDevice = ui->videoDev->text();
    flipH = ui->flipH->isChecked();
    flipV = ui->flipV->isChecked();
    enableMinMkr = ui->enableMinTemp->isChecked();
    enableMaxMkr = ui->enableMaxTemp->isChecked();
    enableCenterMkr = ui->enableCenterTemp->isChecked();
}

void SettingsWidget::on_closeBtn_clicked()
{
    applySettings();

    QJsonObject root;
    root.insert("cm_name", QString::fromStdString(selectedColormap));
    root.insert("cm_invert", invertColormap);
    root.insert("videodev", selectedVideoDevice);
    root.insert("im_fliph", flipH);
    root.insert("im_flipv", flipV);
    root.insert("mkr_min", enableMinMkr);
    root.insert("mkr_max", enableMaxMkr);
    root.insert("mkr_center", enableCenterMkr);

    QString pth = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    std::filesystem::create_directories(pth.toStdString());

    QFile configFile(pth+"/conf.json");
    configFile.open(QFile::WriteOnly);

    QJsonDocument doc(root);
    configFile.write(doc.toJson());
    configFile.close();

    emit back();
}


void SettingsWidget::on_delMarkers_clicked()
{
    emit delMarkers();
}

