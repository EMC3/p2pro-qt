#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <colormap/colormap.h>
#include "thermalcamera.h"

class QCPColorMap;
class QCPItemTracer;
class QCPItemText;

struct Marker;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



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

    void on_flipH_toggled(bool checked);

    void on_flipV_toggled(bool checked);

private:
    void updateUserMarkers();
    void reRange();

    void resetMinMaxRing();


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

    void setFlipSettings();

    float minRing[64];
    float maxRing[64];
    int ringIdx;
};
#endif // MAINWINDOW_H
