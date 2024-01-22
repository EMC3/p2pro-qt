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
    void invertColorBtn();
    void keyPressEvent(QKeyEvent *event);

public slots:
    void changeColormap(int cmIdx);
    void flipHKeyBtn();
    void flipVKeyBtn();


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
