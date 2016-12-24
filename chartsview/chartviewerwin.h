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
    enum class ChartViewIdxSupFileE : quint32
    {
        IDX_SUP_FILE_I,
        IDX_SUP_FILE_II,

        IDX_SUP_FILE_MAX
    };

    class ChartViewCfgElement
    {
    public:
        void reset(){
            cfgMetaData = CfgJsonRecElement();
            cfgRawData.reset();
        }

    public:
        CfgJsonRecElement cfgMetaData;
        QSharedPointer<CfgWashingDataInf> cfgRawData;
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
    bool openJsonFile(const QString& jsonFileName, quint32 location);
    //void makeNewChartWidget();
    //void resetDataStoHandler(quint8 idx);
    void initAxesAndView(QCustomPlot* customPlot);
    void generateData(QSharedPointer<CfgWashingDataInf> cfgRawData, quint32 idx,
                      QVector<QCPGraphData>& pairs, QString& name, quint8& motorIdx);


    bool validateMetaData();
    void setupSignalAndSlot();
    void createActions();
    //void delGraph();
    //void updateGraph();
//    void initTrackFinance(QCustomPlot *customPlot, int mouseX = 0);
//    void trackFinance(QCustomPlot *customPlot, int mouseX = 0);    // Draw the track line
    void updateAxisAtBottomRect(QCustomPlot *customPlot);

    bool loadDefault2Plot();

    QCPGraph* addGraph(QCPAxisRect* rect);
    void updateGraph(QCPAxisRect* rect, quint32 idx);
    void updateGraph(QCPGraph * graph, QVector<QCPGraphData> &pairs, QString &name, quint8 motorIdx);

    QCPAxisRect* addRect(QCustomPlot *customPlot);


    void showText();

private:
    Ui::ChartViewerWin *ui = nullptr;

    //QSharedPointer<QCPTextElement> m_title;
    QCPTextElement* m_title;

    QCPItemStraightLine* vCursor = nullptr;


    QVector<ChartViewerWin::ChartViewCfgElement> cfgElementList;
    CfgJsonRecElement& cfgMetaData;
    QSharedPointer<CfgWashingDataInf>& cfgRawData;


    QCPMarginGroup *marginGroup = nullptr;
    QCPAxisRect* m_assoRect = nullptr;

    double m_key_diff = 0;
//    QGraphicsScene* m_scene;
    //QVarChartView* m_view = nullptr;
signals:
    void leftFileOk(bool);

private slots:
    void removeAllGraphs();
    void removeGraph();
//    void addGraph();

//    void removeGraph(QCPAxisRect* rect);
    void clearGraphsExceptTitle();
    void axisLabelDoubleClick(QCPAxis* axis, QCPAxis::SelectablePart part);
    void open();
    void open_validate();
    void addRect();
    void showVLineItem(QMouseEvent *event);
    void contextMenuRequest(QPoint pos);
    void contextMenuRequest(QCPAxis *axis);
    void contextMenuRequest(QCPAxisRect* rect);
    void releaseSignalAndSlot();
};

#endif // CHARTVIEWERWIN_H
