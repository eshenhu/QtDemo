#ifndef CHARTVIEWERWIN_H
#define CHARTVIEWERWIN_H

#include <QMainWindow>
#include <QtCharts/QChartGlobal>
#include <QtCharts/QChart>

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

private:
    Ui::ChartViewerWin *ui;
    QGraphicsScene* m_scene;
};

#endif // CHARTVIEWERWIN_H