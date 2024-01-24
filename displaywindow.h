#ifndef DISPLAYWINDOW_H
#define DISPLAYWINDOW_H

#include <QMainWindow>
#include <colormap/colormap.h>
#include "thermalcamera.h"
#include "historymonitor.h"



namespace Ui {
class DisplayWindow;
}

class QCPColorMap;
struct Marker;
class SettingsWidget;
class ColorMapManager;

class DisplayWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DisplayWindow(QWidget *parent = nullptr);
    ~DisplayWindow();

    void keyPressEvent(QKeyEvent *ev);

signals:
    void colormapChanged(int cmIdx);
    void flippedH();
    void flippedV();
    void colorInverted();

private slots:
    void on_cmapLock_clicked();
    void on_savePhoto_clicked();
    void on_settings_clicked();
    void on_playPause_clicked();
    void on_tbHistory_clicked();

private:
    Ui::DisplayWindow *ui;

    std::shared_ptr<ThermalImage> img;
    ThermalCamera * tc;


    void displayImage();

    void updateCmap(std::string name, bool inv);

    void tcStatusUpdate(QString statusStr);
    void setMarkerSettings(bool showMax, bool showMin, bool showMid, bool showUsrName);

    QString videoDev;

    std::string savePath;
    int saveIndex;

    SettingsWidget * sw;
    historyMonitor * hm;
    void applySettings();
    void findSaveIndex();
    void showCam();
};

#endif // DISPLAYWINDOW_H
