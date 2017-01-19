#include "datajsonrecelement.h"
#include <QString>
#include <QTextStream>
#include <QDebug>

#include "qtcore/qloggingcategory.h"

#include "ui/qextcheckbox.h"
#include "cfg/datajsonrecelement.h"
#include "cfg/cfgreshandlerinf.h"
#include "actionwidget.h"
#include "cfg/unireslocation.h"

//DataJsonRecElementE2 &DataJsonRecElementE2::DataJsonRecElementE2GetHelper::getElem(bool isNew)
//{
//    if (isNew){
//        g_ele.reset();
//        g_ele.incCursor();
//    }
//    return g_ele;
//}

DataJsonRecElement::DataJsonRecElement():
    m_cursorEnd(0),
    m_data(0)
{
    if (UniResLocation::getCfgResHdl()->motor_type() == CfgResHandlerInf::MotorType::PELEC)
    {
        quint8 numOfMotor = UniResLocation::getCfgResHdl()->num_of_motor();
        if (numOfMotor == 1)
        {
            m_cursorEnd = static_cast<quint8>(ELEMCURSOR::REC_M1MECHAEFFI) + 1;
        }
        else if (numOfMotor == 2)
        {
            m_cursorEnd = static_cast<quint8>(ELEMCURSOR::REC_M2MECHAEFFI) + 1;
        }
        else
        {
            qCritical() << QString("number of motor %1 were beyond the maximum 2").arg(numOfMotor);
        }
    }
    else
    {
        qCritical() << "Not support motor type now.";
    }
    m_data.resize(m_cursorEnd);
}

const QString DataJsonRecElement::toString() const
{
    QStringList list;
    foreach (double v, m_data){
        list << QString::number(v, 'f', 2);
    }

    return list.join(QStringLiteral(","));
}

bool DataJsonRecElement::hardReset()
{
    // here start with 0!
    for (int i = 0; i < m_data.size(); ++i) {
        m_data[i] = 0;
      }
    return true;
}

bool DataJsonRecElement::reset()
{
    // here start with 1!
    for (int i = 1; i < m_data.size(); ++i) {
        m_data[i] = 0;
    }
    return true;
}

bool DataJsonRecElement::incCursor()
{
    ++m_data[static_cast<quint32>(ELEMCURSOR::CURSOR_POS)];
    return true;
}

bool DataJsonRecElement::setMetaData(quint32 vol, quint32 thro1, quint32 thro2, quint32 dis)
{
    m_data[static_cast<quint32>(ELEMCURSOR::VOL_POS)] = static_cast<double>(vol);
    m_data[static_cast<quint32>(ELEMCURSOR::THRO1_POS)] = static_cast<double>(thro1);
    m_data[static_cast<quint32>(ELEMCURSOR::THRO2_POS)] = static_cast<double>(thro2);
    m_data[static_cast<quint32>(ELEMCURSOR::DISTANCE_POS)] = static_cast<double>(dis);
    return true;
}

bool DataJsonRecElement::setPosStatus(quint32 v)
{
    m_data[static_cast<quint32>(ELEMCURSOR::REC_POSSTATUS_POS)] = static_cast<double>(v);
    return true;
}

quint8 DataJsonRecElement::getCursorEnd() const
{
    return m_cursorEnd;
}

bool DataJsonRecElement::DataJsonRecElementFileHelper::newFile(const QString &path)
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

bool DataJsonRecElement::DataJsonRecElementFileHelper::closeFile()
{
    if (getFile().exists())
        getFile().close();
    return true;
}

bool DataJsonRecElement::DataJsonRecElementFileHelper::writeData(const DataJsonRecElement& v)
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
const QString DataJsonRecElement::DataJsonRecElementFileHelper::getTitle()
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

DataJsonRecElement &DataJsonRecElement::DataJsonRecElementGetHelper::getElem(bool isNew)
{
    static DataJsonRecElement g_ele;
    if (isNew){
        g_ele.reset();
        g_ele.incCursor();
    }
    return g_ele;
}

DataJsonRecElement::DataJsonRecElementFileReaderHandler::DataJsonRecElementFileReaderHandler():
    m_data(0)
{

}

const QVector<DataJsonRecElement> &DataJsonRecElement::DataJsonRecElementFileReaderHandler::data()
{
    return m_data;
}

/*
 * load data from the file named with filename to the m_data;
*/
void DataJsonRecElement::DataJsonRecElementFileReaderHandler::loadData(const QString filename)
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
        list[0].toDouble(&isNumber);
        if (!isNumber){
            qCWarning(TEXT_LOGGING) << "Text the first line is not number, = " << list[0];
            continue;
        }
        /* we are the right way to split number*/

        /* I miss the python's method here */
        DataJsonRecElement element;

        if (list.size() != element.getCursorEnd()){
            qCWarning(TEXT_LOGGING) << "Text not match with the lenght with predefined structure. Expected = "
                                    << element.getCursorEnd() << "received = " << list.size();
        }


        int idx = 0;
        for (const QString& str : list){
            idx++;
            bool isNumber = false;
            double data = str.toDouble(&isNumber);
            if (!isNumber){
                qCWarning(TEXT_LOGGING) << "Text is not number, = " << str;
                data = 0.00;
                //continue;
            }
            element.setData(idx-1, data);
        }
        m_data.append(element);
    }
}

Q_LOGGING_CATEGORY(TEXT_LOGGING, "datajson.text", QtDebugMsg)
