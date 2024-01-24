#include "historymonitor.h"
#include "ui_historymonitor.h"

#include <QVector>
#include <QFile>
#include "qcp/qcustomplot.h"

historyMonitor::historyMonitor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::historyMonitor)
{
    ui->setupUi(this);

    /* QCustom Plot TryOut */
    setupGraphs();


    /* Connect Timer to Update Graph */
    ticktack = new QTimer(this);
    connect(ticktack,&QTimer::timeout,this,&historyMonitor::updateGraphs);
    ticktack->start(1000);
}

historyMonitor::~historyMonitor()
{
    delete ui;
    delete ticktack;
}

void historyMonitor::on_pbBack_clicked()
{
    emit back();
}

void historyMonitor::updateGraphs()
{
    double now = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000;
    //qDebug() << "Tick: " << now << "ms";

    QCustomPlot * historyPlot;
    historyPlot = ui->qcpHistory;
    historyPlot->replot(QCustomPlot::rpQueuedReplot);
}

void historyMonitor::setupGraphs()
{
    QCustomPlot * historyPlot = ui->qcpHistory;
    QColor standardColor[2];
    /* Red: 220, 42, 150, 150*/
    /* Blue: 20, 112, 150, 150*/
    standardColor[0] = QColor(220, 42, 150, 150);   // Max
    standardColor[1] = QColor(20, 112, 150, 150);   // Min

    // set locale to english, so we get english month names:
    historyPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
    //customPlot->setBackground(QBrush(QColor("#19232D")));

    // seconds of current time, we'll use it as starting point in time for data:
    double now = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

    // create multiple graphs:
    for (int idx=0; idx < 2; idx++)
    {
        historyPlot->addGraph();
        historyPlot->graph()->setLineStyle(QCPGraph::lsLine);
        //historyPlot->graph()->setPen(QPen(standardColor[idx].lighter(200)));
        historyPlot->graph()->setPen(QPen(standardColor[idx]));
        historyPlot->graph()->setBrush(QBrush(standardColor[idx]));

        // generate random walk data:
        QVector<QCPGraphData> timeData(250);
        for (int i=0; i<250; ++i)
        {
            timeData[i].key = now + 24*3600*i;
            if (i == 0){timeData[i].value = (i/50.0+1)*(rand()/(double)RAND_MAX-0.5);}
            else{timeData[i].value = qFabs(timeData[i-1].value)*(1+0.02/4.0*(4-idx)) + (i/50.0+1)*(rand()/(double)RAND_MAX-0.5);}

        }
        historyPlot->graph()->data()->set(timeData);

    }
    // configure bottom axis to show date instead of number:
    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    //dateTicker->setDateTimeFormat("hh:mm:ss dd-MM\nyyyy");
    historyPlot->xAxis->setTicker(dateTicker);
    // configure left axis text labels:
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTick(10, "a bit\nlow");
    textTicker->addTick(50, "quite\nhigh");
    historyPlot->yAxis->setTicker(textTicker);
    // set a more compact font size for bottom and left axis tick labels:
    historyPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    historyPlot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
    // set axis labels:
    historyPlot->xAxis->setLabel("Time");
    historyPlot->yAxis->setLabel("Temperature [°C]");
    // make top and right axes visible but without ticks and labels:
    historyPlot->xAxis2->setVisible(true);
    historyPlot->yAxis2->setVisible(true);
    historyPlot->xAxis2->setTicks(false);
    historyPlot->yAxis2->setTicks(false);
    historyPlot->xAxis2->setTickLabels(false);
    historyPlot->yAxis2->setTickLabels(false);
    // set axis ranges to show all data:

    historyPlot->xAxis->setRange(now, now+24*3600*249);
    historyPlot->yAxis->setRange(0, 60);

    // show legend with slightly transparent background brush:
    /*
    customPlot->legend->setVisible(true);
    customPlot->legend->setBrush(QColor(255, 255, 255, 150));
    */
}

