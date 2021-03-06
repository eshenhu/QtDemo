#include "cfgwashingdatainf.h"
#include "ui/uiheader.h"
#include <QMap>
#include "util/polyfit.h"
#include <memory>



/*
 * ManualTst for 1 elec motors.
*/
CfgManualWashingDataE1Clz::CfgManualWashingDataE1Clz():
    CfgWashingDataInf(CfgWashingTypeEnum::CFGWASHINGTIME_E1),
    m_data(0)
{
    CfgItemMeasBasedE1WashingOutDataEle ele;
    m_guiList =  ele.m_metaEle;
}

void CfgManualWashingDataE1Clz::wash(const QVector<DataJsonRecElement> &rawdata)
{
    for (const DataJsonRecElement& data : rawdata)
    {
        CfgItemMeasBasedE1WashingOutDataEle ele = deserialize(data);
        m_data.append(ele);
    }
}

QVector<CfgItemMeasBasedE1WashingOutDataEle> &CfgManualWashingDataE1Clz::data()
{
    return m_data;
}

void CfgManualWashingDataE1Clz::generateData(quint32 idx, QVector<QCPGraphData> &pairs, QString &name, quint8 &motorIdx)
{
    name = QStringLiteral("TIME :") +  m_data[0].getName(idx),
    motorIdx = m_data[0].getMotorIdx(idx);

    pairs.resize(m_data.size());
    for (int i = 0; i < m_data.size(); i++)
    {
        pairs[i].key   = i;
        pairs[i].value = m_data[i].getData(idx);
    }
}

CfgItemMeasBasedE1WashingOutDataEle CfgManualWashingDataE1Clz::deserialize(const DataJsonRecElement &in)
{
    CfgItemMeasBasedE1WashingOutDataEle ele;
    ele.setData( (quint8)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_VOL_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::VOL_POS));
    ele.setData( (quint8)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_THRO_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::THRO1_POS));
    //ele.setData( (quint8)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_DISTANCE_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_ECHODISTANCE_POS));

    ele.setData( (quint8)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_CUR1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_THU1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_THU1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_TORQUE1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_SPEED1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_MOTOR1TMP1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_MOTOR1TMP1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_MOTOR1TMP2_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_MOTOR1TMP2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_M1EFFICI1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M1EFFICI1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_M1EFFICI2_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M1EFFICI2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_M1POWER, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M1POWER));
    ele.setData( (quint8)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_M1MECHAPWR, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M1MECHAPWR));
    ele.setData( (quint8)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_M1MECHAEFFI, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M1MECHAEFFI));

    return ele;
}

/*
 * ManualTst for 1 elec motors.
*/
CfgManualWashingDataE2Clz::CfgManualWashingDataE2Clz():
    CfgWashingDataInf(CfgWashingTypeEnum::CFGWASHINGTIME_E2),
    m_data(0)
{
    CfgItemMeasBasedE2WashingOutDataEle ele;
    m_guiList =  ele.m_metaEle;
}

void CfgManualWashingDataE2Clz::wash(const QVector<DataJsonRecElement> &rawdata)
{
    for (const DataJsonRecElement& data : rawdata)
    {
        CfgItemMeasBasedE2WashingOutDataEle ele = deserialize(data);
        m_data.append(ele);
    }
}

QVector<CfgItemMeasBasedE2WashingOutDataEle> &CfgManualWashingDataE2Clz::data()
{
    return m_data;
}

void CfgManualWashingDataE2Clz::generateData(quint32 idx, QVector<QCPGraphData> &pairs, QString &name, quint8 &motorIdx)
{
    name = QStringLiteral("TIME :") +  m_data[0].getName(idx),
    motorIdx = m_data[0].getMotorIdx(idx);

    pairs.resize(m_data.size());
    for (int i = 0; i < m_data.size(); i++)
    {
        pairs[i].key   = i;
        pairs[i].value = m_data[i].getData(idx);
    }
}

CfgItemMeasBasedE2WashingOutDataEle CfgManualWashingDataE2Clz::deserialize(const DataJsonRecElement &in)
{
    CfgItemMeasBasedE2WashingOutDataEle ele;
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_VOL_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::VOL_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_THRO_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::THRO1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_DISTANCE_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_ECHODISTANCE_POS));

    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_CUR1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_THU1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_THU1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_TORQUE1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_SPEED1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_MOTOR1TMP1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_MOTOR1TMP1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_MOTOR1TMP2_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_MOTOR1TMP2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_M1EFFICI1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M1EFFICI1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_M1EFFICI2_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M1EFFICI2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_M1POWER, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M1POWER));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_M1MECHAPWR, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M1MECHAPWR));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_M1MECHAEFFI, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M1MECHAEFFI));

    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_CUR2_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_THU2_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_THU2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_TORQUE2_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_SPEED2_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_MOTOR2TMP1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_MOTOR2TMP1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_MOTOR2TMP2_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_MOTOR2TMP2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_M2EFFICI1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M2EFFICI1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_M2EFFICI2_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M2EFFICI2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_M2POWER, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M2POWER));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_M2MECHAPWR, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M2MECHAPWR));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_M2MECHAEFFI, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M2MECHAEFFI));

    return ele;
}
/*
 * Vol for 1 elec motors.
*/
CfgVolWashingDataE1Clz::CfgVolWashingDataE1Clz():
    CfgWashingDataInf(CfgWashingTypeEnum::CFGWASHINGVOL_E1),
    m_data(0)
{
    CfgItemMeasBasedE1WashingOutDataEle ele;
    m_guiList =  ele.m_metaEle;
}

void CfgVolWashingDataE1Clz::wash(const QVector<DataJsonRecElement> &rawdata)
{
    quint32 cursor = std::numeric_limits<unsigned int>::min();

    quint32 dataInLine = 0;
    CfgItemMeasBasedE1WashingOutDataEle accuData;

    for (const DataJsonRecElement& data : rawdata)
    {
        CfgItemMeasBasedE1WashingOutDataEle ele = deserialize(data);
        dataInLine = (quint32)ele.getData((quint32)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_VOL_POS);

        if (cursor == std::numeric_limits<unsigned int>::min())
        {
            cursor = dataInLine;
        }

        if (cursor == dataInLine){
            accumulate(ele, accuData);
        }
        else{
            m_data.append(accuData);
            cursor = dataInLine;
            accuData = ele;
        }
    }
    m_data.append(accuData);
}

QVector<CfgItemMeasBasedE1WashingOutDataEle> &CfgVolWashingDataE1Clz::data()
{
    return m_data;
}

void CfgVolWashingDataE1Clz::generateData(quint32 idx, QVector<QCPGraphData> &pairs, QString &name, quint8 &motorIdx)
{
    name = m_data[0].getName((quint32)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_VOL_POS) + ':' +  m_data[0].getName(idx),
    motorIdx = m_data[0].getMotorIdx(idx);

    pairs.resize(m_data.size());
    for (int i = 0; i < m_data.size(); i++)
    {
        pairs[i].key   = (quint32)m_data[i].getData((quint32)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_VOL_POS);
        pairs[i].value = m_data[i].getData(idx);
    }
}

CfgItemMeasBasedE1WashingOutDataEle CfgVolWashingDataE1Clz::deserialize(const DataJsonRecElement &in)
{
    CfgItemMeasBasedE1WashingOutDataEle ele;
    ele.setData( (quint8)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_VOL_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::VOL_POS));
    ele.setData( (quint8)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_THRO_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::THRO1_POS));
    //ele.setData( (quint8)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_DISTANCE_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_ECHODISTANCE_POS));

    ele.setData( (quint8)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_CUR1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_THU1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_THU1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_TORQUE1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_SPEED1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_MOTOR1TMP1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_MOTOR1TMP1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_MOTOR1TMP2_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_MOTOR1TMP2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_M1EFFICI1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M1EFFICI1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_M1EFFICI2_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M1EFFICI2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_M1POWER, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M1POWER));
    ele.setData( (quint8)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_M1MECHAPWR, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M1MECHAPWR));
    ele.setData( (quint8)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_M1MECHAEFFI, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M1MECHAEFFI));

    return ele;
}

void CfgVolWashingDataE1Clz::accumulate(const CfgItemMeasBasedE1WashingOutDataEle &data, CfgItemMeasBasedE1WashingOutDataEle &accu)
{
    for (int i = 0; i < accu.m_metaEle.size(); i++)
    {
        accu.setData(i,(accu.getData(i) + data.getData(i))/2);
    }
}

