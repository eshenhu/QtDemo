#ifndef QCXTCHART_H
#define QCXTCHART_H

class QMouseEvent;

#include <QtCharts/QChartGlobal>
#include <QtCharts/QChart>
//#include "actionwidget.h"
#include "ui/qextcheckbox.h"

QT_CHARTS_USE_NAMESPACE

class QCxtChart : public QChart
{
    Q_OBJECT
public:
    QCxtChart(QExtCheckBox*);

public:
    void updateCharts();
    void resetCharts();

//private:
//    void createContextMenu(QGraphicsSceneContextMenuEvent *event);

protected:
//    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

private:
    //QList<QCxtAction*> m_actions;
    QExtCheckBox* m_dataSrc;

    // further improvment --eshenhu
//    qint32 m_dynUpLimit;
//    qint32 m_dynLowLimit;
};

#endif // QCXTCHART_H
