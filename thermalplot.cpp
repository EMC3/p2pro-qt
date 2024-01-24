#include "thermalplot.h"
#include "marker.h"
#include "colormap/colormap.h"
#include "thermalcamera.h"

#include <QInputDialog>

ThermalPlot::ThermalPlot(QWidget *parent)
    : QCustomPlot{parent}
{
    resetAutoRange();

    autoColorRange = true;
    ringIdx = 0;

    axisRect()->setupFullAxesBox(true);

    /*customPlot->xAxis->setBasePen(QPen(Qt::white, 1));
    customPlot->yAxis->setBasePen(QPen(Qt::white, 1));
    customPlot->xAxis->setTickPen(QPen(Qt::white, 1));
    customPlot->yAxis->setTickPen(QPen(Qt::white, 1));
    customPlot->xAxis->setSubTickPen(QPen(Qt::white, 1));
    customPlot->yAxis->setSubTickPen(QPen(Qt::white, 1));
    customPlot->xAxis->setTickLabelColor(Qt::white);
    customPlot->yAxis->setTickLabelColor(Qt::white);
    customPlot->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    customPlot->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    customPlot->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));*/
    //xAxis->grid()->setSubGridVisible(true);
    //yAxis->grid()->setSubGridVisible(true);
    //customPlot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    //customPlot->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    //customPlot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    //customPlot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);




    yAxis->setTickLabels(false);
    yAxis->setTicks(false);
    xAxis->setTickLabels(false);
    xAxis->setTicks(false);
    yAxis2->setTickLabels(false);
    yAxis2->setTicks(false);
    xAxis2->setTickLabels(false);
    xAxis2->setTicks(false);

    axisRect()->setAutoMargins(QCP::msNone);
    axisRect()->setMargins(QMargins(0,0,0,6));
    // set up the QCPColorMap:
    colorMap = new QCPColorMap(xAxis, yAxis);

    // add a color scale:
    QCPColorScale *colorScale = new QCPColorScale(this);
    plotLayout()->addElement(0, 1, colorScale); // add it to the right of the main axis rect
    colorScale->setType(QCPAxis::atRight); // scale shall be vertical bar with tick/axis labels right (actually atRight is already the default)
    colorMap->setColorScale(colorScale); // associate the color map with the color scale

    setBackground(QBrush(QColor(0x19, 0x23, 0x2D)));
    axisRect()->setBackground(QBrush(QColor(0x19, 0x23, 0x2D)));
    colorScale->axis()->setTickLabelColor(Qt::white);
    //colorScale->axis()->setBasePen(QPen(Qt::white, 1));
    //colorScale->axis()->setTickPen(QPen(Qt::white, 1));
    //colorScale->axis()->setSubTickPen(QPen(Qt::white, 1));
    //colorScale->axis()->grid()->setPen(QPen(Qt::white, 1));


    //updateCmap(0, false);

    colorMap->rescaleDataRange();

    // make sure the axis rect and color scale synchronize their bottom and top margins (so they line up):
    QCPMarginGroup *marginGroup = new QCPMarginGroup(this);
    axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
    colorScale->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);

    // rescale the key (x) and value (y) axes so the whole color map is visible:
    this->rescaleAxes();

    connect(this, &QCustomPlot::mousePress, this, &ThermalPlot::plotClick);

    maxMarker = new Marker(colorMap, Qt::red, Qt::red);
    minMarker = new Marker(colorMap, Qt::cyan, Qt::cyan);
}

void ThermalPlot::setFlipSettings(bool flipH, bool flipV){
    xAxis->setRangeReversed(flipH);
    yAxis->setRangeReversed(flipV);
    replot(QCustomPlot::rpQueuedReplot);
}

void ThermalPlot::setImage(std::shared_ptr<ThermalImage> &image){
    img = image;
    plotImg();
}

void ThermalPlot::setUserMarkerNameVisible(bool visibility){
    showUsrMkr = visibility;
    for(const auto& mkr : userMarkers){
        mkr->setNameVislble(showUsrMkr);
    }
}


void ThermalPlot::reRange()
{
    if(!img)return;

    float autoMin = minRing[0];
    float autoMax = maxRing[0];
    for(int i = 1; i < AUTORANGE_RING_SIZE; i++){
        autoMax = std::max(maxRing[i], autoMax);
        autoMin = std::min(minRing[i], autoMin);
    }

    double min = autoColorRange ? autoMin : minCRange;
    double max = autoColorRange ? autoMax : maxCRange;

    colorMap->setDataRange(QCPRange(min, max));
    replot(QCustomPlot::rpQueuedReplot);

    minCRange = min;
    maxCRange = max;
}