/*
 * Vol for 2 elec motors.
*/
CfgVolWashingDataE2Clz::CfgVolWashingDataE2Clz():
    CfgWashingDataInf(CfgWashingTypeEnum::CFGWASHINGVOL_E2),
    m_data(0)
{
    CfgItemMeasBasedE2WashingOutDataEle ele;
    m_guiList =  ele.m_metaEle;
}

void CfgVolWashingDataE2Clz::wash(const QVector<DataJsonRecElement> &rawdata)
{
    quint32 cursor = std::numeric_limits<unsigned int>::min();

    quint32 dataInLine = 0;
    CfgItemMeasBasedE2WashingOutDataEle accuData;

    for (const DataJsonRecElement& data : rawdata)
    {
        CfgItemMeasBasedE2WashingOutDataEle ele = deserialize(data);
        dataInLine = (quint32)ele.getData((quint32)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_VOL_POS);

        if (cursor == std::numeric_limits<unsigned int>::min())
        {
            cursor = dataInLine;
        }

        if (cursor == dataInLine){
            accumulate(ele, accuData);
        }
        else{
            m_data.append(accuData);
            cursor = dataInLine;
            accuData = ele;
        }
    }
    m_data.append(accuData);
}

QVector<CfgItemMeasBasedE2WashingOutDataEle> &CfgVolWashingDataE2Clz::data()
{
    return m_data;
}

void CfgVolWashingDataE2Clz::generateData(quint32 idx, QVector<QCPGraphData> &pairs, QString &name, quint8 &motorIdx)
{
    name = m_data[0].getName((quint32)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_VOL_POS) + ':' +  m_data[0].getName(idx),
    motorIdx = m_data[0].getMotorIdx(idx);

    pairs.resize(m_data.size());
    for (int i = 0; i < m_data.size(); i++)
    {
        pairs[i].key   = (quint32)m_data[i].getData((quint32)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_VOL_POS);
        pairs[i].value = m_data[i].getData(idx);
    }
}

CfgItemMeasBasedE2WashingOutDataEle CfgVolWashingDataE2Clz::deserialize(const DataJsonRecElement &in)
{
    CfgItemMeasBasedE2WashingOutDataEle ele;
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_VOL_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::VOL_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_THRO_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::THRO1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_DISTANCE_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_ECHODISTANCE_POS));

    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_CUR1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_THU1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_THU1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_TORQUE1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_SPEED1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_MOTOR1TMP1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_MOTOR1TMP1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_MOTOR1TMP2_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_MOTOR1TMP2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_M1EFFICI1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M1EFFICI1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_M1EFFICI2_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M1EFFICI2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_M1POWER, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M1POWER));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_M1MECHAPWR, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M1MECHAPWR));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_M1MECHAEFFI, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M1MECHAEFFI));

    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_CUR2_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_THU2_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_THU2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_TORQUE2_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_SPEED2_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_MOTOR2TMP1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_MOTOR2TMP1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_MOTOR2TMP2_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_MOTOR2TMP2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_M2EFFICI1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M2EFFICI1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_M2EFFICI2_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M2EFFICI2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_M2POWER, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M2POWER));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_M2MECHAPWR, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M2MECHAPWR));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_M2MECHAEFFI, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M2MECHAEFFI));

    return ele;
}

void CfgVolWashingDataE2Clz::accumulate(const CfgItemMeasBasedE2WashingOutDataEle &data, CfgItemMeasBasedE2WashingOutDataEle &accu)
{
    for (int i = 0; i < accu.m_metaEle.size(); i++)
    {
        accu.setData(i,(accu.getData(i) + data.getData(i))/2);
    }
}

/*
 * -----------------------------------------------------------------------------
 *  Wash the raw data to the formateed data needed by the Distance test
 */

CfgDistanceWashingDataE2Clz::CfgDistanceWashingDataE2Clz():
    CfgWashingDataInf(CfgWashingTypeEnum::CFGWASHINGDISTANCE),
    m_data(0)
{
    CfgItemMeasBasedE2WashingOutDataEle ele;
    m_guiList =  ele.m_metaEle;
}

void CfgDistanceWashingDataE2Clz::wash(const QVector<DataJsonRecElement> &rawdata)
{
    quint32 cursor = std::numeric_limits<unsigned int>::min();

    quint32 dataInLine = 0;
    CfgItemMeasBasedE2WashingOutDataEle accuData;

    for (const DataJsonRecElement& data : rawdata)
    {
        CfgItemMeasBasedE2WashingOutDataEle ele = deserialize(data);

        if (dataInLine == 0)
            accuData = ele;
        dataInLine = (quint32)ele.getData((quint32)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_DISTANCE_POS);

        if (cursor == std::numeric_limits<unsigned int>::min())
        {
            cursor = dataInLine;
        }

        if (cursor == dataInLine){
            accumulate(ele, accuData);
        }
        else{
            m_data.append(accuData);
            cursor = dataInLine;
            accuData = ele;
        }
    }
    m_data.append(accuData);
}

QVector<CfgItemMeasBasedE2WashingOutDataEle> &CfgDistanceWashingDataE2Clz::data()
{
    return m_data;
}

void CfgDistanceWashingDataE2Clz::generateData(quint32 idx, QVector<QCPGraphData> &pairs, QString &name, quint8 &motorIdx)
{
    name = m_data[0].getName((quint32)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_DISTANCE_POS) + ':' +  m_data[0].getName(idx),
    motorIdx = m_data[0].getMotorIdx(idx);

    pairs.resize(m_data.size());
    for (int i = 0; i < m_data.size(); i++)
    {
        pairs[i].key   = (quint32)m_data[i].getData((quint32)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_DISTANCE_POS);
        pairs[i].value = m_data[i].getData(idx);
    }
}

CfgItemMeasBasedE2WashingOutDataEle CfgDistanceWashingDataE2Clz::deserialize(const DataJsonRecElement &in)
{
    CfgItemMeasBasedE2WashingOutDataEle ele;
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_VOL_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::VOL_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_THRO_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::THRO1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_DISTANCE_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_ECHODISTANCE_POS));

    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_CUR1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_THU1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_THU1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_TORQUE1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_SPEED1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_MOTOR1TMP1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_MOTOR1TMP1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_MOTOR1TMP2_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_MOTOR1TMP2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_M1EFFICI1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M1EFFICI1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_M1EFFICI2_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M1EFFICI2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_M1POWER, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M1POWER));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_M1MECHAPWR, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M1MECHAPWR));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_M1MECHAEFFI, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M1MECHAEFFI));

    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_CUR2_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_THU2_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_THU2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_TORQUE2_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_SPEED2_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_MOTOR2TMP1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_MOTOR2TMP1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_MOTOR2TMP2_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_MOTOR2TMP2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_M2EFFICI1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M2EFFICI1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_M2EFFICI2_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M2EFFICI2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_M2POWER, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M2POWER));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_M2MECHAPWR, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M2MECHAPWR));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_M2MECHAEFFI, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M2MECHAEFFI));

    return ele;
}

void CfgDistanceWashingDataE2Clz::accumulate(const CfgItemMeasBasedE2WashingOutDataEle &data, CfgItemMeasBasedE2WashingOutDataEle &accu)
{
    for (int i = 0; i < accu.m_metaEle.size(); i++)
    {
        accu.setData(i,(accu.getData(i) + data.getData(i))/2);
    }
}

/*
 * -----------------------------------------------------------------------------
 *  Wash the raw data to the formateed data needed by the Throttle test
 */
CfgThrottleWashingDataE1Clz::CfgThrottleWashingDataE1Clz():
    CfgWashingDataInf(CfgWashingTypeEnum::CFGWASHINGTHROTTLE_E1),
    m_data(0)
{
    CfgItemMeasBasedE1WashingOutDataEle ele;
    m_guiList =  ele.m_metaEle;
}

void CfgThrottleWashingDataE1Clz::wash(const QVector<DataJsonRecElement> & rawdata)
{
    quint32 cursor = std::numeric_limits<unsigned int>::min();
    quint32 dataInLine = 0;
    CfgItemMeasBasedE1WashingOutDataEle accuData;

    for (const DataJsonRecElement& data : rawdata)
    {
        CfgItemMeasBasedE1WashingOutDataEle ele = deserialize(data);
        dataInLine = (quint32)ele.getData((quint32)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_THRO_POS);

        if (cursor == std::numeric_limits<unsigned int>::min())
        {
            cursor = dataInLine;
        }

        if (cursor == dataInLine){
            accumulate(ele, accuData);
        }
        else{
            m_data.append(accuData);
            cursor = dataInLine;
            accuData = ele;
        }
    }
    m_data.append(accuData);
}

QVector<CfgItemMeasBasedE1WashingOutDataEle> &CfgThrottleWashingDataE1Clz::data()
{
    return m_data;
}

