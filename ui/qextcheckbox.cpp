#include "qextcheckbox.h"
#include "ui/qrtlineseries.h"


QVector<QExtCheckBox*> QExtCheckBox::m_qExtSpinBoxList = QVector<QExtCheckBox*>();



QExtCheckBox::QExtCheckBox(const JsonPVConfig &config, const JsonGUIElement &element):
    AbstractSpinBoxAttr(config, element)
{
}

QExtCheckBox::~QExtCheckBox()
{
}

void QExtCheckBox::update(const QModbus2DataUnit *data,  Phase phase)
{
    AbstractSpinBoxAttr::update(data, phase);
//    if (QCheckBox::isChecked() && m_assoChartView != nullptr)
//    {
//        QChart* chart = m_assoChartView->chart();

//        QStringList title = chart->title().split(':');
//        QStringList newTitle;
//        newTitle << title[0] << QString::number(this->pushData());
//        chart->setTitle(newTitle.join(':'));

//        foreach (QAbstractSeries* series, chart->series())
//        {
//            static_cast<QRTLineSeries*>(series)->update(this->pushData());
//        }
//    }
}

QExtCheckBox *QExtCheckBox::makeExtCheckBox(const JsonPVConfig &config, const JsonGUIElement &element)
{
    QExtCheckBox::m_qExtSpinBoxList.append(new QExtCheckBox(config, element));
    return m_qExtSpinBoxList.last();
}

const QExtCheckBox *QExtCheckBox::searchExtCheckBox(const JsonGUIElement &element)
{
    foreach(QExtCheckBox* box, m_qExtSpinBoxList){
        if(box->type() == element.type() && box->idx() == element.idx())
            return box;
    }
    return nullptr;
}

const QExtCheckBox *QExtCheckBox::searchExtCheckBox(const JsonGUIPrimType type, const quint32 idxMotor)
{
    foreach(QExtCheckBox* box, m_qExtSpinBoxList){
        if(box->type() == type &&
                ( box->idx().idxMotor() == idxMotor
                  || box->idx().idxMotor() == 0x0F
                )
           )
            return box;
    }
    return nullptr;
}

const QVector<QExtCheckBox *>& QExtCheckBox::qExtSpinBoxList()
{
    return m_qExtSpinBoxList;
}

