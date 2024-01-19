#ifndef MARKER_H
#define MARKER_H

#include <QColor>
#include <memory>

class QCPItemTracer;
class QCPItemText;
class QCPColorMap;
class ThermalImage;

struct Marker{
    QCPItemTracer * tracer;
    QCPItemText * label;
    QCPColorMap *colorMap;
    int dataX, dataY;
    bool hidden;
    Marker(QCPColorMap *colorMap, QColor markerColor, QColor textColor = Qt::white);
    ~Marker();
    void setPosition(int dataX, int dataY);

    void updateText(const std::shared_ptr<ThermalImage> & img);

    void setHidden(bool hide);
};

#endif // MARKER_H