void CfgThrottleWashingDataE1Clz::generateData(quint32 idx, QVector<QCPGraphData> &pairs, QString &name, quint8 &motorIdx)
{
    name = m_data[0].getName((quint32)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_THRO_POS) + ':' + m_data[0].getName(idx);
    motorIdx = m_data[0].getMotorIdx(idx);

    pairs.resize(m_data.size());
    for (int i = 0; i < m_data.size(); i++)
    {
        pairs[i].key   = (quint32)m_data[i].getData((quint32)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_THRO_POS);
        pairs[i].value = m_data[i].getData(idx);
    }
}

CfgItemMeasBasedE1WashingOutDataEle CfgThrottleWashingDataE1Clz::deserialize(const DataJsonRecElement &in)
{
    CfgItemMeasBasedE1WashingOutDataEle ele;
    ele.setData( (quint8)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_VOL_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::VOL_POS));
    ele.setData( (quint8)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_THRO_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::THRO1_POS));
    //ele.setData( (quint8)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_DISTANCE_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_ECHODISTANCE_POS));

    ele.setData( (quint8)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_CUR1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_THU1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_THU1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_TORQUE1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_SPEED1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_MOTOR1TMP1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_MOTOR1TMP1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_MOTOR1TMP2_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_MOTOR1TMP2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_M1EFFICI1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M1EFFICI1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_M1EFFICI2_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M1EFFICI2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_M1POWER, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M1POWER));
    ele.setData( (quint8)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_M1MECHAPWR, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M1MECHAPWR));
    ele.setData( (quint8)CfgItemMeasBasedE1WashingOutDataEle::ELEMEASE1CURSOR::REC_M1MECHAEFFI, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M1MECHAEFFI));

    return ele;
}

void CfgThrottleWashingDataE1Clz::accumulate(const CfgItemMeasBasedE1WashingOutDataEle &data, CfgItemMeasBasedE1WashingOutDataEle& accu)
{
    for (int i = 0; i < accu.m_metaEle.size(); i++)
    {
        accu.setData(i,(accu.getData(i) + data.getData(i))/2);
    }
}


/*
 * -----------------------------------------------------------------------------
 *  Wash the raw data to the formateed data needed by the Throttle test
 */
CfgThrottleWashingDataE2Clz::CfgThrottleWashingDataE2Clz():
    CfgWashingDataInf(CfgWashingTypeEnum::CFGWASHINGTHROTTLE_E2),
    m_data(0)
{
    CfgItemMeasBasedE2WashingOutDataEle ele;
    m_guiList =  ele.m_metaEle;
}

void CfgThrottleWashingDataE2Clz::wash(const QVector<DataJsonRecElement> & rawdata)
{
    quint32 cursor = std::numeric_limits<unsigned int>::min();
    quint32 dataInLine = 0;
    CfgItemMeasBasedE2WashingOutDataEle accuData;

    for (const DataJsonRecElement& data : rawdata)
    {
        CfgItemMeasBasedE2WashingOutDataEle ele = deserialize(data);
        dataInLine = (quint32)ele.getData((quint32)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_THRO_POS);

        if (cursor == std::numeric_limits<unsigned int>::min())
        {
            cursor = dataInLine;
        }

        if (cursor == dataInLine){
            accumulate(ele, accuData);
        }
        else{
            m_data.append(accuData);
            cursor = dataInLine;
            accuData = ele;
        }
    }
    m_data.append(accuData);
}

QVector<CfgItemMeasBasedE2WashingOutDataEle> &CfgThrottleWashingDataE2Clz::data()
{
    return m_data;
}

void CfgThrottleWashingDataE2Clz::generateData(quint32 idx, QVector<QCPGraphData> &pairs, QString &name, quint8 &motorIdx)
{   
    name = m_data[0].getName((quint32)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_THRO_POS) + ':' + m_data[0].getName(idx);
    motorIdx = m_data[0].getMotorIdx(idx);

    pairs.resize(m_data.size());
    for (int i = 0; i < m_data.size(); i++)
    {
        pairs[i].key   = (quint32)m_data[i].getData((quint32)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_THRO_POS);
        pairs[i].value = m_data[i].getData(idx);
    }
}

CfgItemMeasBasedE2WashingOutDataEle CfgThrottleWashingDataE2Clz::deserialize(const DataJsonRecElement &in)
{
    CfgItemMeasBasedE2WashingOutDataEle ele;
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_VOL_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::VOL_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_THRO_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::THRO1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_DISTANCE_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_ECHODISTANCE_POS));

    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_CUR1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_THU1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_THU1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_TORQUE1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_SPEED1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_MOTOR1TMP1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_MOTOR1TMP1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_MOTOR1TMP2_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_MOTOR1TMP2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_M1EFFICI1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M1EFFICI1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_M1EFFICI2_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M1EFFICI2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_M1POWER, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M1POWER));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_M1MECHAPWR, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M1MECHAPWR));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_M1MECHAEFFI, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M1MECHAEFFI));

    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_CUR2_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_THU2_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_THU2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_TORQUE2_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_SPEED2_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_MOTOR2TMP1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_MOTOR2TMP1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_MOTOR2TMP2_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_MOTOR2TMP2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_M2EFFICI1_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M2EFFICI1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_M2EFFICI2_POS, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M2EFFICI2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_M2POWER, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M2POWER));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_M2MECHAPWR, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M2MECHAPWR));
    ele.setData( (quint8)CfgItemMeasBasedE2WashingOutDataEle::ELEMEASCURSOR::REC_M2MECHAEFFI, in.getData((quint32)DataJsonRecElement::ELEMCURSOR::REC_M2MECHAEFFI));
    return ele;
}

void CfgThrottleWashingDataE2Clz::accumulate(const CfgItemMeasBasedE2WashingOutDataEle &data, CfgItemMeasBasedE2WashingOutDataEle& accu)
{
    for (int i = 0; i < accu.m_metaEle.size(); i++)
    {
        accu.setData(i,(accu.getData(i) + data.getData(i))/2);
    }
}

//-------------------------------------------------

CfgMultiWashingDataE1Clz::CfgMultiWashingDataE1Clz():
    CfgWashingDataInf(CfgWashingTypeEnum::CFGWASHINGMULTI_E1),
    m_data(0)