void ThermalPlot::deleteUserMarkers()
{
    userMarkers.clear();
    replot(QCustomPlot::rpQueuedReplot);
}

void ThermalPlot::resetAutoRange(){
    for(int i = 0; i < AUTORANGE_RING_SIZE; i++){
        minRing[i] = +999999.0f;
        maxRing[i] = -999999.0f;
    }
}

void ThermalPlot::setColorMap(ColorMap &cmap, bool inv){
    QCPColorGradient grad;
    cmap.setBounds(0, 1);
    if(inv){
        for(int i = 0; i <= 100; i++){
            grad.setColorStopAt(double(i)/100.0, QColor(cmap.getARGB(1.0 - double(i)/100.0)));
        }
    }else{
        for(int i = 0; i <= 100; i++){
            QColor col(cmap.getARGB(double(i)/100.0));
            grad.setColorStopAt(double(i)/100.0, col);
        }
    }

    colorMap->setGradient(grad);

    reRange();
}

void ThermalPlot::autoReplot(){
    replot(QCustomPlot::rpQueuedReplot);
}

void ThermalPlot::plotImg()
{
    colorMap->data()->setSize(img->width, img->height);
    colorMap->data()->setRange(QCPRange(0, img->width), QCPRange(0, img->height));
    rescaleAxes();

    for (int yIndex=0; yIndex<img->height; ++yIndex){
        //int sy = flipV ? (img->height - yIndex - 1) : yIndex;
        int sy = yIndex;
        /*if(flipH){
            for (int xIndex=0; xIndex<img->width; ++xIndex) colorMap->data()->setCell(xIndex, yIndex, img->getValue(img->width - xIndex - 1, sy));
        }else{*/
        for (int xIndex=0; xIndex<img->width; ++xIndex) colorMap->data()->setCell(xIndex, yIndex, img->getValue(xIndex, sy));
        //}
    }

    minRing[ringIdx] = img->min;
    maxRing[ringIdx] = img->max;
    ringIdx = (ringIdx+1)%AUTORANGE_RING_SIZE;

    maxMarker->setPosition(img->maxX, img->maxY);
    maxMarker->updateText(img);
    maxMarker->updateName("Max");

    minMarker->setPosition(img->minX, img->minY);
    minMarker->updateText(img);
    minMarker->updateName("Min");

    updateUserMarkers();
    reRange();
}

void ThermalPlot::updateUserMarkers()
{
    for(auto & mkr : userMarkers){
        mkr->updateText(img);
    }
}

void ThermalPlot::plotClick(QMouseEvent *event)
{
    double key,val;
    colorMap->pixelsToCoords(QPointF(event->pos()),key,val);
    int rx, ry;
    colorMap->data()->coordToCell(key,val,&rx,&ry);

    bool flag = false;

    /* Left button */

    if(event->button() == Qt::LeftButton){ /* Remove / Adding Markers */
        /* Remove Markers */
        userMarkers.remove_if([&](std::shared_ptr<Marker> & mkr){
            int dx = mkr->dataX - rx;
            int dy = mkr->dataY - ry;
            if(std::sqrt(dx*dx + dy*dy) < SELECT_RANGE){
                flag = true;
                return true;
            }
            return false;
        });

        if(flag){
            replot(QCustomPlot::rpQueuedReplot);
            return;
        }

        /* Add Markers */
        auto mkr = std::make_shared<Marker>(colorMap, Qt::green, Qt::white);
        QString mkrName = "Mkr " + QString::number(userMarkers.size());
        mkr->updateName(mkrName);
        mkr->setNameVislble(showUsrMkr);
        userMarkers.push_back(mkr);
        mkr->setPosition(rx, ry);
        updateUserMarkers();
        replot(QCustomPlot::rpQueuedReplot);
        return;
    }

    if(event->button() == Qt::RightButton){ /* Change Marker Name */
        for(const auto& mkr : userMarkers){
            int dx = mkr->dataX - rx;
            int dy = mkr->dataY - ry;

            if(std::sqrt(dx*dx + dy*dy) < SELECT_RANGE){
                bool ok;
                QString newName = QInputDialog::getText(this, "Change Marker Name", "Please Enter a Markername:", QLineEdit::Normal, mkr->name->text(), &ok);

                if (ok){
                    mkr->updateName(newName);
                    replot(QCustomPlot::rpQueuedReplot);
                    return;
                }
            }
        }
    }
}
