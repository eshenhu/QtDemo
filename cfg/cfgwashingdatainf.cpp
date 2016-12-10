#include "cfgwashingdatainf.h"

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
        dataInLine = ele.thro;

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
    ele.vol = in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::VOL_POS);
    ele.thro = in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::THRO1_POS);

    ele.data[0].current = in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_CUR1_POS);
    ele.data[0].thrust = in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_THU1_POS);
    ele.data[0].torque = in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_TORQUE1_POS);
    ele.data[0].speed = in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_SPEED1_POS);
    ele.data[0].temp1 = in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_MOTOR1TMP1_POS);
    ele.data[0].temp2 = in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_MOTOR1TMP2_POS);
    ele.data[0].effi_power = in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_M1EFFICI1_POS);
    ele.data[0].effi_ele = in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_M1EFFICI2_POS);
    ele.data[0].power = in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_M1POWER);

    ele.data[1].current = in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_CUR2_POS);
    ele.data[1].thrust = in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_THU2_POS);
    ele.data[1].torque = in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_TORQUE2_POS);
    ele.data[1].speed = in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_SPEED2_POS);
    ele.data[1].temp1 = in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_MOTOR2TMP1_POS);
    ele.data[1].temp2 = in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_MOTOR2TMP2_POS);
    ele.data[1].effi_power = in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_M2EFFICI1_POS);
    ele.data[1].effi_ele = in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_M2EFFICI2_POS);
    ele.data[1].power = in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_M2POWER);

    return ele;
}

void CfgThrottleWashingDataE2Clz::accumulate(const CfgMeasBasedThrottleE2DataEle &data, CfgMeasBasedThrottleE2DataEle& accu)
{
    accu.vol = (accu.vol + data.vol)/2;
    accu.thro = (accu.thro + data.thro)/2;
    for (int idx = 0; idx < 2; idx++){
        accu.data[idx].current = (accu.data[idx].current + data.data[idx].current)/2;
        accu.data[idx].thrust = (accu.data[idx].thrust + data.data[idx].thrust)/2;
        accu.data[idx].torque = (accu.data[idx].torque + data.data[idx].torque)/2;
        accu.data[idx].speed = (accu.data[idx].speed + data.data[idx].speed)/2;
        accu.data[idx].temp1 = (accu.data[idx].temp1 + data.data[idx].temp1)/2;
        accu.data[idx].temp2 = (accu.data[idx].temp2 + data.data[idx].temp2)/2;
        accu.data[idx].effi_power = (accu.data[idx].effi_power + data.data[idx].effi_power)/2;
        accu.data[idx].effi_ele = (accu.data[idx].effi_ele + data.data[idx].effi_ele)/2;
        accu.data[idx].power = (accu.data[idx].power + data.data[idx].power)/2;
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
