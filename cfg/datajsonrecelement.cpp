#include "datajsonrecelement.h"
#include <QString>

DataJsonRecElementE2 &DataJsonRecElementE2::DataJsonRecElementE2GetHelper::getElem(bool isNew)
{
    if (isNew){
        g_ele.reset();
        g_ele.incCursor();
    }
    return g_ele;
}

DataJsonRecElementE2::DataJsonRecElementE2():
    m_data(static_cast<quint32>(ELEMCURSOR::ELEMCURSOR_END))
{
}

const QString DataJsonRecElementE2::toString()
{
    return QStringLiteral("");
}

bool DataJsonRecElementE2::hardReset()
{
    // here start with 0!
    for (int i = 0; i < m_data.size(); ++i) {
        m_data[i] = 0;
      }
    return true;
}

bool DataJsonRecElementE2::reset()
{
    // here start with 1!
    for (int i = 1; i < m_data.size(); ++i) {
        m_data[i] = 0;
    }
    return true;
}

bool DataJsonRecElementE2::incCursor()
{
    ++m_data[static_cast<quint32>(ELEMCURSOR::CURSOR_POS)];
    return true;
}

bool DataJsonRecElementE2::setMetaData(quint32 vol, quint32 thro1, quint32 thro2, quint32 dis)
{
    return true;
}

bool DataJsonRecElementE2::setPosStatus()
{
    return true;
}

bool DataJsonRecElementE2::DataJsonRecElementE2FileHelper::newFile(const QString &path)
{
    return true;
}

bool DataJsonRecElementE2::DataJsonRecElementE2FileHelper::closeFile()
{
    return true;
}

bool DataJsonRecElementE2::DataJsonRecElementE2FileHelper::writeData(const DataJsonRecElementE2 &)
{
    return true;
}

//const QVector<DataJsonRecElementE2> &DataJsonRecElementE2::DataJsonRecElementE2FileHelper::loadData(const QString &filename)
//{

//}
