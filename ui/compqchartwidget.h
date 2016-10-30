#ifndef COMPQCHARTWIDGET_H
#define COMPQCHARTWIDGET_H

#include <QWidget>
#include <QVector>
#include <QtCharts/QChartGlobal>

QT_CHARTS_BEGIN_NAMESPACE
class QLineSeries;
class QChart;
class QChartView;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class CfgJsonReader;
class QHBoxLayout;
class QVBoxLayout;
class QModbus2DataUnit;

class CompQChartWidget : public QWidget
{
    Q_OBJECT

    static const int MAX_NUM_CHARTS_SUPPORT = 4;
public:
    explicit CompQChartWidget(const CfgJsonReader* reader, QWidget *parent = 0);

signals:

public slots:
    void updateData(const QModbus2DataUnit* data);

private:
    QChartView* makeNewChart();
    void createChartsView();
    void createCheckBoxView();
    void updateChartsView();

private:
    QWidget* m_chartsWidget;
    QWidget* m_checkBoxWidget;
    QVector<QChartView*> m_chartsViewVector;
    QHBoxLayout* m_layout;
    QVBoxLayout* m_lhsLayout;
    QVBoxLayout* m_rhsLayout;
    const CfgJsonReader* m_reader;
};

#endif // COMPQCHARTWIDGET_H
