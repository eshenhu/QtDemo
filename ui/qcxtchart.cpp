#include "qcxtchart.h"

#include <QMenu>
#include <QValueAxis>
#include "ui/qrtlineseries.h"
#include "cfg/unireslocation.h"

QCxtChart::QCxtChart(QExtCheckBox* box):
    QChart(),
    m_dataSrc(box)
{
    //createContextMenu();
}

void QCxtChart::updateCharts()
{
//    QStringList title = this->title().split(':');
//    QStringList newTitle;
//    newTitle << title[0] << QString::number(m_dataSrc->pushData());

    const char* titleFormat = "%-15s";
    const char* unitFormat = "%-5s";
    const char* numberFormat = JsonGUIElement::format(m_dataSrc->type());

    static char buffer[150];
    sprintf(buffer, "%s( %s ): %s", titleFormat, unitFormat, numberFormat);


    QString titleComp = QString::asprintf(buffer,
                      m_dataSrc->str().toLatin1().constData(),
                      m_dataSrc->unit().toLatin1().constData(),
                      m_dataSrc->pushData());


    //this->setTitle(newTitle.join(':'));
    this->setTitle(titleComp);

    foreach (QAbstractSeries* series, this->series())
    {
        static_cast<QRTLineSeries*>(series)->update(m_dataSrc->pushData());
    }
}

void QCxtChart::resetCharts()
{
    foreach (QAbstractSeries* series, this->series())
    {
        static_cast<QRTLineSeries*>(series)->clear();
    }
}

void QCxtChart::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    Q_UNUSED(event)

    QVector<const QExtCheckBox *> availableCheckboxList;

    const QVector<QExtCheckBox *>& checkboxList = QExtCheckBox::qExtSpinBoxList();

    for (const QExtCheckBox* box : checkboxList)
    {
        if (box->isSelected())
        {
            availableCheckboxList.append(box);
        }
    }

    QMenu menu;
    int idx = 0;

    foreach (const QExtCheckBox* box , availableCheckboxList)
    {
        int idxMotor = box->idx().idxMotor();
        QString motorString(" ");
        if (idxMotor == 0 || idxMotor == 1)
            motorString = QString::number(idxMotor + 1);

        const QString str = QString::asprintf("%-3s%-15s\t%8s",
                        motorString.toLatin1().constData(),
                        box->str().toLatin1().constData(),
                        box->unit().toLatin1().constData());
        QAction* action = menu.addAction(str);
        //const QCxtAction* action = new QCxtAction(ele, ele.str());
        action->setCheckable(true);
        action->setData(QVariant(idx++));
    }

    QAction *selectedAction = menu.exec(QCursor::pos());
    if (selectedAction){
//        const QVector<QExtCheckBox *>& checkboxList = QExtCheckBox::qExtSpinBoxList();
        m_dataSrc = const_cast<QExtCheckBox*>(availableCheckboxList[selectedAction->data().toInt()]);

        /* invalid the data in charts*/
        foreach (QAbstractSeries* series, this->series())
        {
            static_cast<QRTLineSeries*>(series)->clear();
        }

        /* change the title and other outlooking */
        QValueAxis* yaxis = static_cast<QValueAxis*>(this->axisY());

        const char* titleFormat = "%-25s";
        const char* unitFormat = "%-5s";

        char buffer[100];
        sprintf(buffer, "%s( %s )", titleFormat, unitFormat);

        QString titleComp = QString::asprintf(buffer,
                          m_dataSrc->str().toLatin1().constData(),
                          m_dataSrc->unit().toLatin1().constData());

        this->setTitle(titleComp);
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

