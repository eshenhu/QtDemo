#include "chartviewerwin.h"
#include "ui_chartviewerwin.h"

#include "ui/uiheader.h"
#include <QGraphicsLinearLayout>
#include <QLineSeries>
#include <QValueAxis>
#include "cfg/datajsoncfgreader.h"
#include "cfg/cfgjsonprimaryelement.h"

ChartViewerWin::ChartViewerWin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChartViewerWin),
    cfgElementList((quint32)ChartViewerWin::ChartViewIdxSupFileE::IDX_SUP_FILE_MAX),
    cfgMetaData(cfgElementList[(quint32)ChartViewerWin::ChartViewIdxSupFileE::IDX_SUP_FILE_I].cfgMetaData),
    cfgRawData(cfgElementList[(quint32)ChartViewerWin::ChartViewIdxSupFileE::IDX_SUP_FILE_I].cfgRawData)
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
    delete ui;
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
        for (ChartViewerWin::ChartViewCfgElement& ele : cfgElementList)
        {
            ele.reset();
        }

        if (openJsonFile(fileName, (quint32)ChartViewerWin::ChartViewIdxSupFileE::IDX_SUP_FILE_I))
        {
            releaseSignalAndSlot();
            initAxesAndView(ui->customPlot);
            loadDefault2Plot();
            setupSignalAndSlot();

            emit testPlanChanged(cfgMetaData.plan());
            // inform this information
            //emit leftFileOk(true);
        }
        ui->customPlot->replot();
    }
}

/*
 * We only allow compare the files with same 'test plan' and 'pv';
*/
bool ChartViewerWin::validateMetaData()
{
    bool rtn = true;
    CfgResHandlerInf::ProductVersion pv = cfgMetaData.pv();
    TestPlanEnum plan = cfgMetaData.plan();

    for (ChartViewerWin::ChartViewCfgElement& ele : cfgElementList)
    {
        if ( pv != ele.cfgMetaData.pv() || plan != ele.cfgMetaData.plan())
        {
            rtn = false;
            break;
        }
    }
    return rtn;
}

void ChartViewerWin::open_validate()
{
    QString path = QCoreApplication::applicationDirPath();
    const QString fileName = QFileDialog::getOpenFileName(this,
                                                          tr("Open Json File"),
                                                          path,
                                                          tr("Json Files (*.json)"));

    if (!fileName.isEmpty())
    {
        if (openJsonFile(fileName, (quint32)ChartViewerWin::ChartViewIdxSupFileE::IDX_SUP_FILE_II))
        {
            if (validateMetaData())
            {
                releaseSignalAndSlot();
                initAxesAndView(ui->customPlot);
                loadDefault2Plot();
                setupSignalAndSlot();
            }
            else
            {
                quint32 idx = (quint32)ChartViewerWin::ChartViewIdxSupFileE::IDX_SUP_FILE_II;
                ChartViewerWin::ChartViewCfgElement ele = cfgElementList.value(idx);
                ele.reset();

                QMessageBox warningBox(QMessageBox::Warning, tr("Warning"),
                                       tr("Those two files was not compitable  \n\
                                           - Test Plan    \n\
                                           - Product Version "),
                                       QMessageBox::Close);
                warningBox.exec();
            }
        }
        ui->customPlot->replot();
    }
}
/* 1 : OK
*  0 : Fail
*/

#define RETURN_WITH_OK   {isOk = true;  break;}
#define RETURN_WITH_NOK  {isOk = false; break;}

