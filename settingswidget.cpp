#include "settingswidget.h"
#include "ui_settingswidget.h"
#include "colormapmanager.h"
#include <QStandardPaths>
#include <filesystem>
#include "logger/log.h"
#include <QFile>
#include <QKeyEvent>

SettingsWidget::SettingsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SettingsWidget)
{
    ui->setupUi(this);

    auto cmm = ColorMapManager::get();
    for(const auto & entry : cmm->cmaps){
        ui->colormap->addItem(QString::fromStdString(entry.first) + " [" + QString::number(cmm->getIndex(entry.first)) + "]");
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

    auto cmm = ColorMapManager::get();
    QJsonObject root = doc.object();

    std::string cm = root.value("cm_name").toString().toStdString();
    ui->colormap->setCurrentIndex(cmm->getIndex(cm)-1);
    //ui->colormap->setCurrentText();

    ui->colorInvert->setChecked(root.value("cm_invert").toBool());
    ui->videoDev->setText(root.value("videodev").toString());
    ui->flipH->setChecked(root.value("im_fliph").toBool());
    ui->flipV->setChecked(root.value("im_flipv").toBool());
    ui->enableMinTemp->setChecked(root.value("mkr_min").toBool(true));
    ui->enableMaxTemp->setChecked(root.value("mkr_max").toBool(true));
    ui->enableCenterTemp->setChecked(root.value("mkr_center").toBool(true));
    ui->gbUserMarkerVisible->setChecked(root.value("mkr_usrshow").toBool(true));

    int curRotIndex = root.value("cm_rotate").toInt(0);
    if((curRotIndex >= 0) && (curRotIndex < 4)){
        ui->cbRotation->setCurrentIndex(curRotIndex);
    }
    else{
        ui->cbRotation->setCurrentIndex(0);
    }

}

SettingsWidget::~SettingsWidget()
{
    delete ui;
}

void SettingsWidget::changeColormap(int cmIdx)
{
    if(cmIdx <= ui->colormap->count()){
        ui->colormap->setCurrentIndex(cmIdx-1);
    }
    on_closeBtn_clicked();
}

void SettingsWidget::flipHKeyBtn()
{
    ui->flipH->setChecked(!ui->flipH->isChecked());
    on_closeBtn_clicked();
}

void SettingsWidget::flipVKeyBtn()
{
    ui->flipV->setChecked(!ui->flipV->isChecked());
    on_closeBtn_clicked();
}

void SettingsWidget::toggleCrosshairBtn()
{
    ui->enableCenterTemp->setChecked(!ui->enableCenterTemp->isChecked());
    on_closeBtn_clicked();
}

void SettingsWidget::rotateKeyBtn()
{
    int idx = ui->cbRotation->currentIndex();
    (idx < 3) ? idx++ : idx = 0;
    ui->cbRotation->setCurrentIndex(idx);
    on_closeBtn_clicked();
}

void SettingsWidget::invertColorBtn(){
    ui->colorInvert->setChecked(!ui->colorInvert->isChecked());
    on_closeBtn_clicked();
}

void SettingsWidget::keyPressEvent(QKeyEvent *event){
    if (event->key() == Qt::Key_Escape){
        on_closeBtn_clicked();
    }
}

void SettingsWidget::applySettings()
{
    QString cmStoreText = ui->colormap->currentText().section(" [",0,0);
    selectedColormap = cmStoreText.toStdString();
    invertColormap = ui->colorInvert->isChecked();
    selectedVideoDevice = ui->videoDev->text();
    flipH = ui->flipH->isChecked();
    flipV = ui->flipV->isChecked();
    enableMinMkr = ui->enableMinTemp->isChecked();
    enableMaxMkr = ui->enableMaxTemp->isChecked();
    enableCenterMkr = ui->enableCenterTemp->isChecked();
    showUsrMkr = ui->gbUserMarkerVisible->isChecked();
    rotation = ui->cbRotation->currentIndex();
}

void SettingsWidget::on_closeBtn_clicked(){
    applySettings();

    QJsonObject root;
    root.insert("cm_name", QString::fromStdString(selectedColormap));
    root.insert("cm_invert", invertColormap);
    root.insert("cm_rotate", rotation);
    root.insert("videodev", selectedVideoDevice);
    root.insert("im_fliph", flipH);
    root.insert("im_flipv", flipV);
    root.insert("mkr_min", enableMinMkr);
    root.insert("mkr_max", enableMaxMkr);
    root.insert("mkr_center", enableCenterMkr);
    root.insert("mkr_usrshow",showUsrMkr);

    QString pth = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    std::filesystem::create_directories(pth.toStdString());

    QString filepath = pth+"/conf.json";
    QFile configFile(filepath);
    configFile.open(QFile::WriteOnly);

    LOG << "Settings stored to: " << filepath;

    QJsonDocument doc(root);
    configFile.write(doc.toJson());
    configFile.close();

    emit back();
}


void SettingsWidget::on_delMarkers_clicked()
{
    emit delMarkers();
}

