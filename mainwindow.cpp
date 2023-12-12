#include "mainwindow.h"
//#include "logger/log.h"
#include "ui_mainwindow.h"

#include <fstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    upperLock = lowerLock = false;

    ui->colormap->blockSignals(true);
    cmaps.emplace_back(ColorMap::HOTIRON); ui->colormap->addItem("Hotiron");
    cmaps.emplace_back("../jet.csv"); ui->colormap->addItem("Jet");
    cmaps.emplace_back("../gray.csv"); ui->colormap->addItem("Gray");
    cmaps.emplace_back(ColorMap::PLASMA); ui->colormap->addItem("Plasma");
    cmaps.emplace_back(ColorMap::INFERNO); ui->colormap->addItem("Inferno");
    cmaps.emplace_back(ColorMap::MAGMA); ui->colormap->addItem("Magma");
    cmaps.emplace_back(ColorMap::VIRIDIS); ui->colormap->addItem("Viridis");
    cmaps.emplace_back(ColorMap::PARULA); ui->colormap->addItem("Parula");
    ui->colormap->setCurrentIndex(0);
    ui->colormap->blockSignals(false);


    auto customPlot = ui->plot;

    customPlot->axisRect()->setupFullAxesBox(true);

    customPlot->yAxis->setTickLabels(false);
    customPlot->yAxis->setTicks(false);
    customPlot->xAxis->setTickLabels(false);
    customPlot->xAxis->setTicks(false);
    customPlot->yAxis2->setTickLabels(false);
    customPlot->yAxis2->setTicks(false);
    customPlot->xAxis2->setTickLabels(false);
    customPlot->xAxis2->setTicks(false);

    // set up the QCPColorMap:
    colorMap = new QCPColorMap(customPlot->xAxis, customPlot->yAxis);
    /*int nx = 200;
    int ny = 200;
    colorMap->data()->setSize(nx, ny); // we want the color map to have nx * ny data points
    colorMap->data()->setRange(QCPRange(-4, 4), QCPRange(-4, 4)); // and span the coordinate range -4..4 in both key (x) and value (y) dimensions
    // now we assign some data, by accessing the QCPColorMapData instance of the color map:
    double x, y, z;
    for (int xIndex=0; xIndex<nx; ++xIndex)
    {
        for (int yIndex=0; yIndex<ny; ++yIndex)
        {
            colorMap->data()->cellToCoord(xIndex, yIndex, &x, &y);
            double r = 3*qSqrt(x*x+y*y)+1e-2;
            z = 2*x*(qCos(r+2)/r-qSin(r+2)/r); // the B field strength of dipole radiation (modulo physical constants)
            colorMap->data()->setCell(xIndex, yIndex, z);
        }
    }*/

    // add a color scale:
    QCPColorScale *colorScale = new QCPColorScale(customPlot);
    customPlot->plotLayout()->addElement(0, 1, colorScale); // add it to the right of the main axis rect
    colorScale->setType(QCPAxis::atRight); // scale shall be vertical bar with tick/axis labels right (actually atRight is already the default)
    colorMap->setColorScale(colorScale); // associate the color map with the color scale

    updateCmap();

    colorMap->rescaleDataRange();

    // make sure the axis rect and color scale synchronize their bottom and top margins (so they line up):
    QCPMarginGroup *marginGroup = new QCPMarginGroup(customPlot);
    customPlot->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
    colorScale->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);

    // rescale the key (x) and value (y) axes so the whole color map is visible:
    customPlot->rescaleAxes();

    tc = new ThermalCamera(this);
    connect(tc, &ThermalCamera::newThermalImage, this, &MainWindow::displayImage);

    connect(customPlot, &QCustomPlot::mousePress, this, &MainWindow::plotClick);
    connect(customPlot, &QCustomPlot::mouseMove, this, &MainWindow::plotHover);

    maxMarker = new Marker(colorMap, Qt::red, Qt::red);
    minMarker = new Marker(colorMap, Qt::cyan, Qt::cyan);


    // Import Test Image
    img = std::make_shared<ThermalImage>(256, 192);
    double tmp[256*192];
    std::ifstream ifs("/f/TMP/thermal/img00384.bin");
    ifs.read((char*)tmp, img->width*img->height*sizeof(double));
    for(int i = 0; i < 256*192; i++)img->data[i] = tmp[i];
    img->computeMetrics();
    plotImg();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateCmap(){
    QCPColorGradient grad;
    auto & map = cmaps[ui->colormap->currentIndex()];
    map.setBounds(0, 1);
    if(ui->colorInvert->isChecked()){
        for(int i = 0; i <= 100; i++){
            grad.setColorStopAt(double(i)/100.0, QColor(map.getARGB(1.0 - double(i)/100.0)));
        }
    }else{
        for(int i = 0; i <= 100; i++){
            QColor col(map.getARGB(double(i)/100.0));
            grad.setColorStopAt(double(i)/100.0, col);
        }
    }

    colorMap->setGradient(grad);

    reRange();
}

