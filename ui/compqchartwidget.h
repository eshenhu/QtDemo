#ifndef COMPQCHARTWIDGET_H
#define COMPQCHARTWIDGET_H

#include <QWidget>
#include <QVector>
#include <QtCharts/QChartGlobal>
#include "driver/modelpoctype.h"

using namespace ModelPOC;

QT_CHARTS_BEGIN_NAMESPACE
class QLineSeries;
class QChart;
class QChartView;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class CfgGUIJsonReader;
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;
class QModbus2DataUnit;
class QExtCheckBox;
class CompQChartWidget : public QWidget
{
    Q_OBJECT

    static const int MAX_NUM_CHARTS_SUPPORT = 8;
public:
    explicit CompQChartWidget(const CfgGUIJsonReader* reader, QWidget *parent = 0);

signals:

public slots:
    void updateData(const QModbus2DataUnit* data, Phase phase);

private:
    QChartView* makeNewChart(QExtCheckBox*);
    void createChartsView();
    void createCheckBoxView();
    //void createContextMenu();
    //void updateChartsView();

private:
    QWidget* m_chartsWidget;
    QWidget* m_checkBoxWidget;
    QVector<QChartView*> m_chartsViewVector;
    QHBoxLayout* m_layout;
    //QVBoxLayout* m_lhsLayout;
    QGridLayout* m_lhsLayout;
    QVBoxLayout* m_rhsLayout;
    const CfgGUIJsonReader* m_reader;
};

#endif // COMPQCHARTWIDGET_H
