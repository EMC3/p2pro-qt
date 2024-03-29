#include "displaywindow.h"
#include "colormapmanager.h"
#include "ui_displaywindow.h"
#include "marker.h"
#include <logger/log.h>
#include "settingswidget.h"
#include <filesystem>
#include <regex>

DisplayWindow::DisplayWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DisplayWindow)
{
    videoDev = "/dev/video0";
    ui->setupUi(this);

    savePath = (QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)+"/p2pro-qt").toStdString();
    std::filesystem::create_directories(savePath);

    findSaveIndex();

    tc = new ThermalCamera(this);
    connect(tc, &ThermalCamera::newThermalImage, this, &DisplayWindow::displayImage);
    connect(tc, &ThermalCamera::statusUpdate, this, &DisplayWindow::tcStatusUpdate);

    updateCmap("Hotiron", false);

    // Import Test Image
    img = std::make_shared<ThermalImage>(256, 192);
    QFile testFile(":/misc/testdata.bin");
    testFile.open(QFile::ReadOnly);
    QByteArray d = testFile.readAll();
    testFile.close();
    if(d.size() == 256*192*sizeof(float)){
        memcpy(img->data, d.constData(), 256*192*sizeof(float));
    }else{
        memset(img->data, 0, 256*192*sizeof(float));
    }

    img->computeMetrics();
    ui->plot->setImage(img);

    sw = new SettingsWidget(this);
    ui->stackedWidget->addWidget(sw);

    /* S/S from Settings */
    connect(sw, &SettingsWidget::back, this, &DisplayWindow::applySettings);
    connect(sw, &SettingsWidget::delMarkers, ui->plot, &ThermalPlot::deleteUserMarkers);

    /* Hotkey Changes to Settings */
    connect(this, &DisplayWindow::colormapChanged, sw, &SettingsWidget::changeColormap);
    connect(this, &DisplayWindow::flippedH, sw, &SettingsWidget::flipHKeyBtn);
    connect(this, &DisplayWindow::flippedV, sw, &SettingsWidget::flipVKeyBtn);
    connect(this, &DisplayWindow::colorInverted, sw, &SettingsWidget::invertColorBtn);
    connect(this, &DisplayWindow::toggleCrosshair, sw, &SettingsWidget::toggleCrosshairBtn);
    connect(this, &DisplayWindow::rotate, sw, &SettingsWidget::rotateKeyBtn);

    /* History Monitor */
    hm = new historyMonitor(this);
    ui->stackedWidget->addWidget(hm);
    connect(hm, &historyMonitor::back, this, &DisplayWindow::showCam);

    applySettings();
}

void DisplayWindow::findSaveIndex(){
    saveIndex = 0;
    std::regex pattern("^IMG_(\\d{5})");

    std::filesystem::directory_iterator end;
    for(std::filesystem::directory_iterator it(savePath); it != end; it++){
        if(!it->is_regular_file())continue;
        auto fp = it->path();
        std::string fn = fp.filename();

        std::smatch matches;
        if (std::regex_search(fn, matches, pattern)) {

            std::string numberStr = matches[1].str();
            int number = std::stoi(numberStr);
            saveIndex = std::max(saveIndex, number);
        }
    }
}

void DisplayWindow::showCam()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void DisplayWindow::applySettings(){
    /* Move Display Window to the front */
    ui->stackedWidget->setCurrentIndex(0);

    /* Inversion CM*/
    updateCmap(sw->selectedColormap, sw->invertColormap);

    /* Video Device Change */
    videoDev = sw->selectedVideoDevice;

    /* Flip Image */
    ui->plot->setFlipSettings(sw->flipH, sw->flipV);

    /* Rotate Image */
    tc->rotateCam(sw->rotation);

    /* set Marker Settings */
    setMarkerSettings(sw->enableMinMkr, sw->enableMaxMkr, sw->enableCenterMkr, sw->showUsrMkr);
}

DisplayWindow::~DisplayWindow()
{
    delete ui;
}