bool ChartViewerWin::openJsonFile(const QString& jsonFileName, quint32 location)
{
    bool isOk = false;

    while (location < (quint32)ChartViewerWin::ChartViewIdxSupFileE::IDX_SUP_FILE_MAX)
    {

        DataJsonCfgReader reader;

        if (!reader.loadData(jsonFileName))
        {
            QMessageBox warningBox(QMessageBox::Warning, tr("Warning"),
                                   tr("This file can't be read with reason of either \n"
                                      " - json file format was corrupted. \n"
                                      " - data file was corrupted"),
                                   QMessageBox::Close);
            warningBox.exec();
            RETURN_WITH_NOK
        }

        cfgElementList[location].cfgMetaData = reader.getCfgParser();
        cfgElementList[location].cfgRawData = reader.csvDataHandler();

        RETURN_WITH_OK
    }
    return isOk;
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
    QToolBar *fileToolBar = addToolBar(tr("File_A"));

    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/ui/ui/open.png"));
    openAct = new QAction(openIcon, tr("&Open..."), this);
    //openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &ChartViewerWin::open);
    fileToolBar->addAction(openAct);

    openAct->setEnabled(true);

    /*------ add openAct_B -------*/
    QToolBar *fileToolBar_B = addToolBar(tr("File_B"));

    const QIcon openIcon_B = QIcon::fromTheme("document-open", QIcon(":/ui/ui/open.png"));
    openAct_B = new QAction(openIcon_B, tr("&Open..."), this);
    //openAct->setShortcuts(QKeySequence::Open);
    openAct_B->setStatusTip(tr("Open an existing file"));
    connect(openAct_B, &QAction::triggered, this, &ChartViewerWin::open_validate);
    fileToolBar_B->addAction(openAct_B);

    openAct_B->setVisible(true);
    openAct_B->setEnabled(false);

    //connect(this, &ChartViewerWin::leftFileOk, openAct_B, &QAction::setEnabled);

    /*------ add open -------*/
    QToolBar *curveToolBar = addToolBar(tr("Curve"));
    curveToolBar->addSeparator();

    QComboBox* curveComBox = new QComboBox();
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

    curveAction = curveToolBar->addWidget(curveComBox);

    curveAction->setVisible(true);
    curveAction->setEnabled(false);
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
        lastRect->axis(QCPAxis::atBottom)->setLabelFont(QFont("sans", 10, QFont::Bold));
    }
}

void ChartViewerWin::generateData(QSharedPointer<CfgWashingDataInf> cfgRawData, quint32 idx,
                                  QVector<QCPGraphData>& pairs, QString& name, quint8& motorIdx)
{
    if (cfgRawData)
    {
        if (cfgRawData->type() == CfgWashingTypeEnum::CFGWASHINGTHROTTLE_E2 ||
            cfgRawData->type() == CfgWashingTypeEnum::CFGWASHINGVOL_E2 ||
            cfgRawData->type() == CfgWashingTypeEnum::CFGWASHINGMULTI_E2)
        {
            cfgRawData->generateData(idx, pairs, name, motorIdx);
        }
        else
        {
            qWarning() << "not support json type!";
        }
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
    rect->setMarginGroup(QCP::msLeft | QCP::msRight, marginGroup);

    rect->axis(QCPAxis::atBottom)->grid()->setVisible(true);
    rect->axis(QCPAxis::atBottom)->setVisible(false);
    rect->axis(QCPAxis::atLeft)->setLabelFont(QFont("sans", 10));
    //rect->axis(QCPAxis::atLeft)->setLabel(name + " - " + QString::number(motorIdx));
    rect->axis(QCPAxis::atLeft)->setTickLabelFont(QFont("sans", 8));
    //
    rect->setAutoMargins(QCP::msLeft|QCP::msRight|QCP::msBottom|QCP::msTop);
    //rect->setMargins(QMargins(0, 0, 0, 0));
    foreach (QCPAxis *axis, rect->axes())
    {
        axis->setLayer("axes");
        axis->grid()->setLayer("grid");
    }
    //updateAxisAtBottomRect(ui->customPlot);
    customPlot->plotLayout()->addElement(rect);
    //customPlot->replot();
    return rect;
}

QCPGraph* ChartViewerWin::addGraph(QCPAxisRect* rect)
{
    // create and configure plottables:
    QCPGraph *mainGraphCos = ui->customPlot->addGraph(rect->axis(QCPAxis::atBottom), rect->axis(QCPAxis::atLeft));
    return mainGraphCos;
}

void ChartViewerWin::updateGraph(QCPGraph* graph, QVector<QCPGraphData> &pairs, QString &name, quint8 motorIdx)
{
    if (graph)
    {
        QString displayStr = name.split(':').value(1, "");
        graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlus, QPen(Qt::black), QBrush(Qt::white), 10));
        graph->setAntialiasedFill(true);
        graph->data()->set(pairs);
        QColor color = colorPerTestElement[displayStr];
        graph->setPen(QPen(color, 2));
        //mainGraphCos->valueAxis()->setRange(-1, 1);
        graph->setName(displayStr + ": " + QString::number(motorIdx));
        graph->rescaleValueAxis();
        graph->rescaleAxes();
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
    quint32 idx = 0;
    QCPGraph* graph = nullptr;

    QString sample_name;
    quint8 motorIdx;

    for (ChartViewCfgElement& ele : cfgElementList)
    {
        if(!ele.cfgRawData.isNull())
        {
            //graph = rect->graphs()[idx];
            graph = rect->graphs().value(idx, nullptr);
            if (!graph)
            {
                graph = addGraph(rect);
            }
            // prepare data:
            QVector<QCPGraphData> pair;
            generateData(ele.cfgRawData, selectedIdx, pair, sample_name, motorIdx);
            updateGraph(graph, pair, sample_name, motorIdx);

            idx++;
        }
    }
    // update the rect name;
    rect->axis(QCPAxis::atLeft)->setLabel(sample_name + ": " + QString::number(motorIdx));

    ui->customPlot->replot();
}