{
    AsixDataItem axisItemX;
    AsixDataItem axisItemY;
    CfgMultiWashingDataItem item;

    QVector<CfgMultiWashingDataItem> itemList;

    //-------=  -1 = -----------------------
    do
    {
        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemX.str = TestUnitName::THROTTLE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR1_POS;
        axisItemY.str = TestUnitName::CURRENT();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemX.str = TestUnitName::THROTTLE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THU1_POS;
        axisItemY.str = TestUnitName::THRUST();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemX.str = TestUnitName::THROTTLE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE1_POS;
        axisItemY.str = TestUnitName::TORQUE();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemX.str = TestUnitName::THROTTLE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED1_POS;
        axisItemY.str = TestUnitName::SPEED();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemX.str = TestUnitName::THROTTLE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_MOTOR1TMP1_POS;
        axisItemY.str = TestUnitName::TEMP1();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemX.str = TestUnitName::THROTTLE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_MOTOR1TMP2_POS;
        axisItemY.str = TestUnitName::TEMP2();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemX.str = TestUnitName::THROTTLE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1EFFICI1_POS;
        axisItemY.str = TestUnitName::EFFI_POWER();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemX.str = TestUnitName::THROTTLE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1EFFICI2_POS;
        axisItemY.str = TestUnitName::EFFI_ELE();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemX.str = TestUnitName::THROTTLE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1POWER;
        axisItemY.str = TestUnitName::POWER();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemX.str = TestUnitName::THROTTLE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1MECHAPWR;
        axisItemY.str = TestUnitName::MECHAPWR();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemX.str = TestUnitName::THROTTLE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1MECHAEFFI;
        axisItemY.str = TestUnitName::MECHAEFFI();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);


    }while(0);

    //-----------= -1 =-----------------------------
    m_data.append(itemList);
    itemList.clear();

    //-------=  0 = -----------------------
    do
    {
        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR1_POS;
        axisItemX.str = TestUnitName::CURRENT();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemY.str = TestUnitName::THROTTLE();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR1_POS;
        axisItemX.str = TestUnitName::CURRENT();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THU1_POS;
        axisItemY.str = TestUnitName::THRUST();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR1_POS;
        axisItemX.str = TestUnitName::CURRENT();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE1_POS;
        axisItemY.str = TestUnitName::TORQUE();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR1_POS;
        axisItemX.str = TestUnitName::CURRENT();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED1_POS;
        axisItemY.str = TestUnitName::SPEED();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR1_POS;
        axisItemX.str = TestUnitName::CURRENT();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1EFFICI1_POS;
        axisItemY.str = TestUnitName::EFFI_POWER();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR1_POS;
        axisItemX.str = TestUnitName::CURRENT();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1EFFICI2_POS;
        axisItemY.str = TestUnitName::EFFI_ELE();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR1_POS;
        axisItemX.str = TestUnitName::CURRENT();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1POWER;
        axisItemY.str = TestUnitName::POWER();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR1_POS;
        axisItemX.str = TestUnitName::CURRENT();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1MECHAPWR;
        axisItemY.str = TestUnitName::MECHAPWR();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR1_POS;
        axisItemX.str = TestUnitName::CURRENT();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1MECHAEFFI;
        axisItemY.str = TestUnitName::MECHAEFFI();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

    }while(0);

    //-----------= 0 =-----------------------------
    m_data.append(itemList);
    itemList.clear();

    //-------=  1 = -----------------------
    do
    {
        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THU1_POS;
        axisItemX.str = TestUnitName::THRUST();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemY.str = TestUnitName::THROTTLE();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THU1_POS;
        axisItemX.str = TestUnitName::THRUST();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR1_POS;
        axisItemY.str = TestUnitName::CURRENT();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THU1_POS;
        axisItemX.str = TestUnitName::THRUST();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE1_POS;
        axisItemY.str = TestUnitName::TORQUE();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THU1_POS;
        axisItemX.str = TestUnitName::THRUST();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED1_POS;
        axisItemY.str = TestUnitName::SPEED();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THU1_POS;
        axisItemX.str = TestUnitName::THRUST();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1EFFICI1_POS;
        axisItemY.str = TestUnitName::EFFI_POWER();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THU1_POS;
        axisItemX.str = TestUnitName::THRUST();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1EFFICI2_POS;
        axisItemY.str = TestUnitName::EFFI_ELE();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THU1_POS;
        axisItemX.str = TestUnitName::THRUST();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1POWER;
        axisItemY.str = TestUnitName::POWER();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THU1_POS;
        axisItemX.str = TestUnitName::THRUST();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1MECHAPWR;
        axisItemY.str = TestUnitName::MECHAPWR();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THU1_POS;
        axisItemX.str = TestUnitName::THRUST();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1MECHAEFFI;
        axisItemY.str = TestUnitName::MECHAEFFI();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

    }while(0);
    //-----------= 1 =-----------------------------
    m_data.append(itemList);
    itemList.clear();

    //-------=  2 = -----------------------
    do
    {
        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE1_POS;
        axisItemX.str = TestUnitName::TORQUE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemY.str = TestUnitName::THROTTLE();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE1_POS;
        axisItemX.str = TestUnitName::TORQUE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR1_POS;
        axisItemY.str = TestUnitName::CURRENT();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE1_POS;
        axisItemX.str = TestUnitName::TORQUE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THU1_POS;
        axisItemY.str = TestUnitName::THRUST();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE1_POS;
        axisItemX.str = TestUnitName::TORQUE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED1_POS;
        axisItemY.str = TestUnitName::SPEED();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE1_POS;
        axisItemX.str = TestUnitName::TORQUE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1EFFICI1_POS;
        axisItemY.str = TestUnitName::EFFI_POWER();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE1_POS;
        axisItemX.str = TestUnitName::TORQUE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1EFFICI2_POS;
        axisItemY.str = TestUnitName::EFFI_ELE();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE1_POS;
        axisItemX.str = TestUnitName::TORQUE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1POWER;
        axisItemY.str = TestUnitName::POWER();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE1_POS;
        axisItemX.str = TestUnitName::TORQUE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1MECHAPWR;
        axisItemY.str = TestUnitName::MECHAPWR();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE1_POS;
        axisItemX.str = TestUnitName::TORQUE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1MECHAEFFI;
        axisItemY.str = TestUnitName::MECHAEFFI();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

    }while(0);
    //-----------= 2 =-----------------------------
    m_data.append(itemList);
    itemList.clear();

    //-------=  3 = -----------------------
    do
    {
        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED1_POS;
        axisItemX.str = TestUnitName::SPEED();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemY.str = TestUnitName::THROTTLE();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED1_POS;
        axisItemX.str = TestUnitName::SPEED();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR1_POS;
        axisItemY.str = TestUnitName::CURRENT();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED1_POS;
        axisItemX.str = TestUnitName::SPEED();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THU1_POS;
        axisItemY.str = TestUnitName::THRUST();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED1_POS;
        axisItemX.str = TestUnitName::SPEED();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE1_POS;
        axisItemY.str = TestUnitName::TORQUE();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED1_POS;
        axisItemX.str = TestUnitName::SPEED();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1EFFICI1_POS;
        axisItemY.str = TestUnitName::EFFI_POWER();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED1_POS;
        axisItemX.str = TestUnitName::SPEED();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1EFFICI2_POS;
        axisItemY.str = TestUnitName::EFFI_ELE();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED1_POS;
        axisItemX.str = TestUnitName::SPEED();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1POWER;
        axisItemY.str = TestUnitName::POWER();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED1_POS;
        axisItemX.str = TestUnitName::SPEED();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1MECHAPWR;
        axisItemY.str = TestUnitName::MECHAPWR();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED1_POS;
        axisItemX.str = TestUnitName::SPEED();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1MECHAEFFI;
        axisItemY.str = TestUnitName::MECHAEFFI();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

    }while(0);
    //-----------= 3 =-----------------------------
    m_data.append(itemList);
    itemList.clear();


    //-----------= GUI =---------------------------
    m_guiList.clear();
    m_guiList.resize(m_data.size());

    quint32 idxItemList = 0;
    for (const QVector<CfgMultiWashingDataItem>& itemList : m_data)
    {
        for (const CfgMultiWashingDataItem& item : itemList)
        {
            //QStringList list({item.XKey.str, item.YValue.str, QString::number(item.motorIdx)});
            //list << item.XKey.str << item.YValue.str << QString::number(item.motorIdx);
            //m_guiList[idxItemList].append(list.join(':'));

            QStringList axislist({item.XKey.str, item.YValue.str});
            //QStringList list({axislist.join('-'), QString::number(item.motorIdx)});
            //list << item.XKey.str << item.YValue.str << QString::number(item.motorIdx);
            //m_guiList[idxItemList].append(list.join(':'));
            m_guiList[idxItemList].append(CfgMetaElement(axislist.join('-'), item.motorIdx));
        }
        idxItemList++;
    }
}

void CfgMultiWashingDataE1Clz::wash(const QVector<DataJsonRecElement> & rawdata)
{
    for (QVector<CfgMultiWashingDataItem>& itemList : m_data)
    {
        for (CfgMultiWashingDataItem& item : itemList)
        {
            quint32 xIdx = item.XKey.idx;
            quint32 yIdx = item.YValue.idx;
            QMultiMap<double, double> multiMap;

            for (const DataJsonRecElement& data : rawdata)
            {
                double key = data.getData(xIdx);
                double value = data.getData(yIdx);
                //value = (map.value(key, value) + value)/2;
                multiMap.insert(key, value);
            }

            QMap<double, double> map;
            for (const double uniqueKey : multiMap.uniqueKeys())
            {
                QList<double> valueArrays = multiMap.values(uniqueKey);
                double uniqueValue = weightValues(valueArrays);
                map.insert(uniqueKey, uniqueValue);
                //qDebug() << "map insert unique = " << uniqueKey << "valueArrays" << uniqueValue;
            }

            item.data.clear();
            auto iter = map.cbegin();
            while (iter != map.cend())
            {
                //qDebug() << "x = " << iter.key() << "y" << iter.value();
                item.data.append(KV(iter.key(), iter.value()));

                ++iter;
            }
        }
    }
}

//QVector<CfgMultiWashingDataItem> &CfgMultiWashingDataE2Clz::data()
//{
//    return m_data;
//}