void MainWindow::plotImg()
{
    bool flipH = ui->flipH->isChecked();
    bool flipV = ui->flipV->isChecked();

    ui->plot->xAxis->setRangeReversed(flipH);
    ui->plot->yAxis->setRangeReversed(flipV);

    colorMap->data()->setSize(img->width, img->height);
    colorMap->data()->setRange(QCPRange(0, img->width), QCPRange(0, img->height));
    ui->plot->rescaleAxes();

    for (int yIndex=0; yIndex<img->height; ++yIndex){
        //int sy = flipV ? (img->height - yIndex - 1) : yIndex;
        int sy = yIndex;
        /*if(flipH){
            for (int xIndex=0; xIndex<img->width; ++xIndex) colorMap->data()->setCell(xIndex, yIndex, img->getValue(img->width - xIndex - 1, sy));
        }else{*/
            for (int xIndex=0; xIndex<img->width; ++xIndex) colorMap->data()->setCell(xIndex, yIndex, img->getValue(xIndex, sy));
        //}
    }

    maxMarker->setPosition(img->maxX, img->maxY);
    maxMarker->updateText(img);

    minMarker->setPosition(img->minX, img->minY);
    minMarker->updateText(img);

    updateUserMarkers();

    reRange();
}

void MainWindow::displayImage()
{
    img = tc->lastImage;

    plotImg();
}

void MainWindow::displayStatus(QString s)
{
    ui->statusBar->showMessage(s);
}


void MainWindow::on_colormap_currentIndexChanged(int index)
{
    updateCmap();
}


void MainWindow::on_colorInvert_toggled(bool checked)
{
    updateCmap();
}


void MainWindow::on_btnStart_clicked()
{
    tc->start();
}


void MainWindow::on_btnStop_clicked()
{
    tc->stop();
}


void MainWindow::on_lockb_clicked()
{
    ui->lockLower->setChecked(true);
    ui->lockUpper->setChecked(true);
    lowerLock = upperLock = true;
    reRange();
}


void MainWindow::on_lockLower_toggled(bool checked)
{
    lowerLock = checked;
    reRange();
}


void MainWindow::on_lockUpper_toggled(bool checked)
{
    upperLock = checked;
    reRange();
}


void MainWindow::on_unlockb_clicked()
{
    ui->lockLower->setChecked(false);
    ui->lockUpper->setChecked(false);
    lowerLock = upperLock = false;
    reRange();
}

void MainWindow::plotClick(QMouseEvent *event)
{
    double key,val;
    colorMap->pixelsToCoords(QPointF(event->pos()),key,val);
    int rx, ry;
    colorMap->data()->coordToCell(key,val,&rx,&ry);

    /*if(!img)return;
    if(rx < 0 || rx >= img->width)return;
    if(ry < 0 || ry >= img->height)return;*/

    bool flag = false;

    userMarkers.remove_if([&](std::shared_ptr<Marker> & mkr){
        int dx = mkr->dataX - rx;
        int dy = mkr->dataY - ry;
        if(std::sqrt(dx*dx + dy*dy) < 15){
            flag = true;
            return true;
        }
        return false;
    });

    if(flag){
        ui->plot->replot(QCustomPlot::rpQueuedReplot);
        return;
    }

    auto mkr = std::make_shared<Marker>(colorMap, Qt::green, Qt::white);
    userMarkers.push_back(mkr);
    mkr->setPosition(rx, ry);
    updateUserMarkers();

    ui->plot->replot(QCustomPlot::rpQueuedReplot);
}

