#include "cfgwashingdatainf.h"
#include "ui/uiheader.h"
CfgVolWashingDataE2Clz::CfgVolWashingDataE2Clz():
    CfgWashingDataInf(CfgWashingTypeEnum::CFGWASHINGVOL_E2)
{

}

void CfgVolWashingDataE2Clz::wash(const QVector<DataJsonRecElementE2> &)
{

}


/*
 *  Wash the raw data to the formateed data needed by the Throttle test
 */
CfgThrottleWashingDataE2Clz::CfgThrottleWashingDataE2Clz():
    CfgWashingDataInf(CfgWashingTypeEnum::CFGWASHINGTHROTTLE_E2),
    m_data(0)
{
}

void CfgThrottleWashingDataE2Clz::wash(const QVector<DataJsonRecElementE2> & rawdata)
{
    quint32 cursor = 0;

    quint32 dataInLine = 0;
    CfgMeasBasedThrottleE2DataEle accuData;

    for (const DataJsonRecElementE2& data : rawdata)
    {
        CfgMeasBasedThrottleE2DataEle ele = deserialize(data);
        dataInLine = (quint32)ele.getData((quint32)CfgMeasBasedThrottleE2DataEle::ELEMEASCURSOR::REC_THRO_POS);

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

QVector<CfgMeasBasedThrottleE2DataEle> &CfgThrottleWashingDataE2Clz::data()
{
    return m_data;
}

CfgMeasBasedThrottleE2DataEle CfgThrottleWashingDataE2Clz::deserialize(const DataJsonRecElementE2 &in)
{
    CfgMeasBasedThrottleE2DataEle ele;
    ele.setData( (quint8)CfgMeasBasedThrottleE2DataEle::ELEMEASCURSOR::REC_VOL_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::VOL_POS));
    ele.setData( (quint8)CfgMeasBasedThrottleE2DataEle::ELEMEASCURSOR::REC_THRO_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::THRO1_POS));

    ele.setData( (quint8)CfgMeasBasedThrottleE2DataEle::ELEMEASCURSOR::REC_CUR1_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_CUR1_POS));
    ele.setData( (quint8)CfgMeasBasedThrottleE2DataEle::ELEMEASCURSOR::REC_THU1_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_THU1_POS));
    ele.setData( (quint8)CfgMeasBasedThrottleE2DataEle::ELEMEASCURSOR::REC_TORQUE1_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_TORQUE1_POS));
    ele.setData( (quint8)CfgMeasBasedThrottleE2DataEle::ELEMEASCURSOR::REC_SPEED1_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_SPEED1_POS));
    ele.setData( (quint8)CfgMeasBasedThrottleE2DataEle::ELEMEASCURSOR::REC_MOTOR1TMP1_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_MOTOR1TMP1_POS));
    ele.setData( (quint8)CfgMeasBasedThrottleE2DataEle::ELEMEASCURSOR::REC_MOTOR1TMP2_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_MOTOR1TMP2_POS));
    ele.setData( (quint8)CfgMeasBasedThrottleE2DataEle::ELEMEASCURSOR::REC_M1EFFICI1_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_M1EFFICI1_POS));
    ele.setData( (quint8)CfgMeasBasedThrottleE2DataEle::ELEMEASCURSOR::REC_M1EFFICI2_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_M1EFFICI2_POS));
    ele.setData( (quint8)CfgMeasBasedThrottleE2DataEle::ELEMEASCURSOR::REC_M1POWER, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_M1POWER));

    ele.setData( (quint8)CfgMeasBasedThrottleE2DataEle::ELEMEASCURSOR::REC_CUR2_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_CUR2_POS));
    ele.setData( (quint8)CfgMeasBasedThrottleE2DataEle::ELEMEASCURSOR::REC_THU2_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_THU2_POS));
    ele.setData( (quint8)CfgMeasBasedThrottleE2DataEle::ELEMEASCURSOR::REC_TORQUE2_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_TORQUE2_POS));
    ele.setData( (quint8)CfgMeasBasedThrottleE2DataEle::ELEMEASCURSOR::REC_SPEED2_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_SPEED2_POS));
    ele.setData( (quint8)CfgMeasBasedThrottleE2DataEle::ELEMEASCURSOR::REC_MOTOR2TMP1_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_MOTOR2TMP1_POS));
    ele.setData( (quint8)CfgMeasBasedThrottleE2DataEle::ELEMEASCURSOR::REC_MOTOR2TMP2_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_MOTOR2TMP2_POS));
    ele.setData( (quint8)CfgMeasBasedThrottleE2DataEle::ELEMEASCURSOR::REC_M2EFFICI1_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_M2EFFICI1_POS));
    ele.setData( (quint8)CfgMeasBasedThrottleE2DataEle::ELEMEASCURSOR::REC_M2EFFICI2_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_M2EFFICI2_POS));
    ele.setData( (quint8)CfgMeasBasedThrottleE2DataEle::ELEMEASCURSOR::REC_M2POWER, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_M2POWER));

    return ele;
}

