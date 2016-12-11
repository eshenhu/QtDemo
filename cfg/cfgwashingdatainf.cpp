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
        dataInLine = (quint32)ele.thro.getData();

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
    ele.vol.setData( in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::VOL_POS));
    ele.thro.setData( in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::THRO1_POS));

    ele.data[0].current.setData( in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_CUR1_POS));
    ele.data[0].thrust.setData( in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_THU1_POS));
    ele.data[0].torque.setData( in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_TORQUE1_POS));
    ele.data[0].speed.setData( in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_SPEED1_POS));
    ele.data[0].temp1.setData( in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_MOTOR1TMP1_POS));
    ele.data[0].temp2.setData( in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_MOTOR1TMP2_POS));
    ele.data[0].effi_power.setData( in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_M1EFFICI1_POS));
    ele.data[0].effi_ele.setData( in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_M1EFFICI2_POS));
    ele.data[0].power.setData( in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_M1POWER));

    ele.data[1].current.setData( in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_CUR2_POS));
    ele.data[1].thrust.setData( in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_THU2_POS));
    ele.data[1].torque.setData( in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_TORQUE2_POS));
    ele.data[1].speed.setData( in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_SPEED2_POS));
    ele.data[1].temp1.setData( in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_MOTOR2TMP1_POS));
    ele.data[1].temp2.setData( in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_MOTOR2TMP2_POS));
    ele.data[1].effi_power.setData( in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_M2EFFICI1_POS));
    ele.data[1].effi_ele.setData( in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_M2EFFICI2_POS));
    ele.data[1].power.setData( in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_M2POWER));

    return ele;
}

void CfgThrottleWashingDataE2Clz::accumulate(const CfgMeasBasedThrottleE2DataEle &data, CfgMeasBasedThrottleE2DataEle& accu)
{
    accu.vol.setData((accu.vol.getData() + data.vol.getData())/2);
    accu.thro.setData((accu.thro.getData() + data.thro.getData())/2);
    for (int idx = 0; idx < 2; idx++){
        accu.data[idx].current.setData( (accu.data[idx].current.getData() + data.data[idx].current.getData())/2);
        accu.data[idx].thrust.setData( (accu.data[idx].thrust.getData() + data.data[idx].thrust.getData())/2);
        accu.data[idx].torque.setData( (accu.data[idx].torque.getData() + data.data[idx].torque.getData())/2);
        accu.data[idx].speed.setData( (accu.data[idx].speed.getData() + data.data[idx].speed.getData())/2);
        accu.data[idx].temp1.setData( (accu.data[idx].temp1.getData() + data.data[idx].temp1.getData())/2);
        accu.data[idx].temp2.setData( (accu.data[idx].temp2.getData() + data.data[idx].temp2.getData())/2);
        accu.data[idx].effi_power.setData( (accu.data[idx].effi_power.getData() + data.data[idx].effi_power.getData())/2);
        accu.data[idx].effi_ele.setData( (accu.data[idx].effi_ele.getData() + data.data[idx].effi_ele.getData())/2);
        accu.data[idx].power.setData( (accu.data[idx].power.getData() + data.data[idx].power.getData())/2);
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
    vol(TestUnitName::VOL(), 0xFF),
    thro(TestUnitName::THROTTLE(), 0xFF)
{
    for (quint8 i = 0; i < 2; i++)
    {
        data[i].current = CfgMetaElement(TestUnitName::CURRENT(), i);
        data[i].thrust = CfgMetaElement(TestUnitName::THRUST(), i);
        data[i].torque = CfgMetaElement(TestUnitName::TORQUE(), i);
        data[i].speed = CfgMetaElement(TestUnitName::SPEED(), i);
        data[i].temp1 = CfgMetaElement(TestUnitName::TEMP1(), i);
        data[i].temp2 = CfgMetaElement(TestUnitName::TEMP2(), i);
        data[i].effi_power = CfgMetaElement(TestUnitName::EFFI_POWER(), i);
        data[i].effi_ele = CfgMetaElement(TestUnitName::EFFI_ELE(), i);
        data[i].power = CfgMetaElement(TestUnitName::POWER(), i);
    }
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
