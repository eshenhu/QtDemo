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
    void initAxesAndView(QCustomPlot* customPlot);
    void generateData(quint32 idx, QVector<QCPGraphData>& pairs, QString& name, quint8& motorIdx);

    void addGraph(QCustomPlot *customPlot, QVector<QCPGraphData>& pairs, QString& name);
    void contextMenuRequest(QPoint pos);

    void setupSignalAndSlot();
    void createActions();
    //void delGraph();
    //void updateGraph();

private:
    Ui::ChartViewerWin *ui;

    CfgJsonRecElement cfgMetaData;
    QSharedPointer<CfgWashingDataInf> cfgRawData;

    QCPMarginGroup *marginGroup;
//    QGraphicsScene* m_scene;
    //QVarChartView* m_view = nullptr;

private slots:
    void removeAllGraphs();
    void axisLabelDoubleClick(QCPAxis* axis, QCPAxis::SelectablePart part);
    void open();
};

#endif // CHARTVIEWERWIN_H
