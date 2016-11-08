#include "abstractspinboxattr.h"
#include <QDebug>

AbstractSpinBoxAttr::AbstractSpinBoxAttr(const JsonPVConfig &config, const JsonGUIElement &ele):
    m_config(config),
    m_elem(ele)
{

}

void AbstractSpinBoxAttr::update(const QModbus2DataUnit *data)
{
    if (data)
    {
        const FunctionPair pair = m_funMap[this->type()];

        // it should be got improved here, in case of no key was found! --eshenhu
        if (!pair.formulaF)
            qWarning() << "ui.cfg  No matched formula was found! ";
        m_data = pair.functionF(data, m_config, this->idx());

        // it should record the data to persistent storage, like file.  --eshenhu
        m_pushData = pair.formulaF(m_data);
    }
}

double AbstractSpinBoxAttr::pushData() const
{
    return m_pushData;
}
