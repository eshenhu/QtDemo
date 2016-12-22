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
//    initTrackFinance(ui->customPlot);
//    trackFinance(ui->customPlot);
//    ui->customPlot->setInteractions(
//                QCP::iSelectAxes
//                                    | QCP::iSelectLegend
//                                    | QCP::iSelectPlottables
//                                    );

    createSceneAndView(ui->customPlot);
    //setupSignalAndSlot();

    setGeometry(400, 250, 542, 390);

    //openJsonFile("/Users/andrewhoo/Work/Qt/Pilot/201602_Throttle/data.json");


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

//    QVector<QCPGraphData> sample_3;
//    QString sample_name_3;
//    quint8 motorIdx_3;
//    generateData(4, sample_3, sample_name_3, motorIdx_3);
//    addGraph(ui->customPlot, sample_3, sample_name_3);

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
void ChartViewerWin::initTrackFinance(QCustomPlot *customPlot, int mouseX)
{
//    vCursor = new QCPItemLine(customPlot);
//    vCursor->setPen(QPen(Qt::black));
//    vCursor->setSelectedPen(QPen(Qt::red));
}

void ChartViewerWin::trackFinance(QCustomPlot *customPlot, int mouseX)
{
//    vCursor->start->setTypeY(QCPItemPosition::ptAxisRectRatio);
//    vCursor->start->setCoords(0, 0);
//    vCursor->end->setTypeY(QCPItemPosition::ptAxisRectRatio);
    //    vCursor->end->setCoords(0, 1);
}

void ChartViewerWin::updateAxisAtBottomRect(QCustomPlot *customPlot)
{
    QCPAxisRect* rect = nullptr;
    for (quint32 idx = 0; idx < customPlot->axisRectCount(); idx++){
        rect = customPlot->axisRect(idx);
        rect->axis(QCPAxis::atBottom)->grid()->setVisible(false);
        rect->axis(QCPAxis::atBottom)->setVisible(false);
    }

    QCPAxisRect* lastRect = customPlot->axisRect(customPlot->axisRectCount() - 1);
    if (lastRect){
        lastRect->axis(QCPAxis::atBottom)->grid()->setVisible(true);
        lastRect->axis(QCPAxis::atBottom)->setVisible(true);
        lastRect->axis(QCPAxis::atBottom)->setLabelFont(QFont("sans", 10));
    }
}

void ChartViewerWin::generateData(quint32 idx, QVector<QCPGraphData>& pairs, QString& name, quint8& motorIdx)
{
    if (cfgRawData)
    {
        if (cfgRawData->type() == CfgWashingTypeEnum::CFGWASHINGTHROTTLE_E2)
        {
            QSharedPointer<CfgThrottleWashingDataE2Clz> dynCast =
                    qSharedPointerDynamicCast<CfgThrottleWashingDataE2Clz>(cfgRawData);

            QVector<CfgItemMeasBasedE2DataEle>& rawData = dynCast->data();

            if (idx < rawData.size())
            {
                name = rawData[0].getName(idx),
                motorIdx = rawData[0].getMotorIdx(idx);

                pairs.resize(rawData.size());
                for (int i = 0; i < rawData.size(); i++)
                {
                    pairs[i].key   = (quint32)rawData[i].getData((quint32)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_THRO_POS);
                    pairs[i].value = rawData[i].getData(idx);
                }
            }
            else
            {
                qWarning() << "index exceed the maximum number of vector. index = " << idx;
            }
        }
        else if (cfgRawData->type() == CfgWashingTypeEnum::CFGWASHINGVOL_E2)
        {
            QSharedPointer<CfgVolWashingDataE2Clz> dynCast =
                    qSharedPointerDynamicCast<CfgVolWashingDataE2Clz>(cfgRawData);

            QVector<CfgItemMeasBasedE2DataEle>& rawData = dynCast->data();

            if (idx < rawData.size())
            {
                name = rawData[0].getName(idx),
                motorIdx = rawData[0].getMotorIdx(idx);

                pairs.resize(rawData.size());
                for (int i = 0; i < rawData.size(); i++)
                {
                    pairs[i].key   = (quint32)rawData[i].getData((quint32)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_VOL_POS);
                    pairs[i].value = rawData[i].getData(idx);
                }
            }
            else
            {
                qWarning() << "index exceed the maximum number of vector. index = " << idx;
            }
        }
        else
        {
            qWarning() << "not support json type!";
        }
    }
}

