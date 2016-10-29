#include "qextcheckbox.h"


QVector<QExtCheckBox*> QExtCheckBox::m_qExtSpinBoxList = QVector<QExtCheckBox*>();


QExtCheckBox::QExtCheckBox(QWidget *parent)
{
}

QExtCheckBox::QExtCheckBox(const JsonPVConfig &config, const JsonGUIElement &element, QWidget *parent):
    QCheckBox(element.str(), parent),
    AbstractSpinBoxAttr(config, element)
{
}

QExtCheckBox::~QExtCheckBox()
{
}

void QExtCheckBox::update(const QModbus2DataUnit *data)
{
    AbstractSpinBoxAttr::update(data);
    if (QCheckBox::isChecked())
    {
        //emit signal to update main viewport
    }
}

QExtCheckBox *QExtCheckBox::makeExtCheckBox(const JsonPVConfig &config, const JsonGUIElement &element, QWidget *parent)
{
    QExtCheckBox::m_qExtSpinBoxList.append(new QExtCheckBox(config, element, parent));
    return m_qExtSpinBoxList.last();
}

const QVector<QExtCheckBox *>& QExtCheckBox::qExtSpinBoxList()
{
    return m_qExtSpinBoxList;
}
