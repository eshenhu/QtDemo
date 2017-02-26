#ifndef CHARTVIEWERWIN_H
#define CHARTVIEWERWIN_H

#include <QMainWindow>
#include <QtCharts/QChartGlobal>
#include <QtCharts/QChart>

#include "cfg/datajsoncfgreader.h"
#include "chartsview/qcustomplot.h"
#include "ui/fileselectdialog.h"
class QCustomPlot;
namespace Ui {
class ChartViewerWin;
}

QT_CHARTS_USE_NAMESPACE

class ChartViewerWin : public QMainWindow
{
    Q_OBJECT
public:
    enum class ChartViewIdxSupFileE : quint32
    {
        IDX_SUP_FILE_I  = 0,
        IDX_SUP_FILE_II = 1,

        IDX_SUP_FILE_MAX = MAX_ROW_FILE_SELECTION
    };

public:
    explicit ChartViewerWin(QWidget *parent = 0);
    ~ChartViewerWin();

    void createSceneAndView(QCustomPlot *customPlot);
//    void createActions();
//    void createMenusAndToolBars();
//    void createConnections();

    //void createWidgets();
//    void createProxyWidgets();
    //void makeNewChartWidget();
    //void resetDataStoHandler(quint8 idx);
    void initAxesAndView(QCustomPlot* customPlot);
    void generateData(QSharedPointer<CfgWashingDataInf> cfgRawData, quint32 idx,
                      QVector<QCPGraphData>& pairs, QString& name, quint8& motorIdx);

    void setupSignalAndSlot();
    void createActions();
    //void delGraph();
    //void updateGraph();
//    void initTrackFinance(QCustomPlot *customPlot, int mouseX = 0);
//    void trackFinance(QCustomPlot *customPlot, int mouseX = 0);    // Draw the track line
    void updateAxisAtBottomRect(QCustomPlot *customPlot);

    bool loadDefault2Plot();

    QCPGraph* addGraph(QCPAxisRect* rect);
    void updateGraphDuringMultiplePlan(quint32 baseIdx);
    void updateGraph(QCPAxisRect* rect, quint32 idx);
    void updateGraph(QCPGraph * graph, QVector<QCPGraphData> &pairs, QString &name, quint8 motorIdx, quint32 idxOfGraph);

    QCPAxisRect* addRect(QCustomPlot *customPlot);
    void rescalePlot();

    void showText();


private:
    Ui::ChartViewerWin *ui = nullptr;
    QAction *actionOpen;
    QAction* actionCurve;
    QAction* actionExportPDF;
    QAction* actionExportCSV;

    quint32 itemListIdx = 0;
    //QSharedPointer<QCPTextElement> m_title;
    QCPTextElement* m_title;

    QCPItemStraightLine* vCursor = nullptr;

    QMap<QCPAxisRect*, QCPLegend*> m_mapFromAxisToLegend;

    QVector<ChartViewCfgElement> cfgElementList;
    CfgJsonRecElement cfgMetaData;
    QSharedPointer<CfgWashingDataInf> cfgRawData;


    QCPMarginGroup *marginGroup = nullptr;
    QCPAxisRect* m_assoRect = nullptr;

    double m_key_diff = 0;
    bool m_isCmpEnabled = false;
//    QGraphicsScene* m_scene;
    //QVarChartView* m_view = nullptr;



signals:
    //void leftFileOk(bool);
    void testPlanChanged(TestPlanEnum plan);
private slots:
    void removeAllGraphs();
    void removeGraph();
//    void addGraph();

//    void removeGraph(QCPAxisRect* rect);
    void clearGraphsExceptTitle();
    void axisLabelDoubleClick(QCPAxis* axis, QCPAxis::SelectablePart part);
    void callbackActionOpen2Cmp();
    void callbackOnActionExportPDF();
    void callbackOnActionExportCSV();
    void addRect();
    void showVLineItem(QMouseEvent *event);
    void contextMenuRequest(QPoint pos);
    void contextMenuRequest(QCPAxis *axis);
    void contextMenuRequest(QCPAxisRect* rect);
    void releaseSignalAndSlot();    

private:
    /* --- Legend Relative --- */
    QCPLegend* setupLegend(const QCPAxisRect*);
    void addToLegend(QCPLegend*);

    quint32 setupIdxMultipleTest(quint32 idx);
    void saveCSV(const QString& filename);
};

#endif // CHARTVIEWERWIN_H
