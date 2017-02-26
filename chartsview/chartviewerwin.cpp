#include "chartviewerwin.h"
#include "ui_chartviewerwin.h"

#include "ui/uiheader.h"
#include <memory>
#include <QGraphicsLinearLayout>
#include <QLineSeries>
#include <QValueAxis>
#include "cfg/datajsoncfgreader.h"
#include "cfg/cfgjsonprimaryelement.h"
#include "ui/fileselectdialog.h"

ChartViewerWin::ChartViewerWin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChartViewerWin),
    cfgElementList((quint32)ChartViewerWin::ChartViewIdxSupFileE::IDX_SUP_FILE_MAX),
    cfgRawData(nullptr)
{
    ui->setupUi(this);

    createActions();

//    ui->customPlot->setInteractions(
//                QCP::iSelectAxes
//                                    | QCP::iSelectLegend
//                                    | QCP::iSelectPlottables
//                                    );

    createSceneAndView(ui->customPlot);
    //setupSignalAndSlot();

    setGeometry(400, 250, 542, 390);
    //createWidgets();
    showText();
}

ChartViewerWin::~ChartViewerWin()
{
    m_mapFromAxisToLegend.clear();
    delete ui;
}

void ChartViewerWin::callbackActionOpen2Cmp()
{
    std::unique_ptr<FileSelectDialog> fileSelection(new FileSelectDialog(this));
    fileSelection->exec();
    cfgElementList = std::move(fileSelection->cfgElementList);
    cfgMetaData = cfgElementList[0].cfgMetaData;
    cfgRawData = cfgElementList[0].cfgRawData;

    quint32 countOfGraph = 0;
    for (const ChartViewCfgElement& ele : cfgElementList)
    {
        if (!ele.cfgRawData.isNull())
            ++countOfGraph;
    }

    m_isCmpEnabled = false;
    if (countOfGraph == 0)
    {
        qWarning() << "No Json File was selected!";
        return;
    }
    else if (countOfGraph > 1)
        m_isCmpEnabled = true;

    releaseSignalAndSlot();
    initAxesAndView(ui->customPlot);
    loadDefault2Plot();
    setupSignalAndSlot();

    emit testPlanChanged(cfgMetaData.plan());

    ui->customPlot->replot();
}

void ChartViewerWin::callbackOnActionExportPDF()
{
    QString savePDFFileName = QFileDialog::getSaveFileName(
                this, tr("Save File"), "/home/", tr("PDF (*.pdf)"));

    if (savePDFFileName.isNull())
        return;

    ui->customPlot->savePdf(savePDFFileName);
}

void ChartViewerWin::callbackOnActionExportCSV()
{
    QString saveCSVFileName = QFileDialog::getSaveFileName(
                this, tr("Save File"), "/home/", tr("CSV (*.csv)"));

    if (saveCSVFileName.isNull())
        return;

    saveCSV(saveCSVFileName);
}

bool ChartViewerWin::loadDefault2Plot()
{
    QCPAxisRect* rect;

    rect = addRect(ui->customPlot);
    updateGraph(rect, setupIdxMultipleTest(3));

    rect = addRect(ui->customPlot);
    updateGraph(rect, setupIdxMultipleTest(3));

    return true;
}

