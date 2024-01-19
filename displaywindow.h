#ifndef DISPLAYWINDOW_H
#define DISPLAYWINDOW_H

#include <QMainWindow>
#include <colormap/colormap.h>
#include "thermalcamera.h"



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

private slots:
    void on_cmapLock_clicked();

    void on_savePhoto_clicked();

    void on_settings_clicked();

    void on_playPause_clicked();

private:
    Ui::DisplayWindow *ui;

    std::shared_ptr<ThermalImage> img;
    ThermalCamera * tc;


    void displayImage();

    void updateCmap(std::string name, bool inv);

    void tcStatusUpdate(QString statusStr);
    void setMarkerSettings(bool showMax, bool showMin, bool showMid);

    QString videoDev;

    std::string savePath;
    int saveIndex;

    SettingsWidget * sw;
    void applySettings();
    void findSaveIndex();
};

#endif // DISPLAYWINDOW_H