void DisplayWindow::keyPressEvent(QKeyEvent *ev)
{
    //LOG << "You Pressed Key " << ev->text();
    /* check cm changes */
    for(int cnt = 1; cnt < 10; cnt++){
        if (ev->text().compare(QString::number(cnt)) == 0){
            LOG << "Changing colormap: " << ev->text();
            emit colormapChanged(cnt);
            return;
        }
    }

    if (ev->text().compare(QString("h")) == 0){
    LOG << "Flip horizontal";
    emit flippedH();
    return;
    }

    if (ev->text().compare(QString("v")) == 0){
        LOG << "Flip vertical";
        emit flippedV();
        return;
    }

    if (ev->text().compare(QString("i")) == 0){
        LOG << "Color inverted";
        emit colorInverted();
        return;
    }

    if (ev->key() == Qt::Key_C){
        LOG << "Crosshair toggled.";
        emit toggleCrosshair();
        return;
    }

    if (ev->key() == Qt::Key_R){
        LOG << "Rotated";
        emit rotate();
        return;
    }

    if (ev->key() == Qt::Key_L){
        LOG << "Locked";
        on_cmapLock_clicked();
        return;
    }

    if (ev->key() == Qt::Key_P){
        on_playPause_clicked();
        return;
    }
}

void DisplayWindow::on_cmapLock_clicked()
{
    ui->plot->autoColorRange = !ui->plot->autoColorRange;
    if(ui->plot->autoColorRange){
        ui->plot->reRange();
        ui->cmapLock->setIcon(QIcon(":/darkicons/unlocked.svg"));
    }else{
        ui->cmapLock->setIcon(QIcon(":/darkicons/locked.svg"));
    }
}

#include <tiffio.h>
void DisplayWindow::on_savePhoto_clicked()
{
    char sf[16];
    sprintf(sf,"IMG_%05d",saveIndex);
    std::string rp = savePath+"/"+sf+".tiff";
    std::string pp = savePath+"/"+sf+".png";
    saveIndex++;

    LOG << "Raw data=>\""<<rp<<"\" Screenshot=>\""<<pp<<"\"";


    TIFF *image = TIFFOpen(rp.c_str(), "w");

    TIFFSetField(image, TIFFTAG_IMAGEWIDTH, img->width);
    TIFFSetField(image, TIFFTAG_IMAGELENGTH, img->height);
    TIFFSetField(image, TIFFTAG_SAMPLESPERPIXEL, 1); // Grayscale
    TIFFSetField(image, TIFFTAG_BITSPERSAMPLE, 32);  // 32-bit float
    TIFFSetField(image, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
    TIFFSetField(image, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(image, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
    TIFFSetField(image, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_IEEEFP);
    TIFFSetField(image, TIFFTAG_COMPRESSION, COMPRESSION_LZMA);

    for (int y = 0; y < img->height; y++) {
        TIFFWriteScanline(image, img->data + img->width*y, y, 0);
    }

    TIFFClose(image);

    auto screenShot = ui->plot->grab();
    screenShot.save(QString::fromStdString(pp));
}


void DisplayWindow::on_settings_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void DisplayWindow::updateCmap(std::string name, bool inv){

    auto & map = ColorMapManager::get()->getColorMap(name);
    ui->plot->setColorMap(map, inv);
}



void DisplayWindow::displayImage()
{
    img = tc->lastImage;
    ui->plot->setImage(img);
}


void DisplayWindow::setMarkerSettings(bool showMax, bool showMin, bool showMid, bool showUsrName)
{
    ui->plot->maxMarker->setHidden(!showMax);
    ui->plot->minMarker->setHidden(!showMin);
    ui->plot->centerMarker->setHidden(!showMid);
    ui->plot->setUserMarkerNameVisible(showUsrName);
    ui->plot->autoReplot();
}

void DisplayWindow::on_playPause_clicked()
{
    if(tc->isStarted() && !tc->hasError()){
        tc->stop();
        ui->playPause->setIcon(QIcon(":/darkicons/play.svg"));
    }else{
        tc->start(videoDev,ThermalCamera::ROTATION(sw->rotation));
        ui->playPause->setIcon(QIcon(":/darkicons/stop.svg"));
    }

}

void DisplayWindow::tcStatusUpdate(QString statusStr)
{
    if(tc->hasError()){
        ui->playPause->setIcon(QIcon(":/darkicons/play.svg"));
        tc->stop();
        QMessageBox::critical(this, "Error","Failed to access camera: "+statusStr);
    }
}

void DisplayWindow::on_tbHistory_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}