void ChartViewerWin::addGraph(QCustomPlot *customPlot, QVector<QCPGraphData> &pairs, QString &name, quint8 motorIdx)
{
    QCPAxisRect *rect = new QCPAxisRect(customPlot);
    rect->setMarginGroup(QCP::msLeft | QCP::msRight, marginGroup);

    rect->axis(QCPAxis::atBottom)->grid()->setVisible(true);
    rect->axis(QCPAxis::atBottom)->setVisible(false);
    rect->axis(QCPAxis::atLeft)->setLabelFont(QFont("sans", 10));
    rect->axis(QCPAxis::atLeft)->setLabel(name + " - " + QString::number(motorIdx));
    rect->axis(QCPAxis::atLeft)->setTickLabelFont(QFont("sans", 8));
    //
    rect->setAutoMargins(QCP::msLeft|QCP::msRight|QCP::msBottom|QCP::msTop);
    //rect->setMargins(QMargins(0, 0, 0, 0));
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
    mainGraphCos->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlus, QPen(Qt::black), QBrush(Qt::white), 10));
    QColor color = colorPerTestElement[name];
    mainGraphCos->setPen(QPen(color, 3));
    //mainGraphCos->valueAxis()->setRange(-1, 1);
    mainGraphCos->setName(name + " - " + QString::number(motorIdx));
    mainGraphCos->rescaleAxes();

    updateAxisAtBottomRect(ui->customPlot);
    ui->customPlot->replot();
}

void ChartViewerWin::updateGraph(QCPGraph * graph, QVector<QCPGraphData> &pairs, QString &name, quint8 motorIdx)
{
    if (graph)
    {
        graph->data()->set(pairs);
        QColor color = colorPerTestElement[name];
        graph->setPen(QPen(color, 4));
        //mainGraphCos->valueAxis()->setRange(-1, 1);
        graph->setName(name + " - " + QString::number(motorIdx));
        graph->rescaleAxes();
        ui->customPlot->replot();
    }
}

void ChartViewerWin::setupSignalAndSlot()
{
    // connect some interaction slots:
    connect(ui->customPlot, SIGNAL(axisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)),
            this, SLOT(axisLabelDoubleClick(QCPAxis*,QCPAxis::SelectablePart)));

    connect(ui->customPlot, SIGNAL(mouseMove(QMouseEvent*)), this,SLOT(showVLineItem(QMouseEvent*)));

    // setup policy and connect slot for context menu popup:
    ui->customPlot->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->customPlot, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(contextMenuRequest(QPoint)));
}

void ChartViewerWin::releaseSignalAndSlot()
{
    ui->customPlot->disconnect();
}


void ChartViewerWin::removeAllGraphs()
{
    ui->customPlot->disconnect();
    ui->customPlot->clearGraphs();
    ui->customPlot->plotLayout()->clear();
    //clearGraphsExceptTitle();
    ui->customPlot->replot();
}

//void ChartViewerWin::removeGraph()
//{
//   qDebug() << "ui->customPlot remove ";
//}

/*
 * remove the assoicated graphs and items related to this rect.
*/
void ChartViewerWin::removeGraph()
{
    qDebug() << "ui->customPlot remove " << m_assoRect->axis(QCPAxis::atLeft)->label();


//    ui->customPlot->removeLayer();

    if (m_assoRect)
    {
        if (m_assoRect == ui->customPlot->axisRect(0)
                || 1 == ui->customPlot->axisRectCount())
            return;

        for(QCPGraph* graph : m_assoRect->graphs())
        {
            qDebug() << "ui->customPlot select the rect " << graph->name();
            ui->customPlot->removeGraph(graph);
        }

        ui->customPlot->plotLayout()->remove(m_assoRect);
        ui->customPlot->plotLayout()->simplify();


        //    for(QCPAbstractItem* item : rect->items())
        //    {
        //       ui->customPlot->removeItem(item);
        //    }
        updateAxisAtBottomRect(ui->customPlot);
        ui->customPlot->replot();

        m_assoRect = nullptr;
    }
}

