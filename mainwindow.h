#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <colormap/colormap.h>
#include "thermalcamera.h"

class QCPColorMap;
class QCPItemTracer;
class QCPItemText;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void plotImg();

private slots:
    void on_colormap_currentIndexChanged(int index);

    void on_colorInvert_toggled(bool checked);

    void on_btnStart_clicked();

    void on_btnStop_clicked();

    void on_lockb_clicked();

    void on_lockLower_toggled(bool checked);

    void on_lockUpper_toggled(bool checked);

    void on_unlockb_clicked();

    void plotClick(QMouseEvent *event);
    void plotHover(QMouseEvent *event);
    void on_upperBound_valueChanged(double arg1);

    void on_lowerBound_valueChanged(double arg1);

    void on_delMkr_clicked();

    void on_showMax_toggled(bool checked);

    void on_showMin_toggled(bool checked);

private:
    void updateUserMarkers();
    void reRange();

    Ui::MainWindow *ui;
    void updateCmap();
    std::vector<ColorMap> cmaps;
    QCPColorMap *colorMap;

    std::shared_ptr<ThermalImage> img;

    ThermalCamera * tc;

    void displayImage();
    void displayStatus(QString s);

    bool upperLock, lowerLock;

    Marker * maxMarker, * minMarker;

    std::list<std::shared_ptr<Marker>> userMarkers;

};
#endif // MAINWINDOW_H