void ChartViewerWin::createActions()
{
    /*------ add openAct -------*/
    {
        QToolBar *fileToolBar = addToolBar(tr("File_A"));

        const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/ui/ui/open.png"));
        actionOpen = new QAction(openIcon, tr("&Open..."), this);
        //openAct->setShortcuts(QKeySequence::Open);
        actionOpen->setStatusTip(tr("Open an existing file"));
        connect(actionOpen, &QAction::triggered, this, &ChartViewerWin::callbackActionOpen2Cmp);
        fileToolBar->addAction(actionOpen);

        actionOpen->setEnabled(true);
    }

    /* add OPEN */
    {
        /*------ add open -------*/
        QToolBar *curveToolBar = addToolBar(tr("Curve"));
        curveToolBar->addSeparator();

        QComboBox* curveComBox = new QComboBox();
        curveComBox->addItem(tr("throttle character curve"));
        curveComBox->addItem(tr("current character curve"));
        curveComBox->addItem(tr("thrust character curve"));
        curveComBox->addItem(tr("torque character curve"));
        curveComBox->addItem(tr("speed character curve"));
        curveComBox->setCurrentIndex(0);

        connect(curveComBox,
                static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                [=](int index){
            static int lastCurveComBoxIdx = -1;
            if (index != lastCurveComBoxIdx)
            {
                lastCurveComBoxIdx = index;
                //            quint32 startIdx = CfgMultiWashingDataE2Clz::getBasedIdx(index);
                updateGraphDuringMultiplePlan(index);
            }
        });

        actionCurve = curveToolBar->addWidget(curveComBox);

        actionCurve->setVisible(true);
        actionCurve->setEnabled(false);
    }

    /* Add Export to PDF */
    {
        QToolBar *toolBarExportPDF = addToolBar(tr("ExportPDF"));
        toolBarExportPDF->addSeparator();

        const QIcon iconExportPDF = QIcon::fromTheme("document-open", QIcon(":/ui/ui/ExportPDF.png"));
        actionExportPDF = new QAction(iconExportPDF, tr("&ExportPDF..."), this);
        actionExportPDF->setStatusTip(tr("Export to PDF format"));
        connect(actionExportPDF, &QAction::triggered, this, &ChartViewerWin::callbackOnActionExportPDF);
        toolBarExportPDF->addAction(actionExportPDF);

        toolBarExportPDF->setEnabled(true);

    }
    /* Add Export to CSV */
    {
        QToolBar *toolBarExportCSV = addToolBar(tr("ExportCSV"));
        toolBarExportCSV->addSeparator();

        const QIcon iconExportCSV = QIcon::fromTheme("document-open", QIcon(":/ui/ui/ExportCSV.png"));
        actionExportCSV = new QAction(iconExportCSV, tr("&ExportCSV..."), this);
        actionExportCSV->setStatusTip(tr("Export to CSV format"));
        connect(actionExportCSV, &QAction::triggered, this, &ChartViewerWin::callbackOnActionExportCSV);
        toolBarExportCSV->addAction(actionExportCSV);

        toolBarExportCSV->setEnabled(true);
    }

}

void ChartViewerWin::updateAxisAtBottomRect(QCustomPlot *customPlot)
{
    for (const QCPAxisRect* rect : customPlot->axisRects())
    {
        rect->axis(QCPAxis::atBottom)->setVisible(true);
        rect->axis(QCPAxis::atBottom)->grid()->setVisible(true);
    }

    customPlot->axisRects().constLast()->axis(QCPAxis::atBottom)->setVisible(true);
}

void ChartViewerWin::generateData(QSharedPointer<CfgWashingDataInf> cfgRawData, quint32 idx,
                                  QVector<QCPGraphData>& pairs, QString& name, quint8& motorIdx)
{
    if (cfgRawData)
    {
        cfgRawData->generateData(idx, pairs, name, motorIdx);
    }
}

void ChartViewerWin::addRect()
{
    QCPAxisRect* rect = addRect(ui->customPlot);
    updateGraph(rect, setupIdxMultipleTest(2));
}

QCPAxisRect* ChartViewerWin::addRect(QCustomPlot *customPlot)
{
    QCPAxisRect *rect = new QCPAxisRect(customPlot);

    foreach (QCPAxis *axis, rect->axes())
    {
        axis->setLayer("axes");
        axis->grid()->setLayer("grid");
    }

    rect->setMarginGroup(QCP::msLeft | QCP::msRight, marginGroup); 
    rect->axis(QCPAxis::atBottom)->grid()->setVisible(true);
    rect->axis(QCPAxis::atBottom)->setVisible(false);
    rect->axis(QCPAxis::atBottom)->setLabelFont(QFont("Dotum", 9));
    //rect->axis(QCPAxis::atBottom)->setLabel(name + " - " + QString::number(motorIdx));

    rect->axis(QCPAxis::atLeft)->setLabelFont(QFont("Dotum", 10));
    //rect->axis(QCPAxis::atLeft)->setLabel(name + " - " + QString::number(motorIdx));
    rect->axis(QCPAxis::atLeft)->setTickLabelFont(QFont("Dotum", 8));
    //
    rect->setAutoMargins(QCP::msLeft|QCP::msRight|QCP::msBottom|QCP::msTop);
    //rect->setMargins(QMargins(0, 0, 0, 0));

    /* add QCPAxisRect to the QMAP */
    m_mapFromAxisToLegend.insert(rect, setupLegend(rect));

    //updateAxisAtBottomRect(ui->customPlot);
    customPlot->plotLayout()->addElement(rect);
    customPlot->plotLayout()->simplify();
    //customPlot->replot();
    return rect;
}