void CfgMultiWashingDataE1Clz::generateData(quint32 composeIdx, QVector<QCPGraphData> &pairs, QString &name, quint8 &motorIdx)
{
    quint32 itemListIdx = (0xFFFF0000 & composeIdx) >> 16;
    quint32 idx         = (0x0000FFFF & composeIdx);

    if (itemListIdx >= m_data.size())
    {
        qWarning() << "Warning: out of size of array, request = " << idx << "size of container"
                   << m_data.size();
        return;
    }
    QVector<CfgMultiWashingDataItem>& itemList = m_data[itemListIdx];

    if (idx < (quint32)itemList.size())
    {
        motorIdx = itemList[idx].motorIdx;
        name = itemList[idx].XKey.str + ":" + itemList[idx].YValue.str;

        const quint32 size = itemList[idx].data.size();

        std::shared_ptr<double> xAsix(new double[size]);
        std::shared_ptr<double> yAsix(new double[size]);

        do {
            const quint32 order = 5;
            double coefficients[order+1]; // y = ax*x + bx + c;

            double xAsixFloor = itemList[idx].data.first().key;
            double xAsixCeil = itemList[idx].data.last().key;

            if (xAsixFloor > xAsixCeil)
            {
                double temp = xAsixFloor;
                xAsixFloor = xAsixCeil;
                xAsixCeil = temp;
            }

            quint32 eleIdx = 0;
            for (const KV& ele : itemList[idx].data)
            {
                double* xValue = xAsix.get();
                xValue[eleIdx] = ele.key;
                double* yValue = yAsix.get();
                yValue[eleIdx] = ele.value;

                eleIdx++;
            }

            PolyFitClz::plotfit(xAsix.get(), yAsix.get(), size, order, &coefficients[0]);

            const quint32 trimSize = size < MAX_POINT_XASIX ? size : MAX_POINT_XASIX;
            double step = (xAsixCeil - xAsixFloor)/trimSize;

            for (double x = xAsixFloor; x < xAsixCeil; x+=step)
            {
                double y = coefficients[5] * x * x * x * x * x
                         + coefficients[4] * x * x * x * x
                         + coefficients[3] * x * x * x
                         + coefficients[2] * x * x
                         + coefficients[1] * x
                         + coefficients[0];

                pairs.append(QCPGraphData((double)x, y));
                //qDebug() << "plotfit insert x=" << x << "y=" << y << "step=" << step;
            }
        }while(0);
    }
    else
    {
        qWarning() << "Warning: out of size of array, request = " << idx << "size of container"
                   << itemList.size();
    }
}

bool CfgMultiWashingDataE1Clz::dump(const QString filename)
{
    /* loop all the basic type */
    for (const QVector<CfgMultiWashingDataItem>& itemList : m_data)
    {
        quint32 idx = 0;
        QVector<QVector<QCPGraphData>> pairsFull(itemList.size());

        while (idx < (quint32)itemList.size())
        {
            QVector<QCPGraphData>& pairs = pairsFull[idx];

            //int motorIdx = itemList[idx].motorIdx;
            QString name = itemList[idx].XKey.str + "-" + itemList[idx].YValue.str;

            const quint32 size = itemList[idx].data.size();

            std::shared_ptr<double> xAsix(new double[size]);
            std::shared_ptr<double> yAsix(new double[size]);

            do {
                const quint32 order = 5;
                double coefficients[order+1]; // y = ax*x + bx + c;

                double xAsixFloor = itemList[idx].data.first().key;
                double xAsixCeil = itemList[idx].data.last().key;

                if (xAsixFloor > xAsixCeil)
                {
                    double temp = xAsixFloor;
                    xAsixFloor = xAsixCeil;
                    xAsixCeil = temp;
                }

                quint32 eleIdx = 0;
                for (const KV& ele : itemList[idx].data)
                {
                    double* xValue = xAsix.get();
                    xValue[eleIdx] = ele.key;
                    double* yValue = yAsix.get();
                    yValue[eleIdx] = ele.value;

                    eleIdx++;
                }

                PolyFitClz::plotfit(xAsix.get(), yAsix.get(), size, order, &coefficients[0]);

                const quint32 trimSize = size < MAX_POINT_XASIX ? size : MAX_POINT_XASIX;
                double step = (xAsixCeil - xAsixFloor)/trimSize;

                for (double x = xAsixFloor; x < xAsixCeil; x+=step)
                {
                    double y = coefficients[5] * x * x * x * x * x
                            + coefficients[4] * x * x * x * x
                            + coefficients[3] * x * x * x
                            + coefficients[2] * x * x
                            + coefficients[1] * x
                            + coefficients[0];

                    pairs.append(QCPGraphData((double)x, y));
                    //qDebug() << "plotfit insert x=" << x << "y=" << y << "step=" << step;
                }
            }while(0);
            // incremental the idx;
            ++idx;
        }

        // filename with .csv
        int left = filename.size() - 4;
        QString saveFileName =
                filename.left(left) + QStringLiteral("_") + itemList.at(0).XKey.str;
        saveFileName = saveFileName + QStringLiteral(".csv");

        QFile file(saveFileName);
        if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
            return false;

        QTextStream out(&file);
        /* write to the data to the same type of data*/
        QStringList data;

        // print headers
        data << itemList.at(0).XKey.str;
        for (const CfgMultiWashingDataItem& item : itemList)
        {
            data << item.YValue.str;
        }
        out << data.join(',') << QChar('\n');
        data.clear();

        // print data
        int size = pairsFull.at(0).size();

        for (int col = 0; col < size; ++col)
        {
            QStringList line;

            line << QString::number(pairsFull.at(0).at(col).key);
            for (int raw = 0; raw < pairsFull.size(); ++raw)
            {
                double value = pairsFull.at(raw).at(col).value;
                line << QString::number(value);
            }
            out << line.join(',') << QChar('\n');
        }
        file.close();
    }
    return true;
}

/*
 * We can make more functionality here. --eshenhu -Dec-30-16
 */
double CfgMultiWashingDataE1Clz::weightValues(QList<double> &values)
{
    quint32 size = values.size();
    double result;
    if (size == 0)
    {
        result = 0;
    }
    else if (size == 1)
    {
        result = values.value(0);
    }
    else if (size == 2)
    {
        result = (values.value(0) + values.value(1))/2;
    }
    else
    {
        qSort(values);
        double cal = 0.0;
        for (double& inner : values)
        {
            cal += inner;
        }
        cal -= values.first();
        cal -= values.last();
        result = cal/(size-2);
    }
    return result;
}

//-------------------------------------------------

CfgMultiWashingDataE2Clz::CfgMultiWashingDataE2Clz():
    CfgWashingDataInf(CfgWashingTypeEnum::CFGWASHINGMULTI_E2),
    m_data(0)