void ChartViewerWin::addGraph()
{
    QVector<QCPGraphData> sample;
    QString sample_name;
    quint8 motorIdx;
    generateData(4, sample, sample_name, motorIdx);
    addGraph(ui->customPlot, sample, sample_name, motorIdx);
}

void ChartViewerWin::clearGraphsExceptTitle()
{
    QCPLayoutGrid* gridLayout = ui->customPlot->plotLayout();
    // we fix the title on the grid (0,0)
    for (int i=gridLayout->elementCount()-1; i>=1; --i)
    {
      if (gridLayout->elementAt(i))
        gridLayout->removeAt(i);
    }
    gridLayout->simplify();
}

void ChartViewerWin::axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part)
{
    //QPoint pos;
    contextMenuRequest(axis);
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
        releaseSignalAndSlot();
        openJsonFile(fileName);
        setupSignalAndSlot();
        ui->customPlot->replot();
    }
}

void ChartViewerWin::showVLineItem(QMouseEvent *event)
{
    if (!ui->customPlot->xAxis || !ui->customPlot->yAxis)
        return;
    int x = ui->customPlot->xAxis->pixelToCoord(event->pos().x());
    int y = ui->customPlot->yAxis->pixelToCoord(event->pos().y());
    double closeX = (double)x;
    static double lastCloseX;
    statusBar()->showMessage(QString("%1 , %2").arg(x).arg(y), 10000);

    QString textTitle;
    // synchronize selection of graphs with selection of corresponding legend items:
    for (int i=0; i<ui->customPlot->graphCount(); ++i)
    {
        QCPGraph *graph = ui->customPlot->graph(i);
        if (!graph) continue;
        QCPGraphDataContainer::const_iterator it = graph->data()->findBegin((double)x);
        closeX = (*it).mainKey();
        double yData = (*it).mainValue();

//        qDebug() << "ui->customePlot i ->" << i <<  "string ->" << graph->name() << "ydata is "<< yData;
        QString formatGraph = QString::asprintf("%-15s : %-10f \t",
                        graph->name().toLatin1().constData(),
                        yData);

//        textTitle += formatGraph;
//                qDebug() << "ui->customePlot string ->" << "ydata is "<< yData;
//                QString formatGraph = QString::asprintf(" : %-10f \t",
//                                yData);

                textTitle += formatGraph;
    }
    //qDebug() << "ui.customPlot -> x " << title;

    if (closeX != lastCloseX){
        lastCloseX = closeX;

        // update the title
        m_title->setText(textTitle);
        // draw the vehicle line
        if(vCursor) ui->customPlot->removeItem(vCursor);
        vCursor = new QCPItemStraightLine(ui->customPlot);
        vCursor->setClipToAxisRect(false);
        vCursor->point1->setCoords(closeX,0);
        vCursor->point2->setCoords(closeX,1);

        vCursor->setPen(QPen(Qt::darkGray));
        vCursor->setSelectedPen(QPen(Qt::red));

        ui->customPlot->replot();
    }
}

void ChartViewerWin::createSceneAndView(QCustomPlot *customPlot)
{
//    DataJsonCfgReader reader;
//    reader.loadData("/Users/andrewhoo/Work/Qt/Pilot/20161205-210133097/data.json");
//    m_scene = new QGraphicsScene(this);
//    m_scene->setSceneRect(0, 0, ui->graphicsView->width(), ui->graphicsView->height());
//    ui->graphicsView->setScene(m_scene);
    //demoName = "Advanced Axes Demo";
    customPlot->legend->setVisible(false);
    marginGroup = new QCPMarginGroup(customPlot);
    customPlot->axisRect()->setMarginGroup(QCP::msLeft|QCP::msRight, marginGroup);

    customPlot->setAutoAddPlottableToLegend(false);

    // bring bottom and main axis rect closer together:
    ui->customPlot->plotLayout()->setRowSpacing(0);
}

