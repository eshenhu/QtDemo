#include "qcxtchart.h"

#include <QMenu>
#include <QValueAxis>
#include "ui/qrtlineseries.h"
#include "unireslocation.h"

QCxtChart::QCxtChart():
    QChart()
{
    //createContextMenu();
}

void QCxtChart::updateCharts()
{
    QStringList title = this->title().split(':');
    QStringList newTitle;
    newTitle << title[0] << QString::number(m_dataSrc->pushData());
    this->setTitle(newTitle.join(':'));

    foreach (QAbstractSeries* series, this->series())
    {
        static_cast<QRTLineSeries*>(series)->update(m_dataSrc->pushData());
    }
}

void QCxtChart::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    //m_actions.clear();

    QMenu menu;
    int idx = 0;
    foreach(JsonGUIElement ele, UniResLocation::getCfgJsonHdl()->guiList()->elem()){
        //const QString str = QString("%2").arg(ele.str(), 0).arg(ele.unit(), 30);
        const QString str = QString::asprintf("%-15s\t%8s",
                        ele.str().toLatin1().constData(),
                        ele.unit().toLatin1().constData());
        QAction* action = menu.addAction(str);
        //const QCxtAction* action = new QCxtAction(ele, ele.str());
        action->setCheckable(true);
        action->setData(QVariant(idx++));

//        connect(action, &triggered, [this](bool checked){
//            const JsonGUIElement& type =
//                    static_cast<QCxtAction*>(QObject::sender())->type();
//            this->m_dataSrc = QExtCheckBox::searchExtCheckBox(type);

//            /* invalid the data in charts*/
//            foreach (QAbstractSeries* series, this->series())
//            {
//                static_cast<QRTLineSeries*>(series)->clear();
//            }
//        });
        //m_actions.append(action);
    }

    QAction *selectedAction = menu.exec(QCursor::pos());
    if (selectedAction){
        const QVector<QExtCheckBox *>& checkboxList = QExtCheckBox::qExtSpinBoxList();
        m_dataSrc = checkboxList[selectedAction->data().toInt()];

        /* invalid the data in charts*/
        foreach (QAbstractSeries* series, this->series())
        {
            static_cast<QRTLineSeries*>(series)->clear();
        }

        /* change the title and other outlooking */
        QValueAxis* yaxis = static_cast<QValueAxis*>(this->axisY());
        this->setTitle(m_dataSrc->str() + '(' + m_dataSrc->unit() + ')');
//        //yaxis->setTitleText(box->unit());
//        yaxis->setLabelFormat(QStringLiteral("%d"));
//        yaxis->setTickCount(5);
        yaxis->setRange(m_dataSrc->lowLimit(), m_dataSrc->upLimit());

    }
    // this->setContextMenuPolicy(Qt::ActionsContextMenu);
}

//void QCxtChart::mousePressEvent(QGraphicsSceneMouseEvent *event)
//{
//    if (event->buttons() & Qt::RightButton){

//    }
//    QChart::mousePressEvent(event);
//}