{
    AsixDataItem axisItemX;
    AsixDataItem axisItemY;
    CfgMultiWashingDataItem item;

    QVector<CfgMultiWashingDataItem> itemList;

    //-------=  -1 = -----------------------
    do
    {
        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemX.str = TestUnitName::THROTTLE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR1_POS;
        axisItemY.str = TestUnitName::CURRENT();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemX.str = TestUnitName::THROTTLE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THU1_POS;
        axisItemY.str = TestUnitName::THRUST();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemX.str = TestUnitName::THROTTLE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE1_POS;
        axisItemY.str = TestUnitName::TORQUE();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemX.str = TestUnitName::THROTTLE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED1_POS;
        axisItemY.str = TestUnitName::SPEED();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemX.str = TestUnitName::THROTTLE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_MOTOR1TMP1_POS;
        axisItemY.str = TestUnitName::TEMP1();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemX.str = TestUnitName::THROTTLE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_MOTOR1TMP2_POS;
        axisItemY.str = TestUnitName::TEMP2();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemX.str = TestUnitName::THROTTLE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1EFFICI1_POS;
        axisItemY.str = TestUnitName::EFFI_POWER();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemX.str = TestUnitName::THROTTLE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1EFFICI2_POS;
        axisItemY.str = TestUnitName::EFFI_ELE();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemX.str = TestUnitName::THROTTLE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1POWER;
        axisItemY.str = TestUnitName::POWER();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemX.str = TestUnitName::THROTTLE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1MECHAPWR;
        axisItemY.str = TestUnitName::MECHAPWR();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemX.str = TestUnitName::THROTTLE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1MECHAEFFI;
        axisItemY.str = TestUnitName::MECHAEFFI();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        // motor 1
        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemX.str = TestUnitName::THROTTLE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR2_POS;
        axisItemY.str = TestUnitName::CURRENT();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemX.str = TestUnitName::THROTTLE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THU2_POS;
        axisItemY.str = TestUnitName::THRUST();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemX.str = TestUnitName::THROTTLE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE2_POS;
        axisItemY.str = TestUnitName::TORQUE();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemX.str = TestUnitName::THROTTLE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED2_POS;
        axisItemY.str = TestUnitName::SPEED();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemX.str = TestUnitName::THROTTLE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_MOTOR2TMP1_POS;
        axisItemY.str = TestUnitName::TEMP1();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemX.str = TestUnitName::THROTTLE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_MOTOR2TMP2_POS;
        axisItemY.str = TestUnitName::TEMP2();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemX.str = TestUnitName::THROTTLE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M2EFFICI1_POS;
        axisItemY.str = TestUnitName::EFFI_POWER();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemX.str = TestUnitName::THROTTLE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M2EFFICI2_POS;
        axisItemY.str = TestUnitName::EFFI_ELE();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemX.str = TestUnitName::THROTTLE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M2POWER;
        axisItemY.str = TestUnitName::POWER();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemX.str = TestUnitName::THROTTLE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M2MECHAPWR;
        axisItemY.str = TestUnitName::MECHAPWR();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemX.str = TestUnitName::THROTTLE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M2MECHAEFFI;
        axisItemY.str = TestUnitName::MECHAEFFI();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

    }while(0);

    //-----------= -1 =-----------------------------
    m_data.append(itemList);
    itemList.clear();

    //-------=  0 = -----------------------
    do
    {
        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR1_POS;
        axisItemX.str = TestUnitName::CURRENT();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemY.str = TestUnitName::THROTTLE();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR1_POS;
        axisItemX.str = TestUnitName::CURRENT();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THU1_POS;
        axisItemY.str = TestUnitName::THRUST();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR1_POS;
        axisItemX.str = TestUnitName::CURRENT();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE1_POS;
        axisItemY.str = TestUnitName::TORQUE();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR1_POS;
        axisItemX.str = TestUnitName::CURRENT();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED1_POS;
        axisItemY.str = TestUnitName::SPEED();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR1_POS;
        axisItemX.str = TestUnitName::CURRENT();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1EFFICI1_POS;
        axisItemY.str = TestUnitName::EFFI_POWER();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR1_POS;
        axisItemX.str = TestUnitName::CURRENT();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1EFFICI2_POS;
        axisItemY.str = TestUnitName::EFFI_ELE();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR1_POS;
        axisItemX.str = TestUnitName::CURRENT();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1POWER;
        axisItemY.str = TestUnitName::POWER();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR1_POS;
        axisItemX.str = TestUnitName::CURRENT();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1MECHAPWR;
        axisItemY.str = TestUnitName::MECHAPWR();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR1_POS;
        axisItemX.str = TestUnitName::CURRENT();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1MECHAEFFI;
        axisItemY.str = TestUnitName::MECHAEFFI();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR2_POS;
        axisItemX.str = TestUnitName::CURRENT();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemY.str = TestUnitName::THROTTLE();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR2_POS;
        axisItemX.str = TestUnitName::CURRENT();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THU2_POS;
        axisItemY.str = TestUnitName::THRUST();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR2_POS;
        axisItemX.str = TestUnitName::CURRENT();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE2_POS;
        axisItemY.str = TestUnitName::TORQUE();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR2_POS;
        axisItemX.str = TestUnitName::CURRENT();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED2_POS;
        axisItemY.str = TestUnitName::SPEED();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR2_POS;
        axisItemX.str = TestUnitName::CURRENT();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M2EFFICI1_POS;
        axisItemY.str = TestUnitName::EFFI_POWER();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR2_POS;
        axisItemX.str = TestUnitName::CURRENT();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M2EFFICI2_POS;
        axisItemY.str = TestUnitName::EFFI_ELE();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);
        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR2_POS;
        axisItemX.str = TestUnitName::CURRENT();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M2POWER;
        axisItemY.str = TestUnitName::POWER();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR2_POS;
        axisItemX.str = TestUnitName::CURRENT();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M2MECHAPWR;
        axisItemY.str = TestUnitName::MECHAPWR();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR2_POS;
        axisItemX.str = TestUnitName::CURRENT();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M2MECHAEFFI;
        axisItemY.str = TestUnitName::MECHAEFFI();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

    }while(0);

    //-----------= 0 =-----------------------------
    m_data.append(itemList);
    itemList.clear();

    //-------=  1 = -----------------------
    do
    {
        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THU1_POS;
        axisItemX.str = TestUnitName::THRUST();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemY.str = TestUnitName::THROTTLE();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THU1_POS;
        axisItemX.str = TestUnitName::THRUST();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR1_POS;
        axisItemY.str = TestUnitName::CURRENT();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THU1_POS;
        axisItemX.str = TestUnitName::THRUST();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE1_POS;
        axisItemY.str = TestUnitName::TORQUE();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THU1_POS;
        axisItemX.str = TestUnitName::THRUST();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED1_POS;
        axisItemY.str = TestUnitName::SPEED();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THU1_POS;
        axisItemX.str = TestUnitName::THRUST();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1EFFICI1_POS;
        axisItemY.str = TestUnitName::EFFI_POWER();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THU1_POS;
        axisItemX.str = TestUnitName::THRUST();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1EFFICI2_POS;
        axisItemY.str = TestUnitName::EFFI_ELE();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THU1_POS;
        axisItemX.str = TestUnitName::THRUST();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1POWER;
        axisItemY.str = TestUnitName::POWER();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THU1_POS;
        axisItemX.str = TestUnitName::THRUST();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1MECHAPWR;
        axisItemY.str = TestUnitName::MECHAPWR();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THU1_POS;
        axisItemX.str = TestUnitName::THRUST();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1MECHAEFFI;
        axisItemY.str = TestUnitName::MECHAEFFI();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        //---- motot -1 ----------
        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THU2_POS;
        axisItemX.str = TestUnitName::THRUST();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemY.str = TestUnitName::THROTTLE();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THU2_POS;
        axisItemX.str = TestUnitName::THRUST();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR2_POS;
        axisItemY.str = TestUnitName::CURRENT();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THU2_POS;
        axisItemX.str = TestUnitName::THRUST();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE2_POS;
        axisItemY.str = TestUnitName::TORQUE();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THU2_POS;
        axisItemX.str = TestUnitName::THRUST();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED2_POS;
        axisItemY.str = TestUnitName::SPEED();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THU2_POS;
        axisItemX.str = TestUnitName::THRUST();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M2EFFICI1_POS;
        axisItemY.str = TestUnitName::EFFI_POWER();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THU2_POS;
        axisItemX.str = TestUnitName::THRUST();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M2EFFICI2_POS;
        axisItemY.str = TestUnitName::EFFI_ELE();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THU2_POS;
        axisItemX.str = TestUnitName::THRUST();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M2POWER;
        axisItemY.str = TestUnitName::POWER();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THU2_POS;
        axisItemX.str = TestUnitName::THRUST();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M2MECHAPWR;
        axisItemY.str = TestUnitName::MECHAPWR();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THU2_POS;
        axisItemX.str = TestUnitName::THRUST();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M2MECHAEFFI;
        axisItemY.str = TestUnitName::MECHAEFFI();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);
    }while(0);
    //-----------= 1 =-----------------------------
    m_data.append(itemList);
    itemList.clear();

    //-------=  2 = -----------------------
    do
    {
        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE1_POS;
        axisItemX.str = TestUnitName::TORQUE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemY.str = TestUnitName::THROTTLE();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE1_POS;
        axisItemX.str = TestUnitName::TORQUE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR1_POS;
        axisItemY.str = TestUnitName::CURRENT();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE1_POS;
        axisItemX.str = TestUnitName::TORQUE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THU1_POS;
        axisItemY.str = TestUnitName::THRUST();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE1_POS;
        axisItemX.str = TestUnitName::TORQUE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED1_POS;
        axisItemY.str = TestUnitName::SPEED();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE1_POS;
        axisItemX.str = TestUnitName::TORQUE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1EFFICI1_POS;
        axisItemY.str = TestUnitName::EFFI_POWER();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE1_POS;
        axisItemX.str = TestUnitName::TORQUE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1EFFICI2_POS;
        axisItemY.str = TestUnitName::EFFI_ELE();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE1_POS;
        axisItemX.str = TestUnitName::TORQUE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1POWER;
        axisItemY.str = TestUnitName::POWER();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE1_POS;
        axisItemX.str = TestUnitName::TORQUE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1MECHAPWR;
        axisItemY.str = TestUnitName::MECHAPWR();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE1_POS;
        axisItemX.str = TestUnitName::TORQUE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1MECHAEFFI;
        axisItemY.str = TestUnitName::MECHAEFFI();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        //---- motot -1 ----------
        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE2_POS;
        axisItemX.str = TestUnitName::TORQUE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemY.str = TestUnitName::THROTTLE();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE2_POS;
        axisItemX.str = TestUnitName::TORQUE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR2_POS;
        axisItemY.str = TestUnitName::CURRENT();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE2_POS;
        axisItemX.str = TestUnitName::TORQUE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THU2_POS;
        axisItemY.str = TestUnitName::THRUST();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE2_POS;
        axisItemX.str = TestUnitName::TORQUE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED2_POS;
        axisItemY.str = TestUnitName::SPEED();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE2_POS;
        axisItemX.str = TestUnitName::TORQUE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M2EFFICI1_POS;
        axisItemY.str = TestUnitName::EFFI_POWER();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE2_POS;
        axisItemX.str = TestUnitName::TORQUE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M2EFFICI2_POS;
        axisItemY.str = TestUnitName::EFFI_ELE();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE2_POS;
        axisItemX.str = TestUnitName::TORQUE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M2POWER;
        axisItemY.str = TestUnitName::POWER();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE2_POS;
        axisItemX.str = TestUnitName::TORQUE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M2MECHAPWR;
        axisItemY.str = TestUnitName::MECHAPWR();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE2_POS;
        axisItemX.str = TestUnitName::TORQUE();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M2MECHAEFFI;
        axisItemY.str = TestUnitName::MECHAEFFI();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);
    }while(0);
    //-----------= 2 =-----------------------------
    m_data.append(itemList);
    itemList.clear();

    //-------=  3 = -----------------------
    do
    {
        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED1_POS;
        axisItemX.str = TestUnitName::SPEED();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemY.str = TestUnitName::THROTTLE();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED1_POS;
        axisItemX.str = TestUnitName::SPEED();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR1_POS;
        axisItemY.str = TestUnitName::CURRENT();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED1_POS;
        axisItemX.str = TestUnitName::SPEED();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THU1_POS;
        axisItemY.str = TestUnitName::THRUST();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED1_POS;
        axisItemX.str = TestUnitName::SPEED();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE1_POS;
        axisItemY.str = TestUnitName::TORQUE();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED1_POS;
        axisItemX.str = TestUnitName::SPEED();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1EFFICI1_POS;
        axisItemY.str = TestUnitName::EFFI_POWER();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED1_POS;
        axisItemX.str = TestUnitName::SPEED();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1EFFICI2_POS;
        axisItemY.str = TestUnitName::EFFI_ELE();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED1_POS;
        axisItemX.str = TestUnitName::SPEED();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1POWER;
        axisItemY.str = TestUnitName::POWER();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED1_POS;
        axisItemX.str = TestUnitName::SPEED();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1MECHAPWR;
        axisItemY.str = TestUnitName::MECHAPWR();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 0;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED1_POS;
        axisItemX.str = TestUnitName::SPEED();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M1MECHAEFFI;
        axisItemY.str = TestUnitName::MECHAEFFI();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        //---- motot -1 ----------
        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED2_POS;
        axisItemX.str = TestUnitName::SPEED();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THRO_POS;
        axisItemY.str = TestUnitName::THROTTLE();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED2_POS;
        axisItemX.str = TestUnitName::SPEED();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_CUR2_POS;
        axisItemY.str = TestUnitName::CURRENT();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED2_POS;
        axisItemX.str = TestUnitName::SPEED();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_THU2_POS;
        axisItemY.str = TestUnitName::THRUST();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED2_POS;
        axisItemX.str = TestUnitName::SPEED();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_TORQUE2_POS;
        axisItemY.str = TestUnitName::TORQUE();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED2_POS;
        axisItemX.str = TestUnitName::SPEED();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M2EFFICI1_POS;
        axisItemY.str = TestUnitName::EFFI_POWER();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED2_POS;
        axisItemX.str = TestUnitName::SPEED();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M2EFFICI2_POS;
        axisItemY.str = TestUnitName::EFFI_ELE();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED2_POS;
        axisItemX.str = TestUnitName::SPEED();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M2POWER;
        axisItemY.str = TestUnitName::POWER();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED2_POS;
        axisItemX.str = TestUnitName::SPEED();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M2MECHAPWR;
        axisItemY.str = TestUnitName::MECHAPWR();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);

        item.motorIdx = 1;
        axisItemX.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_SPEED2_POS;
        axisItemX.str = TestUnitName::SPEED();
        axisItemY.idx = (quint32)DataJsonRecElement::ELEMCURSOR::REC_M2MECHAEFFI;
        axisItemY.str = TestUnitName::MECHAEFFI();

        item.XKey = axisItemX;
        item.YValue = axisItemY;
        itemList.append(item);
    }while(0);
    //-----------= 3 =-----------------------------
    m_data.append(itemList);
    itemList.clear();


    //-----------= GUI =---------------------------
    m_guiList.clear();
    m_guiList.resize(m_data.size());

    quint32 idxItemList = 0;
    for (const QVector<CfgMultiWashingDataItem>& itemList : m_data)
    {
        for (const CfgMultiWashingDataItem& item : itemList)
        {          
            QStringList axislist({item.XKey.str, item.YValue.str});
            //QStringList list({axislist.join('-'), QString::number(item.motorIdx)});
            //list << item.XKey.str << item.YValue.str << QString::number(item.motorIdx);
            //m_guiList[idxItemList].append(list.join(':'));
            m_guiList[idxItemList].append(CfgMetaElement(axislist.join('-'), item.motorIdx));
        }
        idxItemList++;
    }


}

