#ifndef CHARTVIEWERWIN_H
#define CHARTVIEWERWIN_H

#include <QMainWindow>
#include <QtCharts/QChartGlobal>
#include <QtCharts/QChart>

#include "cfg/datajsoncfgreader.h"
#include "chartsview/qcustomplot.h"
class QCustomPlot;
namespace Ui {
class ChartViewerWin;
}

QT_CHARTS_USE_NAMESPACE

class ChartViewerWin : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChartViewerWin(QWidget *parent = 0);
    ~ChartViewerWin();

    void createSceneAndView();
//    void createActions();
//    void createMenusAndToolBars();
//    void createConnections();

    //void createWidgets();
//    void createProxyWidgets();
    void openJsonFile(const QString& jsonFileName);
    //void makeNewChartWidget();
    void setupAdvancedAxesDemo(QCustomPlot* customPlot);
    void generateData(quint32 idx, QVector<QCPGraphData>& pairs);
private:
    Ui::ChartViewerWin *ui;

    CfgJsonRecElement cfgMetaData;
    QSharedPointer<CfgWashingDataInf> cfgRawData;
//    QGraphicsScene* m_scene;
    //QVarChartView* m_view = nullptr;
};

#endif // CHARTVIEWERWIN_H
