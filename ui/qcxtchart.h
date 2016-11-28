#ifndef QCXTCHART_H
#define QCXTCHART_H

class QMouseEvent;

#include <QtCharts/QChartGlobal>
#include <QtCharts/QChart>

QT_CHARTS_USE_NAMESPACE

class QCxtChart : public QChart
{
    Q_OBJECT
public:
    QCxtChart(const QVector<QAction*>& actions);

private:
    void createContextMenu();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    const QVector<QAction*>& m_UIActions;
};

#endif // QCXTCHART_H