/*
 * Get the (lowest, highest) range from all the graph.
*/
void ChartViewerWin::rescalePlot()
{
    double lowest = std::numeric_limits<double>::max();
    double highest = 1;

    for (const QCPAxisRect* rect : ui->customPlot->axisRects())
    {
        const QCPRange range = rect->axis(QCPAxis::atBottom)->range();
        if (lowest > range.lower)
            lowest = range.lower;
        if (highest < range.upper)
            highest = range.upper;
    }

    for (const QCPAxisRect* rect : ui->customPlot->axisRects())
    {
        const QCPRange range = rect->axis(QCPAxis::atBottom)->range();
        if (lowest != range.lower || highest != range.upper)
        {
            rect->axis(QCPAxis::atBottom)->setRange(lowest, highest);
        }
    }
}

QCPGraph* ChartViewerWin::addGraph(QCPAxisRect* rect)
{
    // create and configure plottables:
    QCPGraph *mainGraphCos = ui->customPlot->addGraph(rect->axis(QCPAxis::atBottom), rect->axis(QCPAxis::atLeft));
    //m_mapFromAxisToLegend[rect]->addItem(mainGraphCos);
    mainGraphCos->addToLegend(m_mapFromAxisToLegend[rect]);
    return mainGraphCos;
}

void ChartViewerWin::updateGraph(QCPGraph* graph, QVector<QCPGraphData> &pairs, QString &name, quint8 motorIdx, quint32 idxOfGraph)
{
    if (graph)
    {
        QString displayStr = name.split(':').value(1, "");
        graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlus, QPen(Qt::black), QBrush(Qt::white), 10));
        graph->setAntialiasedFill(true);
        graph->data()->set(pairs);

        QColor color = TestUnitColor::getColor(displayStr, idxOfGraph, m_isCmpEnabled);
        graph->setPen(QPen(color, 2));
        //mainGraphCos->valueAxis()->setRange(-1, 1);
        graph->setName(displayStr + ":" + QString::number(motorIdx));
        //graph->rescaleValueAxis(true);
        graph->rescaleAxes(false);
        updateAxisAtBottomRect(ui->customPlot);
        ui->customPlot->replot();
    }
}

void ChartViewerWin::updateGraphDuringMultiplePlan(quint32 baseIdx)
{
//    removeAllGraphs();
    for (QCPAxisRect* rect : ui->customPlot->axisRects())
    {
        m_assoRect = rect;
        removeGraph();
    }
    itemListIdx = baseIdx;

    updateGraph(ui->customPlot->axisRect(0), setupIdxMultipleTest(0));
}

void ChartViewerWin::updateGraph(QCPAxisRect* rect, quint32 selectedIdx)
{
    quint32 idxOfGraph = 0;
    QCPGraph* graph = nullptr;

    QString sample_name;
    quint8 motorIdx = 0;

    for (ChartViewCfgElement& ele : cfgElementList)
    {
        if(!ele.cfgRawData.isNull())
        {
            //graph = rect->graphs()[idx];
            graph = rect->graphs().value(idxOfGraph, nullptr);
            if (!graph)
            {
                graph = addGraph(rect);  
            }
            // prepare data:
            QVector<QCPGraphData> pair;
            generateData(ele.cfgRawData, selectedIdx, pair, sample_name, motorIdx);
            updateGraph(graph, pair, sample_name, motorIdx, idxOfGraph);
            rescalePlot();
            idxOfGraph++;
        }
    }
    // update the rect name;
    rect->axis(QCPAxis::atLeft)->setLabel(sample_name + ":" + QString::number(motorIdx));

    QString xAxisName = sample_name.split(':').value(0);
    rect->axis(QCPAxis::atBottom)->setLabel(xAxisName);
    //rect->axis(QCPAxis::atBottom)->setVisible(false);

    ui->customPlot->replot();
}


