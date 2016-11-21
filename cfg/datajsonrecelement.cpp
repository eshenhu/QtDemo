#include "datajsonrecelement.h"
#include <QString>
#include <QTextStream>
#include <QDebug>

#include "ui/qextcheckbox.h"

//DataJsonRecElementE2 &DataJsonRecElementE2::DataJsonRecElementE2GetHelper::getElem(bool isNew)
//{
//    if (isNew){
//        g_ele.reset();
//        g_ele.incCursor();
//    }
//    return g_ele;
//}

DataJsonRecElementE2::DataJsonRecElementE2():
    m_data(static_cast<quint32>(ELEMCURSOR::ELEMCURSOR_END))
{
}

const QString DataJsonRecElementE2::toString() const
{
    QStringList list;
    foreach (const quint32& v, m_data){
        list << QString::number(v);
    }

    return list.join(QStringLiteral(","));
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
    m_data[static_cast<quint32>(ELEMCURSOR::VOL_POS)] = vol;
    m_data[static_cast<quint32>(ELEMCURSOR::THRO1_POS)] = thro1;
    m_data[static_cast<quint32>(ELEMCURSOR::THRO2_POS)] = thro2;
    m_data[static_cast<quint32>(ELEMCURSOR::DISTANCE_POS)] = dis;
    return true;
}

bool DataJsonRecElementE2::setPosStatus(quint32 v)
{
    m_data[static_cast<quint32>(ELEMCURSOR::REC_POSSTATUS_POS)] = v;
    return true;
}

bool DataJsonRecElementE2::DataJsonRecElementE2FileHelper::newFile(const QString &path)
{
    if (m_fileHandler.exists())
        m_fileHandler.close();

    m_fileHandler.setFileName(path);
    if (!m_fileHandler.open(QIODevice::Append | QIODevice::Text)){
        qWarning() << QString("cfg.dataJsonRecElementE2 Failed to open file %1 for writing. ")
                      .arg(path);
        return false;
    }

    QTextStream out(&m_fileHandler);
    out << this->getTitle() << "\n";

    return true;
}

bool DataJsonRecElementE2::DataJsonRecElementE2FileHelper::closeFile()
{
    if (m_fileHandler.exists())
        m_fileHandler.close();
    return true;
}

bool DataJsonRecElementE2::DataJsonRecElementE2FileHelper::writeData(const DataJsonRecElementE2& v)
{
//    //m_filename
//    QFile saveFile(m_filename);
//    if (!saveFile.open(QIODevice::Append | QIODevice::Text))
//        return;

    QTextStream out(&m_fileHandler);
    out << v.toString() << '\n';

    return true;
}

// It should be improved here!  Why there are so many items left to be improved?! fuck me.
const QString &DataJsonRecElementE2::DataJsonRecElementE2FileHelper::getTitle()
{
    QStringList list;
    list << "#" << "voltage" << "throttle 1" << "throttle 2" << "distance"
         << "position";

    const QVector<QExtCheckBox *>& checkboxList = QExtCheckBox::qExtSpinBoxList();
    foreach (const QExtCheckBox* box, checkboxList)
    {
        list << box->str();
    }

    return list.join(",");
}

DataJsonRecElementE2 &DataJsonRecElementE2::DataJsonRecElementE2GetHelper::getElem(bool isNew)
{
    static DataJsonRecElementE2 g_ele;
    if (isNew){
        g_ele.reset();
        g_ele.incCursor();
    }
    return g_ele;
}
