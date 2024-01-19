#ifndef THERMALPLOT_H
#define THERMALPLOT_H

#include <QWidget>
#include "qcp/qcustomplot.h"


class ThermalImage;
class ColorMap;
struct Marker;

#define AUTORANGE_RING_SIZE 32

class ThermalPlot : public QCustomPlot
{
    Q_OBJECT
public:
    explicit ThermalPlot(QWidget *parent = nullptr);
    void setFlipSettings(bool flipH, bool flipV);
    void setImage(std::shared_ptr<ThermalImage> & image);

    void reRange();
    void deleteUserMarkers();

    void resetAutoRange();

    void setColorMap(ColorMap & cmap, bool inv);

    void autoReplot();

    bool autoColorRange;
    double minCRange, maxCRange;
    Marker * maxMarker;
    Marker * minMarker;
    std::list<std::shared_ptr<Marker>> userMarkers;
private:

    QCPColorMap *colorMap;

    void plotImg();

    void updateUserMarkers();

    void plotClick(QMouseEvent *event);

    float minRing[AUTORANGE_RING_SIZE];
    float maxRing[AUTORANGE_RING_SIZE];
    int ringIdx;

    std::shared_ptr<ThermalImage> img;

signals:
};

#endif // THERMALPLOT_H
