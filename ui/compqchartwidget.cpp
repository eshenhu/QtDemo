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

CompQChartWidget::CompQChartWidget(const CfgGUIJsonReader* reader, QWidget *parent) :
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

void CompQChartWidget::updateData(const QModbus2DataUnit *data, Phase phase)
{
    const QVector<QExtCheckBox *>& checkboxList = QExtCheckBox::qExtSpinBoxList();
    foreach (QExtCheckBox* box, checkboxList)
    {
        box->update(data, phase);
    }

    foreach(const QChartView* chartView ,m_chartsViewVector){
        QCxtChart* chart = static_cast<QCxtChart*>(chartView->chart());
        chart->updateCharts();
    }

    /*
     * Record the log.
     */
    DataJsonRecElement& e2 = DataJsonRecElement::DataJsonRecElementGetHelper().getElem();

    quint32 startIdx = static_cast<quint32>(DataJsonRecElement::ELEMCURSOR::REC_ECHODISTANCE_POS);
    foreach (QExtCheckBox* box, checkboxList)
    {
        e2.setData(startIdx, box->pushData());
        ++startIdx;
    }

    if (phase == Phase::Phase_NomalRunning)
    {
        static DataJsonRecElement::DataJsonRecElementFileHelper helper;
        helper.writeData(e2);
    }
}

void CompQChartWidget::resetChartWidget()
{
    for (QChartView* chartView : m_chartsViewVector)
    {
        QCxtChart* chart = dynamic_cast<QCxtChart *>(chartView->chart());
        chart->resetCharts();
    }
}

QChartView* CompQChartWidget::makeNewChart(QExtCheckBox* box)
{
    QRTLineSeries *series = new QRTLineSeries();
    //*series << QPointF(1, 1) << QPointF(2, 2) << QPointF(3, 3) << QPointF(4, 4) << QPointF(5, 5) << QPointF(6, 6);

    QCxtChart *chart = new QCxtChart(box);
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


    const QVector<QExtCheckBox *>& checkboxList = QExtCheckBox::qExtSpinBoxList();

    quint32 idxOfChart = 0;
    for (QExtCheckBox* box : checkboxList)
    {
        if (box->isSelected())
        {
            do {
                QChartView* chartView = CompQChartWidget::makeNewChart(box);

                QChart* chart = chartView->chart();
                QValueAxis* yaxis = static_cast<QValueAxis*>(chart->axisY());

                const char* titleFormat = "%-15s";
                const char* unitFormat = "%-5s";

                static char buffer[100];
                sprintf(buffer, "%s( %s )", titleFormat, unitFormat);

                QString titleComp = QString::asprintf(buffer,
                                                      box->str().toLatin1().constData(),
                                                      box->unit().toLatin1().constData());

                chart->setTitle(titleComp);
                //yaxis->setTitleText(box->unit());
                yaxis->setLabelFormat(QStringLiteral("%d"));
                yaxis->setTickCount(5);
                yaxis->setRange(box->lowLimit(), box->upLimit());

                chartView->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Minimum);

                //m_lhsLayout->addWidget(chartView, 0, Qt::AlignTop);
                row = idxOfChart/div;
                col = idxOfChart%div;
                m_lhsLayout->addWidget(chartView, row, col);
            }while(0);

            if (++idxOfChart >= MAX_NUM_CHARTS_SUPPORT)
                break;
        }
    }
}

void CompQChartWidget::createCheckBoxView()
{
    const JsonPVConfig* config = m_reader->config();
    foreach (const JsonGUIElement& ele, m_reader->guiList()->elem())
    {
        /*memory leakage here -- eshenhu*/
        QExtCheckBox* checkbox = QExtCheckBox::makeExtCheckBox(*config, ele);
    }
}
