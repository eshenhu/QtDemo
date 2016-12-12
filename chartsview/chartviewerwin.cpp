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

void ChartViewerWin::generateData(quint32 idx, QVector<QCPGraphData>& pairs)
{
    if (cfgRawData)
    {
        if (cfgRawData->type() == CfgWashingTypeEnum::CFGWASHINGTHROTTLE_E2)
        {
            QSharedPointer<CfgThrottleWashingDataE2Clz> dynCast =
                    qSharedPointerDynamicCast<CfgThrottleWashingDataE2Clz>(cfgRawData);

            QVector<CfgMeasBasedThrottleE2DataEle>& rawData = dynCast->data();

            if (idx < rawData.size())
            {
                pairs.resize(rawData.size());
                for (int i = 0; i < rawData.size(); i++)
                {
                    pairs[i].key   = (quint32)rawData[i].getData((quint32)CfgMeasBasedThrottleE2DataEle::ELEMEASCURSOR::REC_THRO_POS);
                    pairs[i].value = rawData[i].getData(idx);
                }
            }
            else
            {
                qWarning() << "index exceed the maximum number of vector. index = " << idx;
            }
        }
    }
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
    QCPMarginGroup *marginGroup = new QCPMarginGroup(customPlot);

    // configure axis rect:
    customPlot->plotLayout()->clear(); // clear default axis rect so we can start from scratch

    QCPAxisRect *wideAxisI = new QCPAxisRect(customPlot);
    wideAxisI->setMarginGroup(QCP::msLeft | QCP::msRight, marginGroup);

    //customPlot->plotLayout()->setRowStretchFactor(1, 2);
    // prepare axis rects that will be placed in the sublayout:
    QCPAxisRect *wideAxisII = new QCPAxisRect(customPlot, true); // false means to not setup default axes
    //  wideAxisII->axis(QCPAxis::atLeft)->ticker()->setTickCount(2);
    //  subRectRight->axis(QCPAxis::atBottom)->ticker()->setTickCount(2);
    wideAxisII->axis(QCPAxis::atBottom)->grid()->setVisible(true);
    // synchronize the left and right margins of the top and bottom axis rects:
    wideAxisII->setMarginGroup(QCP::msLeft | QCP::msRight, marginGroup);

    QCPAxisRect *wideAxisIII = new QCPAxisRect(customPlot);
    wideAxisIII->setMarginGroup(QCP::msLeft | QCP::msRight, marginGroup);

    QCPAxisRect *wideAxisIV = new QCPAxisRect(customPlot);
    wideAxisIV->setMarginGroup(QCP::msLeft | QCP::msRight, marginGroup);

    customPlot->plotLayout()->addElement(0, 0, wideAxisI); // insert axis rect in first row
    customPlot->plotLayout()->addElement(1, 0, wideAxisII); // sub layout in second row (grid layout will grow accordingly)
    customPlot->plotLayout()->addElement(2, 0, wideAxisIII);
    customPlot->plotLayout()->addElement(3, 0, wideAxisIV);

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
    QVector<QCPGraphData> sample;
    generateData(3, sample);


    // create and configure plottables:
    QCPGraph *mainGraphCos = customPlot->addGraph(wideAxisI->axis(QCPAxis::atBottom), wideAxisI->axis(QCPAxis::atLeft));
    mainGraphCos->data()->set(sample);
    mainGraphCos->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlus, QPen(Qt::black), QBrush(Qt::white), 6));
    mainGraphCos->setPen(QPen(QColor(120, 120, 120), 2));
    //mainGraphCos->valueAxis()->setRange(-1, 1);
    mainGraphCos->rescaleAxes();

    //  QCPGraph *mainGraphGauss = customPlot->addGraph(wideAxisRect->axis(QCPAxis::atBottom), wideAxisRect->axis(QCPAxis::atLeft, 1));
    //  mainGraphGauss->data()->set(dataGauss);
    //  mainGraphGauss->setPen(QPen(QColor("#8070B8"), 2));
    //  mainGraphGauss->setBrush(QColor(110, 170, 110, 30));
    //  mainGraphCos->setChannelFillGraph(mainGraphGauss);
    //  mainGraphCos->setBrush(QColor(255, 161, 0, 50));
    //  mainGraphGauss->valueAxis()->setRange(0, 1000);
    //  mainGraphGauss->rescaleKeyAxis();

    QCPGraph *subGraphRandom = customPlot->addGraph(wideAxisII->axis(QCPAxis::atBottom), wideAxisII->axis(QCPAxis::atLeft));
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
