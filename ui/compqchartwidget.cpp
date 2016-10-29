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
#include <QtCharts/QDateTimeAxis>

#include <QDebug>

#include "ui/qrtlineseries.h"

QT_CHARTS_USE_NAMESPACE

CompQChartWidget::CompQChartWidget(const CfgJsonReader* reader, QWidget *parent) :
    QWidget(parent),
    m_reader(reader)
{
    m_layout = new QHBoxLayout;
    m_lhsLayout = new QVBoxLayout;
    m_rhsLayout = new QVBoxLayout;

    createCheckBoxView();
    createChartsView();

    m_layout->addLayout(m_lhsLayout, 1);
    m_layout->addLayout(m_rhsLayout, 0);

    setLayout(m_layout);
}

QChartView* CompQChartWidget::makeNewChart()
{
    QRTLineSeries *series = new QRTLineSeries();
    *series << QPointF(1, 1) << QPointF(2, 73) << QPointF(3, 268) << QPointF(4, 17) << QPointF(5, 4325) << QPointF(6, 723);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->legend()->hide();
    //chart->setTitle("Logarithmic axis example");

    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setFormat("dd-MM h:mm:s");
    axisX->setTickCount(6);
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
    for (int idx = 0; idx < MAX_NUM_CHARTS_SUPPORT; ++idx)
    {
        QChartView* chartView = CompQChartWidget::makeNewChart();
        chartView->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Minimum);
        m_lhsLayout->addWidget(chartView, 0, Qt::AlignVertical_Mask);
    }
}

void CompQChartWidget::createCheckBoxView()
{
    const JsonPVConfig* config = m_reader->config();
    foreach (const JsonGUIElement& ele, m_reader->guiList()->elem())
    {
        QCheckBox* checkbox = QExtCheckBox::makeExtCheckBox(*config, ele, this);
        checkbox->setChecked(ele.isSelected());
        checkbox->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Minimum);

        m_rhsLayout->addWidget(checkbox, 0, Qt::AlignVertical_Mask);
        m_rhsLayout->addStretch();
    }
    QPushButton* applyButton = new QPushButton(tr("Apply"));

    connect(applyButton, &QPushButton::clicked, [this](){
        quint32 totalNum = 0;
        const QVector<QExtCheckBox *>& checkboxList = QExtCheckBox::qExtSpinBoxList();
        foreach (const QExtCheckBox* box, checkboxList)
        {
            if (box->isChecked())
            {
                ++totalNum;
                if (totalNum > MAX_NUM_CHARTS_SUPPORT)
                {
                    qWarning() << "ui.chartsview Total Number of Charts was exceeded the MAX Support Charts" << MAX_NUM_CHARTS_SUPPORT;
                    return;
                }
            }
        }
        updateChartsView();
    });
    m_rhsLayout->addWidget(applyButton, 1, Qt::AlignTop);
}

void CompQChartWidget::updateChartsView()
{
    int idx = 0;
    const QVector<QExtCheckBox *>& checkboxList = QExtCheckBox::qExtSpinBoxList();
    foreach (const QExtCheckBox* box, checkboxList)
    {
        if (box->isChecked())
        {
            if (idx > MAX_NUM_CHARTS_SUPPORT)
            {
                qWarning() << "ui.chartsview Total Number of Charts was exceeded the MAX Support Charts" << MAX_NUM_CHARTS_SUPPORT;
                return;
            }

            if (idx < m_chartsViewVector.size())
            {
                QChart* chart = m_chartsViewVector[idx]->chart();

                QValueAxis* yaxis = static_cast<QValueAxis*>(chart->axisY());
                chart->setTitle(box->str());
                yaxis->setTitleText(box->unit());
                yaxis->setTickCount(5);
                yaxis->setRange(box->lowLimit(), box->upLimit());
            }
            else
            {
                qWarning() << "ui.chartsview Total Number of Charts " << m_chartsViewVector.size()
                           << "was exceeded the Vector Charts" << MAX_NUM_CHARTS_SUPPORT;
            }
            ++idx;
        }
    }
}
