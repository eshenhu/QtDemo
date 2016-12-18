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

    void createSceneAndView(QCustomPlot *customPlot);
//    void createActions();
//    void createMenusAndToolBars();
//    void createConnections();

    //void createWidgets();
//    void createProxyWidgets();
    bool openJsonFile(const QString& jsonFileName);
    //void makeNewChartWidget();
    void initAxesAndView(QCustomPlot* customPlot);
    void generateData(quint32 idx, QVector<QCPGraphData>& pairs, QString& name, quint8& motorIdx);

    void addGraph(QCustomPlot *customPlot, QVector<QCPGraphData>& pairs, QString& name, quint8 motorIdx = 0);
    void contextMenuRequest(QPoint pos);

    void setupSignalAndSlot();
    void createActions();
    void fillDataInTableWidget(QTableWidget *);
    //void delGraph();
    //void updateGraph();
    void initTrackFinance(QCustomPlot *customPlot, int mouseX = 0);
    void trackFinance(QCustomPlot *customPlot, int mouseX = 0);    // Draw the track line

private:
    Ui::ChartViewerWin *ui = nullptr;
    QCPTextElement *m_title = nullptr;
    QCPItemStraightLine* vCursor = nullptr;
    CfgJsonRecElement cfgMetaData;
    QSharedPointer<CfgWashingDataInf> cfgRawData;

    QCPMarginGroup *marginGroup = nullptr;
//    QGraphicsScene* m_scene;
    //QVarChartView* m_view = nullptr;

private slots:
    void removeAllGraphs();
    void clearGraphsExceptTitle();
    void axisLabelDoubleClick(QCPAxis* axis, QCPAxis::SelectablePart part);
    void open();
    void showVLineItem(QMouseEvent *event);
};

#endif // CHARTVIEWERWIN_H