void CfgThrottleWashingDataE2Clz::accumulate(const CfgMeasBasedThrottleE2DataEle &data, CfgMeasBasedThrottleE2DataEle& accu)
{
    for (int i = 0; i < accu.m_metaEle.size(); i++)
    {
        accu.setData(i,(accu.getData(i) + data.getData(i))/2);
    }
}

CfgWashingDataInf::CfgWashingDataInf(const CfgWashingTypeEnum type):
    m_type(type)
{

}

CfgWashingTypeEnum CfgWashingDataInf::type() const
{
    return m_type;
}

CfgMeasBasedThrottleE2DataEle::CfgMeasBasedThrottleE2DataEle():
    m_metaEle((quint32)ELEMEASCURSOR::ELEMCURSOR_END)
{
    m_metaEle[0] = CfgMetaElement(TestUnitName::VOL(), 0xFF);
    m_metaEle[1] = CfgMetaElement(TestUnitName::THROTTLE(), 0xFF);

    m_metaEle[2] = CfgMetaElement(TestUnitName::CURRENT(), 0);
    m_metaEle[3] = CfgMetaElement(TestUnitName::THRUST(), 0);
    m_metaEle[4] = CfgMetaElement(TestUnitName::TORQUE(), 0);
    m_metaEle[5] = CfgMetaElement(TestUnitName::SPEED(), 0);
    m_metaEle[6] = CfgMetaElement(TestUnitName::TEMP1(), 0);
    m_metaEle[7] = CfgMetaElement(TestUnitName::TEMP2(), 0);
    m_metaEle[8] = CfgMetaElement(TestUnitName::EFFI_POWER(), 0);
    m_metaEle[9] = CfgMetaElement(TestUnitName::EFFI_ELE(), 0);
    m_metaEle[10] = CfgMetaElement(TestUnitName::POWER(), 0);

    m_metaEle[11] = CfgMetaElement(TestUnitName::CURRENT(), 1);
    m_metaEle[12] = CfgMetaElement(TestUnitName::THRUST(), 1);
    m_metaEle[13] = CfgMetaElement(TestUnitName::TORQUE(), 1);
    m_metaEle[14] = CfgMetaElement(TestUnitName::SPEED(), 1);
    m_metaEle[15] = CfgMetaElement(TestUnitName::TEMP1(), 1);
    m_metaEle[16] = CfgMetaElement(TestUnitName::TEMP2(), 1);
    m_metaEle[17] = CfgMetaElement(TestUnitName::EFFI_POWER(), 1);
    m_metaEle[18] = CfgMetaElement(TestUnitName::EFFI_ELE(), 1);
    m_metaEle[19] = CfgMetaElement(TestUnitName::POWER(), 1);
}

void CfgMeasBasedThrottleE2DataEle::setData(quint32 idx, double value)
{
    if (idx < (quint32)ELEMEASCURSOR::ELEMCURSOR_END)
    {
        m_metaEle[idx].setData(value);
    }
}

double CfgMeasBasedThrottleE2DataEle::getData(quint32 idx) const
{
    return (idx < (quint32)ELEMEASCURSOR::ELEMCURSOR_END) ? m_metaEle[idx].getData() : 0;
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
