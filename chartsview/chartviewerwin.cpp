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

    createActions();
    ui->customPlot->setInteractions(QCP::iSelectAxes |
                                    QCP::iSelectLegend |
                                    QCP::iSelectPlottables);
    setupSignalAndSlot();

    setGeometry(400, 250, 542, 390);

    //openJsonFile("/Users/andrewhoo/Work/Qt/Pilot/201602_Throttle/data.json");

    //createSceneAndView();
    //createWidgets();
}

ChartViewerWin::~ChartViewerWin()
{
    delete ui;
}

bool ChartViewerWin::openJsonFile(const QString& jsonFileName)
{
    bool isFailed = false;
    DataJsonCfgReader reader;

    if (!reader.loadData(jsonFileName))
    {
        QMessageBox warningBox(QMessageBox::Warning, tr("Warning"),
                             tr("This file can't be read with reason of either \n"
                                " - json file format was corrupted. \n"
                                " - data file was corrupted"),
                             QMessageBox::Close);
        warningBox.exec();
        return false;
    }
    cfgMetaData = reader.getCfgParser();
    cfgRawData = reader.csvDataHandler();

    initAxesAndView(ui->customPlot);

    QVector<QCPGraphData> sample_4;
    QString sample_name_4;
    quint8 motorIdx_4;
    generateData(9, sample_4, sample_name_4, motorIdx_4);
    addGraph(ui->customPlot, sample_4, sample_name_4);

    // prepare data:
    QVector<QCPGraphData> sample;
    QString sample_name;
    quint8 motorIdx;
    generateData(3, sample, sample_name, motorIdx);
    addGraph(ui->customPlot, sample, sample_name);

    QVector<QCPGraphData> sample_3;
    QString sample_name_3;
    quint8 motorIdx_3;
    generateData(4, sample_3, sample_name_3, motorIdx_3);
    addGraph(ui->customPlot, sample_3, sample_name_3);

    QString title = TestPlanStringMap[(int)cfgMetaData.plan()];
    setWindowTitle(title);
    statusBar()->clearMessage();
    ui->customPlot->replot();
}

void ChartViewerWin::createActions()
{
    QToolBar *fileToolBar = addToolBar(tr("File"));

    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/ui/ui/open.png"));
    QAction *openAct = new QAction(openIcon, tr("&Open..."), this);
    //openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &ChartViewerWin::open);
    fileToolBar->addAction(openAct);
}

void ChartViewerWin::fillDataInTableWidget(QTableWidget * widget)
{
//    widget->clear();

//    QTableWidgetItem *sizeItem = new QTableWidgetItem(tr("%1 KB")
//                                         .arg(int((size + 1023) / 1024)));
//    sizeItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

//    int row = filesTable->rowCount();
//    filesTable->insertRow(row);
//    filesTable->setItem(row, 0, fileNameItem);
//    filesTable->setItem(row, 1, sizeItem);
}

void ChartViewerWin::generateData(quint32 idx, QVector<QCPGraphData>& pairs, QString& name, quint8& motorIdx)
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
                name = rawData[0].getName(idx),
                motorIdx = rawData[0].getMotorIdx(idx);

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

void ChartViewerWin::addGraph(QCustomPlot *customPlot, QVector<QCPGraphData> &pairs, QString &name, quint8 motorIdx)
{
    QCPAxisRect *rect = new QCPAxisRect(customPlot);
    rect->setMarginGroup(QCP::msLeft | QCP::msRight, marginGroup);
    rect->axis(QCPAxis::atBottom)->grid()->setVisible(true);
    rect->axis(QCPAxis::atLeft)->setLabelFont(QFont(QFont().family(), 10));
    rect->axis(QCPAxis::atLeft)->setLabel(name + " - " + QString::number(motorIdx));
    rect->axis(QCPAxis::atLeft)->setTickLabelFont(QFont(QFont().family(), 8));
    //rect->axis(QCPAxis::atLeft)->setPadding(3);
    //rect->axis(QCPAxis::atLeft)->setLabelPadding(15);

    foreach (QCPAxis *axis, rect->axes())
    {
        axis->setLayer("axes");
        axis->grid()->setLayer("grid");
    }

    customPlot->plotLayout()->addElement(rect);

    // create and configure plottables:
    QCPGraph *mainGraphCos = customPlot->addGraph(rect->axis(QCPAxis::atBottom), rect->axis(QCPAxis::atLeft));
    mainGraphCos->data()->set(pairs);
    //mainGraphCos->setName(sample_name);
    mainGraphCos->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlus, QPen(Qt::black), QBrush(Qt::white), 6));
    mainGraphCos->setPen(QPen(QColor("#FFA100"), 2));
    //mainGraphCos->valueAxis()->setRange(-1, 1);
    mainGraphCos->rescaleAxes();

    ui->customPlot->replot();
}

void ChartViewerWin::setupSignalAndSlot()
{
    // connect some interaction slots:
    connect(ui->customPlot, SIGNAL(axisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)),
            this, SLOT(axisLabelDoubleClick(QCPAxis*,QCPAxis::SelectablePart)));

    // setup policy and connect slot for context menu popup:
    ui->customPlot->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->customPlot, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(contextMenuRequest(QPoint)));
}

void ChartViewerWin::removeAllGraphs()
{
    ui->customPlot->clearGraphs();
    ui->customPlot->plotLayout()->clear();
    ui->customPlot->replot();
}

void ChartViewerWin::axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part)
{
    QPoint pos;
    contextMenuRequest(pos);
}

void ChartViewerWin::open()
{
    QString path = QCoreApplication::applicationDirPath();
    const QString fileName = QFileDialog::getOpenFileName(this,
                                                          tr("Open Json File"),
                                                          path,
                                                          tr("Json Files (*.json)"));
    if (!fileName.isEmpty())
    {
        openJsonFile(fileName);
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

void ChartViewerWin::contextMenuRequest(QPoint pos)
{
    QMenu *menu = new QMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);

    menu->addAction("Remove all graphs", this, SLOT(removeAllGraphs()));

    if (cfgRawData)
    {
        if (cfgRawData->type() == CfgWashingTypeEnum::CFGWASHINGTHROTTLE_E2)
        {
            quint32 idx = 0;
            CfgMeasBasedThrottleE2DataEle actionlist;
            for (CfgMetaElement& ele : actionlist.m_metaEle)
            {
                QAction* action = menu->addAction("add "
                                                  + ele.getName()
                                                  + '-'
                                                  + QString::number(ele.getMotorIdx()));
                action->setCheckable(true);
                action->setData(QVariant(idx++));
            }

            QAction *selectedAction = menu->exec(QCursor::pos());
            if (selectedAction)
            {
                int selectIdx = selectedAction->data().toInt();

                // prepare data:
                QVector<QCPGraphData> sample;
                QString sample_name;
                quint8 motorIdx;
                generateData(selectIdx, sample, sample_name, motorIdx);
                addGraph(ui->customPlot, sample, sample_name);
            }
        }
    }
}

void ChartViewerWin::initAxesAndView(QCustomPlot *customPlot)
{
    //customPlot->legend->setVisible(true);
    //demoName = "Advanced Axes Demo";
    marginGroup = new QCPMarginGroup(customPlot);

    // configure axis rect:
    customPlot->plotLayout()->clear(); // clear default axis rect so we can start from scratch
}