void CfgMultiWashingDataE2Clz::wash(const QVector<DataJsonRecElement> & rawdata)
{
    for (QVector<CfgMultiWashingDataItem>& itemList : m_data)
    {
        for (CfgMultiWashingDataItem& item : itemList)
        {
            quint32 xIdx = item.XKey.idx;
            quint32 yIdx = item.YValue.idx;
            QMultiMap<double, double> multiMap;

            for (const DataJsonRecElement& data : rawdata)
            {
                double key = data.getData(xIdx);
                double value = data.getData(yIdx);
                //value = (map.value(key, value) + value)/2;
                multiMap.insert(key, value);
            }

            QMap<double, double> map;
            for (const double uniqueKey : multiMap.uniqueKeys())
            {
                QList<double> valueArrays = multiMap.values(uniqueKey);
                double uniqueValue = weightValues(valueArrays);
                map.insert(uniqueKey, uniqueValue);
                //qDebug() << "map insert unique = " << uniqueKey << "valueArrays" << uniqueValue;
            }

            item.data.clear();
            auto iter = map.cbegin();
            while (iter != map.cend())
            {
                //qDebug() << "x = " << iter.key() << "y" << iter.value();
                item.data.append(KV(iter.key(), iter.value()));

                ++iter;
            }
        }
    }
}

//QVector<CfgMultiWashingDataItem> &CfgMultiWashingDataE2Clz::data()
//{
//    return m_data;
//}

void CfgMultiWashingDataE2Clz::generateData(quint32 composeIdx, QVector<QCPGraphData> &pairs, QString &name, quint8 &motorIdx)
{
    quint32 itemListIdx = (0xFFFF0000 & composeIdx) >> 16;
    quint32 idx         = (0x0000FFFF & composeIdx);

    if (itemListIdx >= m_data.size())
    {
        qWarning() << "Warning: out of size of array, request = " << idx << "size of container"
                   << m_data.size();
        return;
    }
    QVector<CfgMultiWashingDataItem>& itemList = m_data[itemListIdx];

    if (idx < (quint32)itemList.size())
    {
        motorIdx = itemList[idx].motorIdx;
        name = itemList[idx].XKey.str + ":" + itemList[idx].YValue.str;

        const quint32 size = itemList[idx].data.size();

        std::shared_ptr<double> xAsix(new double[size]);
        std::shared_ptr<double> yAsix(new double[size]);

        do {
            const quint32 order = 5;
            double coefficients[order+1]; // y = ax*x + bx + c;

            qint32 xAsixFloor = qFloor(itemList[idx].data.first().key);
            qint32 xAsixCeil = qCeil(itemList[idx].data.last().key);

            quint32 eleIdx = 0;
            for (const KV& ele : itemList[idx].data)
            {
                double* xValue = xAsix.get();
                xValue[eleIdx] = ele.key;
                double* yValue = yAsix.get();
                yValue[eleIdx] = ele.value;

                eleIdx++;
            }

//            double* pX = xAsix.get();
//            double* pY = yAsix.get();

//            for (int x = 0; x < size; x++)
//            {
//                qDebug() << "cfgwashingdatainf x=" << *(pX+x) << "y=" << *(pY+x);
//            }

            PolyFitClz::plotfit(xAsix.get(), yAsix.get(), size, order, &coefficients[0]);

            const quint32 trimSize = size < MAX_POINT_XASIX ? size : MAX_POINT_XASIX;
            qint32 step = (xAsixCeil - xAsixFloor)/trimSize;
            step = (step == 0 ? 1 : step);
            for (qint32 x = xAsixFloor; x < xAsixCeil; x+=step)
            {
                double y = coefficients[5] * x * x * x * x * x
                         + coefficients[4] * x * x * x * x
                         + coefficients[3] * x * x * x
                         + coefficients[2] * x * x
                         + coefficients[1] * x
                         + coefficients[0];
//                double y = coefficients[4] * x * x * x * x
//                         + coefficients[3] * x * x * x
//                         + coefficients[2] * x * x
//                         + coefficients[1] * x
//                         + coefficients[0];
                pairs.append(QCPGraphData((double)x, y));
                //qDebug() << "plotfit insert x=" << x << "y=" << y << "step=" << step;
            }
        }while(0);
    }
    else
    {
        qWarning() << "Warning: out of size of array, request = " << idx << "size of container"
                   << itemList.size();
    }
}

