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

    name = new QCPItemText(plot);
    name->position->setParentAnchor(tracer->position);
    name->setPositionAlignment(Qt::AlignTop | Qt::AlignLeft);
    name->position->setCoords(0,-20);
    name->setColor(Qt::white);
    name->setBrush(QBrush(QColor(0,0,0,100)));
    name->setText("MarkerName");

    label = new QCPItemText(plot);
    label->position->setParentAnchor(tracer->position);
    label->setPositionAlignment(Qt::AlignTop | Qt::AlignLeft);
    label->setColor(Qt::white);
    label->setBrush(QBrush(QColor(0,0,0,100)));
    label->setText("XXX");
}

Marker::~Marker()
{
    colorMap->parentPlot()->removeItem(name);
    colorMap->parentPlot()->removeItem(label);
    colorMap->parentPlot()->removeItem(tracer);

    //delete tracer;
    //delete label;
    //delete name;

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

void Marker::updateName(const QString &newText){
    name->setText(newText);
}

void Marker::setNameVislble(bool value){
    name->setVisible(value);
}

void Marker::setHidden(bool hide)
{
    tracer->setVisible(!hide);
    label->setVisible(!hide);
    name->setVisible(!hide);
    hidden = hide;
}
