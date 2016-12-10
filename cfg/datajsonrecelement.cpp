#include "datajsonrecelement.h"
#include <QString>
#include <QTextStream>
#include <QDebug>

#include "qtcore/qloggingcategory.h"

#include "ui/qextcheckbox.h"
#include "cfg/datajsonrecelement.h"
#include "actionwidget.h"
#include "unireslocation.h"

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
    foreach (double v, m_data){
        list << QString::number(v, 'f', 2);
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
    m_data[static_cast<quint32>(ELEMCURSOR::VOL_POS)] = static_cast<double>(vol);
    m_data[static_cast<quint32>(ELEMCURSOR::THRO1_POS)] = static_cast<double>(thro1);
    m_data[static_cast<quint32>(ELEMCURSOR::THRO2_POS)] = static_cast<double>(thro2);
    m_data[static_cast<quint32>(ELEMCURSOR::DISTANCE_POS)] = static_cast<double>(dis);
    return true;
}

bool DataJsonRecElementE2::setPosStatus(quint32 v)
{
    m_data[static_cast<quint32>(ELEMCURSOR::REC_POSSTATUS_POS)] = static_cast<double>(v);
    return true;
}

bool DataJsonRecElementE2::DataJsonRecElementE2FileHelper::newFile(const QString &path)
{
    if (getFile().exists())
        getFile().close();

    getFile().setFileName(path);
    if (!getFile().open(QIODevice::Append | QIODevice::Text)){
        qWarning() << QString("cfg.dataJsonRecElementE2 Failed to open file %1 for writing. ")
                      .arg(path);
        return false;
    }

    QTextStream out(&getFile());
    out << this->getTitle() << "\n";

    return true;
}

bool DataJsonRecElementE2::DataJsonRecElementE2FileHelper::closeFile()
{
    if (getFile().exists())
        getFile().close();
    return true;
}

bool DataJsonRecElementE2::DataJsonRecElementE2FileHelper::writeData(const DataJsonRecElementE2& v)
{
//    //m_filename
//    QFile saveFile(m_filename);
//    if (!saveFile.open(QIODevice::Append | QIODevice::Text))
//        return;

    QFile& file = getFile();
    QTextStream out(&file);
    out << v.toString() << '\n';

    return true;
}

// It should be improved here!  Why there are so many items left to be improved?! fuck me.
const QString DataJsonRecElementE2::DataJsonRecElementE2FileHelper::getTitle()
{
    QStringList list;
    list << "#" << "voltage" << "throttle 1" << "throttle 2" << "distance"
         << "position";

//    const QVector<QExtCheckBox *>& checkboxList = QExtCheckBox::qExtSpinBoxList();
//    foreach (const QExtCheckBox* box, checkboxList)
//    {
//        list << box->str();
//    }

//    QStringList list;
    foreach (const JsonGUIElement& ele, UniResLocation::getCfgJsonHdl()->guiList()->elem()){
        list << ele.str();
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

DataJsonRecElementE2::DataJsonRecElementE2FileReaderHandler::DataJsonRecElementE2FileReaderHandler():
    m_data(0)
{

}

const QVector<DataJsonRecElementE2> &DataJsonRecElementE2::DataJsonRecElementE2FileReaderHandler::data()
{
    return m_data;
}

/*
 * load data from the file named with filename to the m_data;
*/
void DataJsonRecElementE2::DataJsonRecElementE2FileReaderHandler::loadData(const QString filename)
{
    //m_filename
    QFile loadFile(filename);
    if (!loadFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream out(&loadFile);

    // the 1st line is the comment line;
    out.readLine();

    QStringList list;
    while (!out.atEnd()){
        list = out.readLine().split(',');

        /* the 1st str must be the cursor indication*/
        bool isNumber = false;
        list[0].toInt(&isNumber);
        if (!isNumber){
            qCWarning(TEXT_LOGGING) << "Text the first line is not number, = " << list[0];
            continue;
        }
        /* we are the right way to split number*/

        if (list.size() != (int)ELEMCURSOR::ELEMCURSOR_END){
            qCWarning(TEXT_LOGGING) << "Text not match with the lenght with predefined structure. Expected = "
                                    << (int)ELEMCURSOR::ELEMCURSOR_END << "received = " << list.size();
        }

        /* I miss the python's method here */
        DataJsonRecElementE2 element;
        int idx = 0;
        for (const QString& str : list){
            idx++;
            bool isNumber = false;
            double data = str.toDouble(&isNumber);
            if (!isNumber){
                qCWarning(TEXT_LOGGING) << "Text is not number, = " << data;
                continue;
            }
            element.setData(idx-1, data);
        }
        m_data.append(element);
    }
}

Q_LOGGING_CATEGORY(TEXT_LOGGING, "datajson.text", QtDebugMsg)