void ChartViewerWin::setupSignalAndSlot()
{
    connect(this, &ChartViewerWin::testPlanChanged, [this](TestPlanEnum plan){
        openAct_B->setEnabled(false);
        curveAction->setEnabled(false);

        if (plan == TestPlanEnum::Voltage ||
            plan == TestPlanEnum::Throttle)
        {
            openAct_B->setEnabled(true);
            itemListIdx = 0;
        }
        else if (plan == TestPlanEnum::Multiplue)
        {
            curveAction->setEnabled(true);
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

quint32 ChartViewerWin::setupIdxMultipleTest(quint32 idx)
{
    return (itemListIdx << 16 | idx);
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
    int x = ui->customPlot->xAxis->pixelToCoord(event->pos().x());
    int y = ui->customPlot->yAxis->pixelToCoord(event->pos().y());
    double closeX = (double)x;
    double closeXX = (double)x;
    static double lastCloseX;
    statusBar()->showMessage(QString("%1 , %2").arg(x).arg(y), 1000);

    QString textTitle;

    QCPGraph *graph = ui->customPlot->graph(0);
    if (!graph)
        return;
    QCPGraphDataContainer::const_iterator it = graph->data()->findBegin((double)x);
    closeX = (*it).mainKey();

    if (++it != graph->data()->constEnd())
    {
        closeXX = (*it).mainKey();
    }

    double midX = (closeX + closeXX + 0.5) / 2;
    if (x > midX)
    {
        closeX = closeXX;
    }
    //qDebug() << "ui.customPlot -> x " << title;

    if (closeX != lastCloseX){
        lastCloseX = closeX;

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

            static char buffer[100];
            sprintf(buffer, "%s : %s%s%s",
                    "<small>%-15s</small> ", "<b><big><i>", formatData, "</i></big></b>   ");


            QString formatGraph = QString::asprintf(buffer,
                                                    graph->name().toLatin1().constData(),
                                                    yData);

            textTitle += formatGraph;
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

                if (false == (!matchWord.isEmpty() && ele.getName() == matchWord))
                    action = menu->addAction(cxtHeader + str);

                action->setCheckable(true);
                action->setData(QVariant(idx++));
            }
        }
        else if (cfgRawData->type() == CfgWashingTypeEnum::CFGWASHINGMULTI_E2)
        {
            QList<QString>& guiList = cfgRawData->getGUIActionList(itemListIdx);

            quint32 idx = 0;
            for (const QString& str : guiList)
            {
                QString cxtHeader("   ");
                if (fullLabelName == str)
                    cxtHeader = QStringLiteral("[*]");

                action = menu->addAction(cxtHeader + str);
                action->setCheckable(true);
                action->setData(QVariant(idx++));
            }
        }

        QAction *selectedAction = menu->exec(QCursor::pos());
        if (selectedAction)
        {
            quint32 selectIdx = selectedAction->data().toUInt();

            qDebug() << "ui->custormPlot selectIdx = " << selectIdx;
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
    QString text = TestPlanStringMap[(int)cfgMetaData.plan()];
    setWindowTitle(text);
}

void ChartViewerWin::showText()
{
    ui->textEdit->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    //ui->textEdit->setText("<b>Hello</b> <i>Qt!</i>");
    ui->textEdit->setText("<b><big><i>Welcome to the graph!</i></big></b> ");
}


