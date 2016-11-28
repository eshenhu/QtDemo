#include "compqchartwidget.h"
#include "cfg/cfgjsonprimaryelement.h"
#include "ui/qextcheckbox.h"
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

#include <QtGlobal>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChart>

#include <QDebug>
#include <QMargins>
#include "ui/qrtlineseries.h"
#include "comm/qmodbusdataunit.h"
#include "cfg/datajsonrecelement.h"
#include "ui/qcxtchart.h"

QT_CHARTS_USE_NAMESPACE

CompQChartWidget::CompQChartWidget(const CfgJsonReader* reader, QWidget *parent) :
    QWidget(parent),
    m_reader(reader)
{
    m_layout = new QHBoxLayout;
    //m_lhsLayout = new QVBoxLayout;
    m_lhsLayout = new QGridLayout;
    m_lhsLayout->setHorizontalSpacing(0);
    m_lhsLayout->setVerticalSpacing(0);
    m_lhsLayout->setSizeConstraint(QLayout::SetMaximumSize);
    //m_lhsLayout->setMargin(0);
    m_lhsLayout->setContentsMargins(0,0,0,0);
    m_rhsLayout = new QVBoxLayout;

    createCheckBoxView();
    createChartsView();

    m_layout->addLayout(m_lhsLayout, 1);
    m_layout->addLayout(m_rhsLayout, 0);

    setLayout(m_layout);
}

void CompQChartWidget::updateData(const QModbus2DataUnit *data)
{
    const QVector<QExtCheckBox *>& checkboxList = QExtCheckBox::qExtSpinBoxList();
    foreach (QExtCheckBox* box, checkboxList)
    {
        box->update(data);
    }

    foreach(const QChartView* chartView ,m_chartsViewVector){
        QCxtChart* chart = static_cast<QCxtChart*>(chartView->chart());
        chart->updateCharts();
    }

    /*
     * Record the log.
     */
    DataJsonRecElementE2& e2 = DataJsonRecElementE2::DataJsonRecElementE2GetHelper().getElem();

    quint32 startIdx = static_cast<quint32>(DataJsonRecElementE2::ELEMCURSOR::REC_VOL_POS);
    foreach (QExtCheckBox* box, checkboxList)
    {
        e2.setData(startIdx, box->pushData());
        ++startIdx;
    }

    static DataJsonRecElementE2::DataJsonRecElementE2FileHelper helper;
    helper.writeData(e2);
}

QChartView* CompQChartWidget::makeNewChart()
{
    QRTLineSeries *series = new QRTLineSeries();
    //*series << QPointF(1, 1) << QPointF(2, 2) << QPointF(3, 3) << QPointF(4, 4) << QPointF(5, 5) << QPointF(6, 6);

    QCxtChart *chart = new QCxtChart();
    chart->addSeries(series);
    //chart->setTheme(QChart::ChartThemeBrownSand);
    //chart->setMargins(QMargins(0,0,0,0));
    chart->legend()->hide();

    QValueAxis *axisX = new QValueAxis;
//    //axisX->setFormat("dd-MM h:mm:s");
    axisX->setTickCount(QRTLineSeries::MAX_NUM_POINTS / 4);
    axisX->setRange(0, QRTLineSeries::MAX_NUM_POINTS);
    axisX->hide();
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis;
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    m_chartsViewVector.append(new QChartView(chart));
    QChartView *chartView = m_chartsViewVector.last();
    chartView->setRenderHint(QPainter::Antialiasing);

    return chartView;
}

void CompQChartWidget::createChartsView()
{
    const int div = 2;
    int row = 0;
    int col = 0;
    for (int idx = 0; idx < MAX_NUM_CHARTS_SUPPORT; ++idx)
    {
        QChartView* chartView = CompQChartWidget::makeNewChart();

        const QVector<QExtCheckBox *>& checkboxList = QExtCheckBox::qExtSpinBoxList();
        const quint32 sizeOfCheckBox = checkboxList.size();

        const QExtCheckBox* box;
        if (idx < sizeOfCheckBox){
           box = checkboxList[idx];
        }
        else{
            qDebug() << "compQChartWidget:: more than checked box options";
            break;
        }
        QChart* chart = chartView->chart();
        QValueAxis* yaxis = static_cast<QValueAxis*>(chart->axisY());
        chart->setTitle(box->str() + '(' + box->unit() + ')');
        //yaxis->setTitleText(box->unit());
        yaxis->setLabelFormat(QStringLiteral("%d"));
        yaxis->setTickCount(5);
        yaxis->setRange(box->lowLimit(), box->upLimit());

        chartView->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Minimum);

        //m_lhsLayout->addWidget(chartView, 0, Qt::AlignTop);
        row = idx/div;
        col = idx%div;
        m_lhsLayout->addWidget(chartView, row, col);
    }
}

void CompQChartWidget::createCheckBoxView()
{
    const JsonPVConfig* config = m_reader->config();
    foreach (const JsonGUIElement& ele, m_reader->guiList()->elem())
    {
        /*memory leakage here -- eshenhu*/
        QExtCheckBox* checkbox = QExtCheckBox::makeExtCheckBox(*config, ele);


//        checkbox->setChecked(ele.isSelected());
//        checkbox->setSizePolicy(QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Maximum);

//        m_rhsLayout->addWidget(checkbox, 0, Qt::AlignVertical_Mask);
//        m_rhsLayout->addStretch();
    }
//    QPushButton* applyButton = new QPushButton(tr("Apply"));

//    connect(applyButton, &QPushButton::clicked, [this](){
//        updateChartsView();
//    });

//    m_rhsLayout->addWidget(applyButton, 1, Qt::AlignTop);
}

//void CompQChartWidget::createContextMenu()
//{
//    spreadsheet->addAction(cutAction);
//    spreadsheet->addAction(copyAction);
//    spreadsheet->addAction(pasteAction);
//    this->setContextMenuPolicy(Qt::ActionsContextMenu);
//}

//void CompQChartWidget::updateChartsView()
//{
//    int idx = 0;
//    const QVector<QExtCheckBox *>& checkboxList = QExtCheckBox::qExtSpinBoxList();
//    foreach (QExtCheckBox* box, checkboxList)
//    {
//        box->setAssoChartView(nullptr);

//        if (box->isChecked())
//        {
//            if (idx > MAX_NUM_CHARTS_SUPPORT)
//            {
//                qWarning() << "ui.chartsview Total Number of Charts was exceeded the MAX Support Charts" << MAX_NUM_CHARTS_SUPPORT;
//                return;
//            }

//            if (idx < m_chartsViewVector.size())
//            {
//                box->setAssoChartView(m_chartsViewVector[idx]);
//                QChart* chart = m_chartsViewVector[idx]->chart();
//                QValueAxis* yaxis = static_cast<QValueAxis*>(chart->axisY());
//                chart->setTitle(box->str() + '(' + box->unit() + ')');
//                //yaxis->setTitleText(box->unit());
//                yaxis->setLabelFormat(QStringLiteral("%d"));
//                yaxis->setTickCount(5);
//                yaxis->setRange(box->lowLimit(), box->upLimit());
//            }
//            else
//            {
//                qWarning() << "ui.chartsview Total Number of Charts " << m_chartsViewVector.size()
//                           << "was exceeded the Vector Charts" << MAX_NUM_CHARTS_SUPPORT;
//            }
//            ++idx;
//        }
//    }
//}
