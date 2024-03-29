#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>

namespace Ui {
class SettingsWidget;
}



class SettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWidget(QWidget *parent = nullptr);
    ~SettingsWidget();

    std::string selectedColormap;
    bool invertColormap;
    QString selectedVideoDevice;
    bool flipH;
    bool flipV;
    bool enableMinMkr;
    bool enableMaxMkr;
    bool enableCenterMkr;
    bool showUsrMkr;
    int rotation;   /* 0 = 0 Degree; 1 = 90 Degree; 2 = 180 Degree; 3 = 270 Degree; */
    void invertColorBtn();
    void keyPressEvent(QKeyEvent *event);

public slots:
    void changeColormap(int cmIdx);
    void flipHKeyBtn();
    void flipVKeyBtn();
    void toggleCrosshairBtn();
    void rotateKeyBtn();


private slots:
    void on_closeBtn_clicked();
    void on_delMarkers_clicked();

signals:
    void back();
    void delMarkers();
private:

    Ui::SettingsWidget *ui;
    void applySettings();
    void loadConfig();
};

#endif // SETTINGSWIDGET_H