void ChartViewerWin::contextMenuRequest(QCPAxisRect* rect)
{
    m_assoRect = rect;

    QAction* action = nullptr;
    QMenu *menu = new QMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);

    action = menu->addAction("Remove all graphs", this, SLOT(removeAllGraphs()));
    action->setCheckable(true);
    action->setData(UINT32_MAX);
    menu->addSeparator();

    action = menu->addAction("Remove this graph", this, SLOT(removeGraph()));
    action->setCheckable(true);
    action->setData(UINT32_MAX);

    menu->addSeparator();

    action = menu->addAction("Add one graph", this, SLOT(addGraph()));
    action->setCheckable(true);
    action->setData(UINT32_MAX);

    menu->addSeparator();
    if (cfgRawData)
    {
        if (cfgRawData->type() == CfgWashingTypeEnum::CFGWASHINGTHROTTLE_E2 ||
                cfgRawData->type() == CfgWashingTypeEnum::CFGWASHINGVOL_E2)
        {
            quint32 idx = 0;
            CfgItemMeasBasedE2DataEle actionlist;
            for (CfgMetaElement& ele : actionlist.m_metaEle)
            {
                quint8 idxMotor = ele.getMotorIdx();
                QString motorString(" ");
                if (idxMotor == 0 || idxMotor == 1)
                    motorString = QString::number(idxMotor + 1);

                const QString str = QString::asprintf("Switch to %-15s\t%5s",
                                ele.getName().toLatin1().constData(),
                                motorString.toLatin1().constData());

                action = menu->addAction(str);
                action->setCheckable(true);
                action->setData(QVariant(idx++));
            }

            QAction *selectedAction = menu->exec(QCursor::pos());
            if (selectedAction)
            {
                quint32 selectIdx = selectedAction->data().toUInt();

                qDebug() << "ui->custormPlot selectIdx = " << selectIdx;
                if (selectIdx <= static_cast<quint32>(DataJsonRecElementE2::ELEMCURSOR::ELEMCURSOR_END))
                {
                    // prepare data:
                    QVector<QCPGraphData> sample;
                    QString sample_name;
                    quint8 motorIdx;
                    generateData(selectIdx, sample, sample_name, motorIdx);
                    //addGraph(ui->customPlot, sample, sample_name);
                    if (rect)
                    {
                        rect->axis(QCPAxis::atLeft)->setLabel(sample_name + " - " + QString::number(motorIdx));
                        for (QCPGraph* graph: rect->graphs())
                        {
                            updateGraph(graph, sample, sample_name, motorIdx);
                        }
                    }
                }
            }
        }
    }
}

void ChartViewerWin::contextMenuRequest(QPoint pos)
{
    contextMenuRequest(ui->customPlot->axisRectAt(pos));
}

void ChartViewerWin::contextMenuRequest(QCPAxis *axis)
{
    contextMenuRequest(axis->axisRect());
}

void ChartViewerWin::initAxesAndView(QCustomPlot *customPlot)
{
    //customPlot->legend->setVisible(true);
    // configure axis rect:
//    customPlot->clearGraphs();
//    customPlot->plotLayout()->clear(); // clear default axis rect so we can start from scratch

    removeAllGraphs();
    // first we create and prepare a text layout element:
    QString text = TestPlanStringMap[(int)cfgMetaData.plan()];

    m_title = new QCPTextElement(ui->customPlot);
    m_title->setMarginGroup(QCP::msLeft | QCP::msRight, marginGroup);
    m_title->setText(text);
    m_title->setTextFlags(Qt::AlignLeft | Qt::TextWordWrap);
    m_title->setFont(QFont("sans", 11, QFont::Medium));
    // then we add it to the main plot layout:
    customPlot->plotLayout()->insertRow(0); // insert an empty row above the axis rect
    customPlot->plotLayout()->addElement(0, 0, m_title); // place the title in the empty cell we've just created
}
