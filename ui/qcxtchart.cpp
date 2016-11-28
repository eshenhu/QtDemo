#include "qcxtchart.h"

#include <QtGlobal>


QCxtChart::QCxtChart()
{

}

void QCxtChart::createContextMenu()
{
    //QVector<QAction*>& m_UIActions
    foreach(const QAction* action, m_UIActions)
    {

    }

    this->addAction(cutAction);
        spreadsheet->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void QCxtChart::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

}