void ChartViewerWin::setupSignalAndSlot()
{
    connect(this, &ChartViewerWin::testPlanChanged, [this](TestPlanEnum plan){
        actionCurve->setEnabled(false);

        if (plan == TestPlanEnum::Multiplue)
        {
            actionCurve->setEnabled(true);
            itemListIdx = 0;
        }
        else
        {
            itemListIdx = 0;
        }
    });

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

QCPLegend *ChartViewerWin::setupLegend(const QCPAxisRect * rect)
{
    QCPLegend* legend = new QCPLegend;
    legend->setVisible(true);

    rect->insetLayout()->addElement(legend, Qt::AlignRight|Qt::AlignTop);
    rect->insetLayout()->setMargins(QMargins(12, 12, 12, 12));

    legend->setLayer(QLatin1String("legend"));
    return legend;
}

quint32 ChartViewerWin::setupIdxMultipleTest(quint32 idx)
{
    return (itemListIdx << 16 | idx);
}

void ChartViewerWin::saveCSV(const QString& filename)
{
    quint32 idx = 0;
    for (const ChartViewCfgElement& ele : cfgElementList)
    {
        if (ele.cfgCsvFullPathName.isEmpty())
            break;

        QString leftFileName = filename.left(filename.size() - 4);
        QStringList saveNameList({leftFileName, TestPlanStringMap[ele.cfgMetaData.plan()], QString::number(idx)});
        QString saveName = saveNameList.join('_') + QStringLiteral(".csv");

        /* we simplely copy the src data file to here exclude the Multiplue test*/
        if (ele.cfgMetaData.plan() != TestPlanEnum::Multiplue)
        {
            DataJsonRecElement::DataJsonRecElementFileReaderHandler handler;
            handler.loadData(ele.cfgCsvFullPathName, saveName);
        }
        else
        {
            ele.cfgRawData->dump(saveName);
        }
        ++idx;
    }
}

//void ChartViewerWin::clearGraphsExceptTitle()
//{
//    QCPLayoutGrid* gridLayout = ui->customPlot->plotLayout();
//    // we fix the title on the grid (0,0)
//    for (int i=gridLayout->elementCount()-1; i>=1; --i)
//    {
//      if (gridLayout->elementAt(i))
//        gridLayout->removeAt(i);
//    }
//    gridLayout->simplify();
//}


void ChartViewerWin::removeAllGraphs()
{
    showText();
    emit testPlanChanged(TestPlanEnum::Invaild);
    ui->customPlot->disconnect();
    ui->customPlot->clearGraphs();
    ui->customPlot->plotLayout()->clear();
    //clearGraphsExceptTitle();
    ui->customPlot->replot();
}

/*
 * remove the assoicated graphs and items related to this rect.
*/
void ChartViewerWin::removeGraph()
{
    qDebug() << "ui->customPlot remove " << m_assoRect->axis(QCPAxis::atLeft)->label();

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

        delete m_mapFromAxisToLegend[m_assoRect];
        m_mapFromAxisToLegend.remove(m_assoRect);

        ui->customPlot->plotLayout()->remove(m_assoRect);
        ui->customPlot->plotLayout()->simplify();

        updateAxisAtBottomRect(ui->customPlot);
        ui->customPlot->replot();

        m_assoRect = nullptr;
    }
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

void ChartViewerWin::showVLineItem(QMouseEvent *event)
{
    if (!ui->customPlot->xAxis || !ui->customPlot->yAxis)
        return;
    double x = ui->customPlot->xAxis->pixelToCoord(event->pos().x());
    double y = ui->customPlot->yAxis->pixelToCoord(event->pos().y());
    double closeX = (double)x;
    double closeXX = (double)x;
    static double lastCloseX;
    statusBar()->showMessage(QString("%1 , %2").arg(x).arg(y), 1000);

    QCPGraph *graph = ui->customPlot->graph(0);
    if (!graph)
        return;
    QCPGraphDataContainer::const_iterator it = graph->data()->findBegin((double)x);
    closeX = (*it).mainKey();

    if (++it != graph->data()->constEnd())
    {
        closeXX = (*it).mainKey();
    }
    else
    {
        closeXX = closeX;
    }

    double midX = (closeX + closeXX) / 2;
    if (x > midX)
    {
        closeX = closeXX;
    }
    //

    if (closeX != lastCloseX){
        lastCloseX = closeX;

        static char buffer[100];
        sprintf(buffer, "%s : %s",
                "<small>%-15s</small> ", "<b><big><i> %10.2f </i></big></b> ");

        QString textTitle = QString::asprintf(buffer,
            ui->customPlot->axisRect(0)->axis(QCPAxis::atBottom)->label().toLatin1().constData(),
            closeX);

        //qDebug() << "ui.customPlot -> x " << x << closeX;
        // synchronize selection of graphs with selection of corresponding legend items:
        for (int i=0; i<ui->customPlot->graphCount(); ++i)
        {
            QCPGraph *graph = ui->customPlot->graph(i);
            if (!graph) continue;
            QCPGraphDataContainer::const_iterator it = graph->data()->findBegin((double)closeX);
            //closeX = (*it).mainKey();
            double yData = (*it).mainValue();

            //qDebug() << "ui->customePlot i ->" << i <<  "string ->" << graph->name() << "ydata is "<< yData;

            JsonGUIPrimType guiType = JsonGUIElement::lookup(graph->name().split(':').at(0));
            const char* formatData = JsonGUIElement::format(guiType);

//            static char buffer[100];
            sprintf(buffer, "%s : %s%s%s",
                    "<small>%-15s</small> ", "<b><big><i>", formatData, "</i></big></b>   ");


            QString formatGraph = QString::asprintf(buffer,
                                                    graph->name().toLatin1().constData(),
                                                    yData);

            textTitle += formatGraph;
//            {
//                // add label for phase tracer:
//                static QCPItemText *phaseTracerText = new QCPItemText(ui->customPlot);
//                phaseTracerText->position->setType(QCPItemPosition::ptAxisRectRatio);
//                phaseTracerText->setPositionAlignment(Qt::AlignRight|Qt::AlignBottom);
//                phaseTracerText->position->setCoords( 1, 1); // lower right corner of axis rect
//                phaseTracerText->setText("Points of fixed\nphase define\nphase velocity vp");
//                phaseTracerText->setTextAlignment(Qt::AlignLeft);
//                phaseTracerText->setFont(QFont(font().family(), 9));
//                phaseTracerText->setPadding(QMargins(8, 0, 0, 0));

//            }
        }

        // update the title
        //m_title->setText(textTitle);
        ui->textEdit->setText(textTitle);
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

    QString fullLabelName = rect->axis(QCPAxis::atLeft)->label();
    QString labelName = rect->axis(QCPAxis::atLeft)->label().split(':').at(0);
    QString motorIdxString = rect->axis(QCPAxis::atLeft)->label().split(':').at(1);
    quint32 motorIdxInt = motorIdxString.toInt();

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

    action = menu->addAction("Add one graph", this, SLOT(addRect()));
    action->setCheckable(true);
    action->setData(UINT32_MAX);

    menu->addSeparator();
    if (cfgRawData)
    {
        if (cfgRawData->type() == CfgWashingTypeEnum::CFGWASHINGMULTI_E2 ||
            cfgRawData->type() == CfgWashingTypeEnum::CFGWASHINGMULTI_E1)
        {
            QVector<CfgMetaElement>& guiList = cfgRawData->getGUIActionList(itemListIdx);

            quint32 idx = 0;
            for (const CfgMetaElement& ele : guiList)
            {
                QString cxtHeader("   ");
                const QStringList list({ele.getName(), QString::number(ele.getMotorIdx())});
                const QString fullName = list.join(':');
                if (fullLabelName == fullName)
                    cxtHeader = QStringLiteral("[*]");

                action = menu->addAction(cxtHeader + fullName);
                action->setCheckable(true);
                action->setData(QVariant(idx++));
            }
        }
        else
        {
            quint32 idx = 0;
            for (const CfgMetaElement& ele : cfgRawData->getGUIActionList(itemListIdx))
            {
                quint8 idxMotor = ele.getMotorIdx();
                QString motorString(" ");
                if (idxMotor == 0 || idxMotor == 1)
                    motorString = QString::number(idxMotor + 1);

                QString cxtHeader("   ");
                if (labelName == ele.getName() && idxMotor == motorIdxInt)
                    cxtHeader = QStringLiteral("[*]");

                const QString str = QString::asprintf("Switch to %-15s\t%5s",
                                                      ele.getName().toLatin1().constData(),
                                                      motorString.toLatin1().constData());

                QString matchWord;
                if (cfgMetaData.plan() == TestPlanEnum::Voltage)
                    matchWord = TestUnitName::VOL();
                else if (cfgMetaData.plan() == TestPlanEnum::Throttle)
                    matchWord = TestUnitName::THROTTLE();
                else if (cfgMetaData.plan() == TestPlanEnum::Distance)
                    matchWord = TestUnitName::DISTANCE();

                if (false == (!matchWord.isEmpty() && ele.getName() == matchWord))
                    action = menu->addAction(cxtHeader + str);

                action->setCheckable(true);
                action->setData(QVariant(idx++));
            }
        }

        QAction *selectedAction = menu->exec(QCursor::pos());
        if (selectedAction)
        {
            quint32 selectIdx = selectedAction->data().toUInt();

            //qDebug() << "ui->custormPlot selectIdx = " << selectIdx;
            //if (selectIdx <= static_cast<quint32>(DataJsonRecElementE2::ELEMCURSOR::ELEMCURSOR_END))
            if (selectIdx != UINT32_MAX)
            {
                updateGraph(rect, setupIdxMultipleTest(selectIdx));
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
    removeAllGraphs();

    statusBar()->clearMessage();

    // first we create and prepare a text layout element:
    QString text = TestPlanStringMap[(int)cfgMetaData.plan()] + QStringLiteral(" Test");
    setWindowTitle(text);
}

void ChartViewerWin::showText()
{
    ui->textEdit->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    //ui->textEdit->setText("<b>Hello</b> <i>Qt!</i>");
    ui->textEdit->setText("<b><big><i>Welcome to the graph!</i></big></b> ");
}
