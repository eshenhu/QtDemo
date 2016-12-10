#ifndef CHARTVIEWERWIN_H
#define CHARTVIEWERWIN_H

#include <QMainWindow>
#include <QtCharts/QChartGlobal>
#include <QtCharts/QChart>
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

    void createWidgets();
//    void createProxyWidgets();

    //void makeNewChartWidget();
    void setupAdvancedAxesDemo(QCustomPlot* customPlot);
private:
    Ui::ChartViewerWin *ui;
//    QGraphicsScene* m_scene;
    //QVarChartView* m_view = nullptr;
};

#endif // CHARTVIEWERWIN_H