void MainWindow::plotHover(QMouseEvent *event)
{
    double key,val;
    colorMap->pixelsToCoords(QPointF(event->pos()),key,val);
    int rx, ry;
    colorMap->data()->coordToCell(key,val,&rx,&ry);

    if(!img || rx < 0 || rx >= img->width || ry < 0 || ry >= img->height){
        return;
    }

    ui->statusBar->showMessage("Temperature @ ("+QString::number(rx)+", "+QString::number(ry)+") is "+QString::number(img->getValue(rx, ry),'f',1)+" °C");
}




void MainWindow::reRange()
{
    if(!img)return;

    double min = lowerLock ? ui->lowerBound->value() : img->min;
    double max = upperLock ? ui->upperBound->value() : img->max;

    if(!lowerLock)ui->lowerBound->setValue(min);
    if(!upperLock)ui->upperBound->setValue(max);

    colorMap->setDataRange(QCPRange(min, max));
    ui->plot->replot(QCustomPlot::rpQueuedReplot);
}


void MainWindow::on_upperBound_valueChanged(double arg1)
{
    if(upperLock)reRange();
}


void MainWindow::on_lowerBound_valueChanged(double arg1)
{
    if(lowerLock)reRange();
}

void MainWindow::updateUserMarkers()
{
    for(auto & mkr : userMarkers){
        mkr->updateText(img);
    }
}


Marker::Marker(QCPColorMap *colorMap, QColor markerColor, QColor textColor){
    hidden = false;
    this->colorMap = colorMap;
    auto plot = colorMap->parentPlot();
    tracer = new QCPItemTracer(plot);
    tracer->position->setType(QCPItemPosition::ptPlotCoords);
    tracer->setInterpolating(true);
    tracer->setStyle(QCPItemTracer::tsPlus);
    QPen thickPen(markerColor);
    thickPen.setWidth(2);
    tracer->setPen(thickPen);
    tracer->setBrush(markerColor);
    tracer->setSize(12);


    label = new QCPItemText(plot);
    label->position->setParentAnchor(tracer->position);
    label->setPositionAlignment(Qt::AlignTop | Qt::AlignLeft);
    label->setColor(Qt::white);
    label->setBrush(QBrush(QColor(0,0,0,100)));
    label->setText("XXX");
}

Marker::~Marker()
{
    delete tracer;
    delete label;
}

void Marker::setPosition(int dataX, int dataY){
    double key, val;
    colorMap->data()->cellToCoord(dataX,dataY,&key,&val);
    tracer->position->setCoords(key,val);
    this->dataX = dataX;
    this->dataY = dataY;
}

void Marker::updateText(const std::shared_ptr<ThermalImage> &img){
    float temp = img->getValue(dataX, dataY);
    label->setText(QString::number(temp, 'f', 1));
}

void Marker::setHidden(bool hide)
{
    tracer->setVisible(!hide);
    label->setVisible(!hide);
    hidden = hide;
}

void MainWindow::on_delMkr_clicked()
{
    userMarkers.clear();
    ui->plot->replot(QCustomPlot::rpQueuedReplot);
}


void MainWindow::on_showMax_toggled(bool checked)
{
    maxMarker->setHidden(!checked);
    ui->plot->replot(QCustomPlot::rpQueuedReplot);
}


void MainWindow::on_showMin_toggled(bool checked)
{
    minMarker->setHidden(!checked);
    ui->plot->replot(QCustomPlot::rpQueuedReplot);
}

