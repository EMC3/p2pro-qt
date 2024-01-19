#include "marker.h"
#include "qcp/qcustomplot.h"
#include "thermalcamera.h"


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
