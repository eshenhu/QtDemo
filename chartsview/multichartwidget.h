#ifndef MULTICHARTWIDGET_H
#define MULTICHARTWIDGET_H

#include <QWidget>
#include <QVector>
#include <QVBoxLayout>
#include <QtCharts/QChartGlobal>

QT_CHARTS_BEGIN_NAMESPACE
class QLineSeries;
class QChart;
class QChartView;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class MultiChartWidget : public QWidget
{
    Q_OBJECT
    static const int MAX_NUM_CHARTS_SUPPORT = 2;

public:
    explicit MultiChartWidget(QWidget *parent = 0);

signals:

public slots:
private:
    QChartView* makeNewChart();
    void createChartsView();
private:
    QVector<QChartView*> m_chartsViewVector;

    QVBoxLayout* m_layout;
};

#endif // MULTICHARTWIDGET_H
