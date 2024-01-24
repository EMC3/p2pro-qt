#ifndef HISTORYMONITOR_H
#define HISTORYMONITOR_H

#include <QWidget>
#include <QTimer>
#include <QColor>

namespace Ui {
class historyMonitor;
}

class historyMonitor : public QWidget
{
    Q_OBJECT

public:
    explicit historyMonitor(QWidget *parent = nullptr);
    ~historyMonitor();

signals:
    void back();

private slots:
    void on_pbBack_clicked();

    void updateGraphs();

private:
    Ui::historyMonitor *ui;
    void setupGraphs();
    QTimer * ticktack;
};

#endif // HISTORYMONITOR_H
