#include "chartviewerwin.h"
#include "ui_chartviewerwin.h"

#include "ui/uiheader.h"
#include <QGraphicsLinearLayout>
#include <QLineSeries>
#include <QValueAxis>
#include "cfg/datajsoncfgreader.h"

ChartViewerWin::ChartViewerWin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChartViewerWin)
{
    ui->setupUi(this);

    setGeometry(400, 250, 542, 390);

    openJsonFile("/Users/andrewhoo/Work/Qt/Pilot/20161205-210133097/data.json");

    //createSceneAndView();
    //createWidgets();
}

ChartViewerWin::~ChartViewerWin()
{
    delete ui;
}

void ChartViewerWin::openJsonFile(const QString& jsonFileName)
{
    DataJsonCfgReader reader;
    reader.loadData(jsonFileName);

    cfgMetaData = reader.getCfgParser();
    cfgRawData = reader.csvDataHandler();

    setupAdvancedAxesDemo(ui->customPlot);

    QString title = TestPlanStringMap[(int)cfgMetaData.plan()];
    setWindowTitle(title);
    statusBar()->clearMessage();
    ui->customPlot->replot();
}

void ChartViewerWin::createSceneAndView()
{
//    DataJsonCfgReader reader;
//    reader.loadData("/Users/andrewhoo/Work/Qt/Pilot/20161205-210133097/data.json");
//    m_scene = new QGraphicsScene(this);
//    m_scene->setSceneRect(0, 0, ui->graphicsView->width(), ui->graphicsView->height());
//    ui->graphicsView->setScene(m_scene);
}


void ChartViewerWin::setupAdvancedAxesDemo(QCustomPlot *customPlot)
{
  //demoName = "Advanced Axes Demo";

  // configure axis rect:
  customPlot->plotLayout()->clear(); // clear default axis rect so we can start from scratch
  QCPAxisRect *wideAxisRect = new QCPAxisRect(customPlot);
//  wideAxisRect->setupFullAxesBox(true);
  //wideAxisRect->axis(QCPAxis::atRight, 0)->setTickLabels(true);
  //wideAxisRect->addAxis(QCPAxis::atLeft)->setTickLabelColor(QColor("#6050F8")); // add an extra axis on the left and color its numbers


  //customPlot->plotLayout()->setRowStretchFactor(1, 2);
  // prepare axis rects that will be placed in the sublayout:
  QCPAxisRect *subRectLeft = new QCPAxisRect(customPlot, true); // false means to not setup default axes
//  subRectLeft->axis(QCPAxis::atLeft)->ticker()->setTickCount(2);
//  subRectRight->axis(QCPAxis::atBottom)->ticker()->setTickCount(2);
  subRectLeft->axis(QCPAxis::atBottom)->grid()->setVisible(true);
  // synchronize the left and right margins of the top and bottom axis rects:
  QCPMarginGroup *marginGroup = new QCPMarginGroup(customPlot);
  subRectLeft->setMarginGroup(QCP::msLeft | QCP::msRight, marginGroup);
  wideAxisRect->setMarginGroup(QCP::msLeft | QCP::msRight, marginGroup);

  //  QCPLayoutGrid *subLayout = new QCPLayoutGrid;
    customPlot->plotLayout()->addElement(0, 0, wideAxisRect); // insert axis rect in first row
    customPlot->plotLayout()->addElement(1, 0, subRectLeft); // sub layout in second row (grid layout will grow accordingly)

  // move newly created axes on "axes" layer and grids on "grid" layer:
  foreach (QCPAxisRect *rect, customPlot->axisRects())
  {
    foreach (QCPAxis *axis, rect->axes())
    {
      axis->setLayer("axes");
      axis->grid()->setLayer("grid");
    }
  }

  // prepare data:
  QVector<QCPGraphData> dataCos(21), dataGauss(50), dataRandom(100);
  QVector<double> x3, y3;
  qsrand(3);
  for (int i=0; i<dataCos.size(); ++i)
  {
    dataCos[i].key = i/(double)(dataCos.size()-1)*10-5.0;
    dataCos[i].value = qCos(dataCos[i].key);
  }
  for (int i=0; i<dataGauss.size(); ++i)
  {
    dataGauss[i].key = i/(double)dataGauss.size()*10-5.0;
    dataGauss[i].value = qExp(-dataGauss[i].key*dataGauss[i].key*0.2)*1000;
  }
  for (int i=0; i<dataRandom.size(); ++i)
  {
    dataRandom[i].key = i/(double)dataRandom.size()*10;
    dataRandom[i].value = qrand()/(double)RAND_MAX-0.5+dataRandom[qMax(0, i-1)].value;
  }
  x3 << 1 << 2 << 3 << 4;
  y3 << 2 << 2.5 << 4 << 1.5;

  // create and configure plottables:
  QCPGraph *mainGraphCos = customPlot->addGraph(wideAxisRect->axis(QCPAxis::atBottom), wideAxisRect->axis(QCPAxis::atLeft));
  mainGraphCos->data()->set(dataCos);
  mainGraphCos->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlus, QPen(Qt::black), QBrush(Qt::white), 6));
  mainGraphCos->setPen(QPen(QColor(120, 120, 120), 2));
  mainGraphCos->valueAxis()->setRange(-1, 1);
  mainGraphCos->rescaleAxes();

//  QCPGraph *mainGraphGauss = customPlot->addGraph(wideAxisRect->axis(QCPAxis::atBottom), wideAxisRect->axis(QCPAxis::atLeft, 1));
//  mainGraphGauss->data()->set(dataGauss);
//  mainGraphGauss->setPen(QPen(QColor("#8070B8"), 2));
//  mainGraphGauss->setBrush(QColor(110, 170, 110, 30));
//  mainGraphCos->setChannelFillGraph(mainGraphGauss);
//  mainGraphCos->setBrush(QColor(255, 161, 0, 50));
//  mainGraphGauss->valueAxis()->setRange(0, 1000);
//  mainGraphGauss->rescaleKeyAxis();

  QCPGraph *subGraphRandom = customPlot->addGraph(subRectLeft->axis(QCPAxis::atBottom), subRectLeft->axis(QCPAxis::atLeft));
  subGraphRandom->data()->set(dataRandom);
  //subGraphRandom->setLineStyle(QCPGraph::lsLine);
  subGraphRandom->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlus, QPen(Qt::black), QBrush(Qt::white), 6));
  subGraphRandom->setPen(QPen(QColor("#FFA100"), 1.5));
  subGraphRandom->rescaleAxes();

//  QCPBars *subBars = new QCPBars(subRectRight->axis(QCPAxis::atBottom), subRectRight->axis(QCPAxis::atRight));
//  subBars->setWidth(3/(double)x3.size());
//  subBars->setData(x3, y3);
//  subBars->setPen(QPen(Qt::black));
//  subBars->setAntialiased(false);
//  subBars->setAntialiasedFill(false);
//  subBars->setBrush(QColor("#705BE8"));
//  subBars->keyAxis()->setSubTicks(false);
//  subBars->rescaleAxes();
//  // setup a ticker for subBars key axis that only gives integer ticks:
//  QSharedPointer<QCPAxisTickerFixed> intTicker(new QCPAxisTickerFixed);
//  intTicker->setTickStep(1.0);
//  intTicker->setScaleStrategy(QCPAxisTickerFixed::ssMultiples);
//  subBars->keyAxis()->setTicker(intTicker);
}
