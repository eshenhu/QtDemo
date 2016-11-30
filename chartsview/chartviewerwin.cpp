#include "chartviewerwin.h"
#include "ui_chartviewerwin.h"

#include <QGraphicsLinearLayout>
#include <QLineSeries>
#include <QValueAxis>

ChartViewerWin::ChartViewerWin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChartViewerWin)
{
    ui->setupUi(this);
    setFixedSize(ui->graphicsView->width(), ui->graphicsView->height());

    createSceneAndView();
    createWidgets();
}

ChartViewerWin::~ChartViewerWin()
{
    delete ui;
}

void ChartViewerWin::createSceneAndView()
{
    m_scene = new QGraphicsScene(this);
    m_scene->setSceneRect(0, 0, ui->graphicsView->width(), ui->graphicsView->height());
    ui->graphicsView->setScene(m_scene);
}

void ChartViewerWin::createWidgets()
{
    QLineSeries *series = new QLineSeries();
    *series << QPointF(1, 1) << QPointF(2, 2) << QPointF(3, 3) << QPointF(4, 4) << QPointF(5, 5) << QPointF(6, 6);

    QChart *chart = new QChart();
    chart->addSeries(series);
    //chart->setTheme(QChart::ChartThemeBrownSand);
    //chart->setMargins(QMargins(0,0,0,0));
    chart->legend()->hide();

    QValueAxis *axisX = new QValueAxis;
//    //axisX->setFormat("dd-MM h:mm:s");
//    axisX->hide();
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis;
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout();

    layout->addItem(chart);

    QGraphicsWidget *widget = new QGraphicsWidget;
    widget->setLayout(layout);
    m_scene->addItem(widget);
}
