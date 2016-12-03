#ifndef QCXTCHART_H
#define QCXTCHART_H

class QMouseEvent;

#include <QtCharts/QChartGlobal>
#include <QtCharts/QChart>
#include "actionwidget.h"
#include "ui/qextcheckbox.h"

QT_CHARTS_USE_NAMESPACE

class QCxtChart : public QChart
{
    Q_OBJECT
public:
    QCxtChart(QExtCheckBox*);

public:
    void updateCharts();

//private:
//    void createContextMenu(QGraphicsSceneContextMenuEvent *event);

protected:
//    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

private:
    //QList<QCxtAction*> m_actions;
    QExtCheckBox* m_dataSrc;
};

#endif // QCXTCHART_H
