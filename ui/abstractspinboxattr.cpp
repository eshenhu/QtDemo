#include "abstractspinboxattr.h"

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
        m_data = pair.functionF(data, m_config, this->idx());

        // it should record the data to persistent storage, like file.  --eshenhu
        m_pushData = pair.formulaF(m_data);
    }
}
