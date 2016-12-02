#include "multichartwidget.h"
#include "ui/qextcheckbox.h"
#include <QtGlobal>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChart>

MultiChartWidget::MultiChartWidget(QWidget *parent) : QWidget(parent)
{
    m_layout = new QVBoxLayout;

    m_layout->setSizeConstraint(QLayout::SetMaximumSize);
    m_layout->setMargin(0);
    m_layout->setContentsMargins(0,0,0,0);

    createChartsView();

    setLayout(m_layout);
}

QChartView *MultiChartWidget::makeNewChart()
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
//    axisX->setTickCount(QRTLineSeries::MAX_NUM_POINTS / 4);
//    axisX->setRange(0, QRTLineSeries::MAX_NUM_POINTS);
//    axisX->hide();
//    chart->addAxis(axisX, Qt::AlignBottom);
//    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis;
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    m_chartsViewVector.append(new QChartView(chart));
    QChartView *chartView = m_chartsViewVector.last();
    chartView->setRenderHint(QPainter::Antialiasing);

    return chartView;
}

void MultiChartWidget::createChartsView()
{
    for (int idx = 0; idx < MAX_NUM_CHARTS_SUPPORT; ++idx)
    {
        QChartView* chartView = makeNewChart();

        const QVector<QExtCheckBox *>& checkboxList = QExtCheckBox::qExtSpinBoxList();
        const quint32 sizeOfCheckBox = checkboxList.size();

        const QExtCheckBox* box;
        if (idx < sizeOfCheckBox){
           box = checkboxList[idx];
        }
        else{
            qDebug() << "MultiChartWidget:: more than checked box options";
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
        m_layout->addWidget(chartView);
    }
}