/*
 * We can make more functionality here. --eshenhu -Dec-30-16
 */
double CfgMultiWashingDataE2Clz::weightValues(QList<double> &values)
{
    quint32 size = values.size();
    double result;
    if (size == 0)
    {
        result = 0;
    }
    else if (size == 1)
    {
        result = values.value(0);
    }
    else if (size == 2)
    {
        result = (values.value(0) + values.value(1))/2;
    }
    else
    {
        qSort(values);
        double cal = 0.0;
        for (double& inner : values)
        {
            cal += inner;
        }
        cal -= values.first();
        cal -= values.last();
        result = cal/(size-2);
    }
    return result;
}

//-------------------------------------------------
CfgWashingDataInf::CfgWashingDataInf(const CfgWashingTypeEnum type):
    m_type(type)
{

}

bool CfgWashingDataInf::dump(const QString filename)
{
    return true;
}

CfgWashingTypeEnum CfgWashingDataInf::type() const
{
    return m_type;
}

// ----------------------------------------------
CfgItemMeasBasedE1WashingOutDataEle::CfgItemMeasBasedE1WashingOutDataEle():
    m_metaEle((quint32)ELEMEASE1CURSOR::ELEMCURSOR_END)
{
    quint32 idx = 0;
    m_metaEle[idx] = CfgMetaElement(TestUnitName::VOL(), 0xFF);
    m_metaEle[++idx] = CfgMetaElement(TestUnitName::THROTTLE(), 0xFF);
    //m_metaEle[++idx] = CfgMetaElement(TestUnitName::DISTANCE(), 0);

    m_metaEle[++idx] = CfgMetaElement(TestUnitName::CURRENT(), 0);
    m_metaEle[++idx] = CfgMetaElement(TestUnitName::THRUST(), 0);
    m_metaEle[++idx] = CfgMetaElement(TestUnitName::TORQUE(), 0);
    m_metaEle[++idx] = CfgMetaElement(TestUnitName::SPEED(), 0);
    m_metaEle[++idx] = CfgMetaElement(TestUnitName::TEMP1(), 0);
    m_metaEle[++idx] = CfgMetaElement(TestUnitName::TEMP2(), 0);
    m_metaEle[++idx] = CfgMetaElement(TestUnitName::EFFI_POWER(), 0);
    m_metaEle[++idx] = CfgMetaElement(TestUnitName::EFFI_ELE(), 0);
    m_metaEle[++idx] = CfgMetaElement(TestUnitName::POWER(), 0);
    m_metaEle[++idx] = CfgMetaElement(TestUnitName::MECHAPWR(), 0);
    m_metaEle[++idx] = CfgMetaElement(TestUnitName::MECHAEFFI(), 0);
}

void CfgItemMeasBasedE1WashingOutDataEle::setData(quint32 idx, double value)
{
    if (idx < (quint32)ELEMEASE1CURSOR::ELEMCURSOR_END)
    {
        m_metaEle[idx].setData(value);
    }
}

double CfgItemMeasBasedE1WashingOutDataEle::getData(quint32 idx) const
{
    return (idx < (quint32)ELEMEASE1CURSOR::ELEMCURSOR_END) ? m_metaEle[idx].getData() : 0;
}

QString CfgItemMeasBasedE1WashingOutDataEle::getName(quint32 idx) const
{
    if (idx < (quint32)ELEMEASE1CURSOR::ELEMCURSOR_END)
    {
        return m_metaEle[idx].getName();
    }
    return QStringLiteral("");
}

quint8 CfgItemMeasBasedE1WashingOutDataEle::getMotorIdx(quint32 idx) const
{
    if (idx < (quint32)ELEMEASE1CURSOR::ELEMCURSOR_END)
    {
        return m_metaEle[idx].getMotorIdx();
    }
    return 0;
}

/*
 * -------------------------------------------------------
 */
CfgItemMeasBasedE2WashingOutDataEle::CfgItemMeasBasedE2WashingOutDataEle():
    m_metaEle((quint32)ELEMEASCURSOR::ELEMCURSOR_END)
{
    quint32 idx = 0;
    m_metaEle[idx] = CfgMetaElement(TestUnitName::VOL(), 0xFF);
    m_metaEle[++idx] = CfgMetaElement(TestUnitName::THROTTLE(), 0xFF);
    m_metaEle[++idx] = CfgMetaElement(TestUnitName::DISTANCE(), 0);

    m_metaEle[++idx] = CfgMetaElement(TestUnitName::CURRENT(), 0);
    m_metaEle[++idx] = CfgMetaElement(TestUnitName::THRUST(), 0);
    m_metaEle[++idx] = CfgMetaElement(TestUnitName::TORQUE(), 0);
    m_metaEle[++idx] = CfgMetaElement(TestUnitName::SPEED(), 0);
    m_metaEle[++idx] = CfgMetaElement(TestUnitName::TEMP1(), 0);
    m_metaEle[++idx] = CfgMetaElement(TestUnitName::TEMP2(), 0);
    m_metaEle[++idx] = CfgMetaElement(TestUnitName::EFFI_POWER(), 0);
    m_metaEle[++idx] = CfgMetaElement(TestUnitName::EFFI_ELE(), 0);
    m_metaEle[++idx] = CfgMetaElement(TestUnitName::POWER(), 0);
    m_metaEle[++idx] = CfgMetaElement(TestUnitName::MECHAPWR(), 0);
    m_metaEle[++idx] = CfgMetaElement(TestUnitName::MECHAEFFI(), 0);

    m_metaEle[++idx] = CfgMetaElement(TestUnitName::CURRENT(), 1);
    m_metaEle[++idx] = CfgMetaElement(TestUnitName::THRUST(), 1);
    m_metaEle[++idx] = CfgMetaElement(TestUnitName::TORQUE(), 1);
    m_metaEle[++idx] = CfgMetaElement(TestUnitName::SPEED(), 1);
    m_metaEle[++idx] = CfgMetaElement(TestUnitName::TEMP1(), 1);
    m_metaEle[++idx] = CfgMetaElement(TestUnitName::TEMP2(), 1);
    m_metaEle[++idx] = CfgMetaElement(TestUnitName::EFFI_POWER(), 1);
    m_metaEle[++idx] = CfgMetaElement(TestUnitName::EFFI_ELE(), 1);
    m_metaEle[++idx] = CfgMetaElement(TestUnitName::POWER(), 1);
    m_metaEle[++idx] = CfgMetaElement(TestUnitName::MECHAPWR(), 1);
    m_metaEle[++idx] = CfgMetaElement(TestUnitName::MECHAEFFI(), 1);
}

void CfgItemMeasBasedE2WashingOutDataEle::setData(quint32 idx, double value)
{
    if (idx < (quint32)ELEMEASCURSOR::ELEMCURSOR_END)
    {
        m_metaEle[idx].setData(value);
    }
}

double CfgItemMeasBasedE2WashingOutDataEle::getData(quint32 idx) const
{
    return (idx < (quint32)ELEMEASCURSOR::ELEMCURSOR_END) ? m_metaEle[idx].getData() : 0;
}

QString CfgItemMeasBasedE2WashingOutDataEle::getName(quint32 idx) const
{
    if (idx < (quint32)ELEMEASCURSOR::ELEMCURSOR_END)
    {
        return m_metaEle[idx].getName();
    }
    return QStringLiteral("");
}

quint8 CfgItemMeasBasedE2WashingOutDataEle::getMotorIdx(quint32 idx) const
{
    if (idx < (quint32)ELEMEASCURSOR::ELEMCURSOR_END)
    {
        return m_metaEle[idx].getMotorIdx();
    }
    return 0;
}

CfgMetaElement::CfgMetaElement(QString lfh_name, quint32 lfh_motorIdx):
    name(lfh_name),
    motorIdx(lfh_motorIdx)
{

}

QString CfgMetaElement::getName() const
{
    return name;
}

quint8 CfgMetaElement::getMotorIdx() const
{
    return motorIdx;
}

double CfgMetaElement::getData() const
{
    return data;
}

void CfgMetaElement::setData(double value)
{
    data = value;
}


