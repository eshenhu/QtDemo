#include "cfgwashingdatainf.h"
#include "ui/uiheader.h"
#include <QMap>
#include "util/polyfit.h"
#include <memory>

CfgVolWashingDataE2Clz::CfgVolWashingDataE2Clz():
    CfgWashingDataInf(CfgWashingTypeEnum::CFGWASHINGVOL_E2),
    m_data(0)
{

}

void CfgVolWashingDataE2Clz::wash(const QVector<DataJsonRecElementE2> &rawdata)
{
    quint32 cursor = 0;

    quint32 dataInLine = 0;
    CfgItemMeasBasedE2DataEle accuData;

    for (const DataJsonRecElementE2& data : rawdata)
    {
        CfgItemMeasBasedE2DataEle ele = deserialize(data);
        dataInLine = (quint32)ele.getData((quint32)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_VOL_POS);

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

QVector<CfgItemMeasBasedE2DataEle> &CfgVolWashingDataE2Clz::data()
{
    return m_data;
}

void CfgVolWashingDataE2Clz::generateData(quint32 idx, QVector<QCPGraphData> &pairs, QString &name, quint8 &motorIdx)
{
    name = m_data[0].getName((quint32)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_VOL_POS) + ':' +  m_data[0].getName(idx),
    motorIdx = m_data[0].getMotorIdx(idx);

    pairs.resize(m_data.size());
    for (int i = 0; i < m_data.size(); i++)
    {
        pairs[i].key   = (quint32)m_data[i].getData((quint32)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_VOL_POS);
        pairs[i].value = m_data[i].getData(idx);
    }
}

CfgItemMeasBasedE2DataEle CfgVolWashingDataE2Clz::deserialize(const DataJsonRecElementE2 &in)
{
    CfgItemMeasBasedE2DataEle ele;
    ele.setData( (quint8)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_VOL_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::VOL_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_THRO_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::THRO1_POS));

    ele.setData( (quint8)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_CUR1_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_CUR1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_THU1_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_THU1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_TORQUE1_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_TORQUE1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_SPEED1_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_SPEED1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_MOTOR1TMP1_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_MOTOR1TMP1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_MOTOR1TMP2_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_MOTOR1TMP2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_M1EFFICI1_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_M1EFFICI1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_M1EFFICI2_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_M1EFFICI2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_M1POWER, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_M1POWER));

    ele.setData( (quint8)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_CUR2_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_CUR2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_THU2_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_THU2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_TORQUE2_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_TORQUE2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_SPEED2_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_SPEED2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_MOTOR2TMP1_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_MOTOR2TMP1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_MOTOR2TMP2_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_MOTOR2TMP2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_M2EFFICI1_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_M2EFFICI1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_M2EFFICI2_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_M2EFFICI2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_M2POWER, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_M2POWER));

    return ele;
}

void CfgVolWashingDataE2Clz::accumulate(const CfgItemMeasBasedE2DataEle &data, CfgItemMeasBasedE2DataEle &accu)
{
    for (int i = 0; i < accu.m_metaEle.size(); i++)
    {
        accu.setData(i,(accu.getData(i) + data.getData(i))/2);
    }
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
    CfgItemMeasBasedE2DataEle accuData;

    for (const DataJsonRecElementE2& data : rawdata)
    {
        CfgItemMeasBasedE2DataEle ele = deserialize(data);
        dataInLine = (quint32)ele.getData((quint32)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_THRO_POS);

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

QVector<CfgItemMeasBasedE2DataEle> &CfgThrottleWashingDataE2Clz::data()
{
    return m_data;
}

void CfgThrottleWashingDataE2Clz::generateData(quint32 idx, QVector<QCPGraphData> &pairs, QString &name, quint8 &motorIdx)
{   
    name = m_data[0].getName((quint32)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_THRO_POS) + ':' + m_data[0].getName(idx);
    motorIdx = m_data[0].getMotorIdx(idx);

    pairs.resize(m_data.size());
    for (int i = 0; i < m_data.size(); i++)
    {
        pairs[i].key   = (quint32)m_data[i].getData((quint32)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_THRO_POS);
        pairs[i].value = m_data[i].getData(idx);
    }
}

CfgItemMeasBasedE2DataEle CfgThrottleWashingDataE2Clz::deserialize(const DataJsonRecElementE2 &in)
{
    CfgItemMeasBasedE2DataEle ele;
    ele.setData( (quint8)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_VOL_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::VOL_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_THRO_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::THRO1_POS));

    ele.setData( (quint8)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_CUR1_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_CUR1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_THU1_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_THU1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_TORQUE1_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_TORQUE1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_SPEED1_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_SPEED1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_MOTOR1TMP1_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_MOTOR1TMP1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_MOTOR1TMP2_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_MOTOR1TMP2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_M1EFFICI1_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_M1EFFICI1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_M1EFFICI2_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_M1EFFICI2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_M1POWER, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_M1POWER));
    ele.setData( (quint8)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_M1MECHAPWR, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_M1MECHAPWR));
    ele.setData( (quint8)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_M1MECHAEFFI, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_M1MECHAEFFI));

    ele.setData( (quint8)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_CUR2_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_CUR2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_THU2_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_THU2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_TORQUE2_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_TORQUE2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_SPEED2_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_SPEED2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_MOTOR2TMP1_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_MOTOR2TMP1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_MOTOR2TMP2_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_MOTOR2TMP2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_M2EFFICI1_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_M2EFFICI1_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_M2EFFICI2_POS, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_M2EFFICI2_POS));
    ele.setData( (quint8)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_M2POWER, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_M2POWER));
    ele.setData( (quint8)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_M2MECHAPWR, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_M2MECHAPWR));
    ele.setData( (quint8)CfgItemMeasBasedE2DataEle::ELEMEASCURSOR::REC_M2MECHAEFFI, in.getData((quint32)DataJsonRecElementE2::ELEMCURSOR::REC_M2MECHAEFFI));
    return ele;
}

void CfgThrottleWashingDataE2Clz::accumulate(const CfgItemMeasBasedE2DataEle &data, CfgItemMeasBasedE2DataEle& accu)
{
    for (int i = 0; i < accu.m_metaEle.size(); i++)
    {
        accu.setData(i,(accu.getData(i) + data.getData(i))/2);
    }
}


//-------------------------------------------------

CfgMultiWashingDataE2Clz::CfgMultiWashingDataE2Clz():
    CfgWashingDataInf(CfgWashingTypeEnum::CFGWASHINGMULTI_E2)
{
    AsixDataItem axisItemX;
    AsixDataItem axisItemY;
    CfgMultiWashingDataItem item;

    item.motorIdx = 0;
    axisItemX.idx = (quint32)DataJsonRecElementE2::ELEMCURSOR::REC_CUR1_POS;
    axisItemX.str = TestUnitName::CURRENT();
    axisItemY.idx = (quint32)DataJsonRecElementE2::ELEMCURSOR::REC_THU1_POS;
    axisItemY.str = TestUnitName::THRUST();

    item.XKey = axisItemX;
    item.YValue = axisItemY;
    m_data.append(item);

    item.motorIdx = 0;
    axisItemX.idx = (quint32)DataJsonRecElementE2::ELEMCURSOR::REC_CUR1_POS;
    axisItemX.str = TestUnitName::CURRENT();
    axisItemY.idx = (quint32)DataJsonRecElementE2::ELEMCURSOR::REC_TORQUE1_POS;
    axisItemY.str = TestUnitName::TORQUE();

    item.XKey = axisItemX;
    item.YValue = axisItemY;
    m_data.append(item);

    item.motorIdx = 0;
    axisItemX.idx = (quint32)DataJsonRecElementE2::ELEMCURSOR::REC_CUR1_POS;
    axisItemX.str = TestUnitName::CURRENT();
    axisItemY.idx = (quint32)DataJsonRecElementE2::ELEMCURSOR::REC_SPEED1_POS;
    axisItemY.str = TestUnitName::SPEED();

    item.XKey = axisItemX;
    item.YValue = axisItemY;
    m_data.append(item);

    item.motorIdx = 0;
    axisItemX.idx = (quint32)DataJsonRecElementE2::ELEMCURSOR::REC_CUR1_POS;
    axisItemX.str = TestUnitName::CURRENT();
    axisItemY.idx = (quint32)DataJsonRecElementE2::ELEMCURSOR::REC_M1EFFICI1_POS;
    axisItemY.str = TestUnitName::EFFI_POWER();

    item.XKey = axisItemX;
    item.YValue = axisItemY;
    m_data.append(item);

    item.motorIdx = 0;
    axisItemX.idx = (quint32)DataJsonRecElementE2::ELEMCURSOR::REC_CUR1_POS;
    axisItemX.str = TestUnitName::CURRENT();
    axisItemY.idx = (quint32)DataJsonRecElementE2::ELEMCURSOR::REC_M1EFFICI2_POS;
    axisItemY.str = TestUnitName::EFFI_ELE();

    item.XKey = axisItemX;
    item.YValue = axisItemY;
    m_data.append(item);

    m_guiList.clear();
    for (const CfgMultiWashingDataItem& item : m_data)
    {
        QStringList list({item.XKey.str, item.YValue.str, QString::number(item.motorIdx)});
        //list << item.XKey.str << item.YValue.str << QString::number(item.motorIdx);
        m_guiList.append(list.join(':'));
    }
}

void CfgMultiWashingDataE2Clz::wash(const QVector<DataJsonRecElementE2> & rawdata)
{
    for (CfgMultiWashingDataItem& item : m_data)
    {
        quint32 xIdx = item.XKey.idx;
        quint32 yIdx = item.YValue.idx;
        QMap<double, double> map;

        for (const DataJsonRecElementE2& data : rawdata)
        {
            map.insert(data.getData(xIdx), data.getData(yIdx));
        }        

        item.data.clear();
        auto iter = map.cbegin();
        while (iter != map.cend())
        {
            qDebug() << "x = " << iter.key() << "y" << iter.value();
            item.data.append( KV(iter.key(), iter.value()) );

            ++iter;
        }
    }
}

QVector<CfgMultiWashingDataItem> &CfgMultiWashingDataE2Clz::data()
{
    return m_data;
}

void CfgMultiWashingDataE2Clz::generateData(quint32 idx, QVector<QCPGraphData> &pairs, QString &name, quint8 &motorIdx)
{
    if (idx < (quint32)m_data.size())
    {
        motorIdx = m_data[idx].motorIdx;
        name = m_data[idx].XKey.str + ":" + m_data[idx].YValue.str;

        const quint32 size = m_data[idx].data.size();

        std::shared_ptr<double> xAsix(new double[size]);
        std::shared_ptr<double> yAsix(new double[size]);

        do {
            const quint32 order = 2;
            double coefficients[order+1]; // y = ax*x + bx + c;

            qint32 xAsixFloor = qFloor(m_data[idx].data.first().key);
            qint32 xAsixCeil = qCeil(m_data[idx].data.last().key);

            quint32 eleIdx = 0;
            for (const KV& ele : m_data[idx].data)
            {
                double* xValue = xAsix.get();
                xValue[eleIdx] = ele.key;
                double* yValue = yAsix.get();
                yValue[eleIdx] = ele.value;

                eleIdx++;
            }

//            for (int x = 0; x < size; x++)
//            {
//                qDebug() << "cfgwashingdatainf x=" << xAsix[x] << "y=" << yAsix[x];
//            }

            PolyFitClz::plotfit(xAsix.get(), yAsix.get(), size, order, coefficients);

            for (qint32 x = xAsixFloor; x < xAsixCeil; x++)
            {
                double y = coefficients[2] * x * x
                        +  coefficients[1] * x
                        +  coefficients[0];

                pairs.append(QCPGraphData((double)x, y));
            }
        }while(0);
    }
    else
    {
        qWarning() << "Warning: out of size of array, request = " << idx << "size of container"
                   << m_data.size();
    }
}

//-------------------------------------------------
CfgWashingDataInf::CfgWashingDataInf(const CfgWashingTypeEnum type):
    m_type(type)
{

}

CfgWashingTypeEnum CfgWashingDataInf::type() const
{
    return m_type;
}

CfgItemMeasBasedE2DataEle::CfgItemMeasBasedE2DataEle():
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
    m_metaEle[11] = CfgMetaElement(TestUnitName::MECHAPWR(), 0);
    m_metaEle[12] = CfgMetaElement(TestUnitName::MECHAEFFI(), 0);

    m_metaEle[13] = CfgMetaElement(TestUnitName::CURRENT(), 1);
    m_metaEle[14] = CfgMetaElement(TestUnitName::THRUST(), 1);
    m_metaEle[15] = CfgMetaElement(TestUnitName::TORQUE(), 1);
    m_metaEle[16] = CfgMetaElement(TestUnitName::SPEED(), 1);
    m_metaEle[17] = CfgMetaElement(TestUnitName::TEMP1(), 1);
    m_metaEle[18] = CfgMetaElement(TestUnitName::TEMP2(), 1);
    m_metaEle[19] = CfgMetaElement(TestUnitName::EFFI_POWER(), 1);
    m_metaEle[20] = CfgMetaElement(TestUnitName::EFFI_ELE(), 1);
    m_metaEle[21] = CfgMetaElement(TestUnitName::POWER(), 1);
    m_metaEle[22] = CfgMetaElement(TestUnitName::MECHAPWR(), 1);
    m_metaEle[23] = CfgMetaElement(TestUnitName::MECHAEFFI(), 1);
}

void CfgItemMeasBasedE2DataEle::setData(quint32 idx, double value)
{
    if (idx < (quint32)ELEMEASCURSOR::ELEMCURSOR_END)
    {
        m_metaEle[idx].setData(value);
    }
}

double CfgItemMeasBasedE2DataEle::getData(quint32 idx) const
{
    return (idx < (quint32)ELEMEASCURSOR::ELEMCURSOR_END) ? m_metaEle[idx].getData() : 0;
}

QString CfgItemMeasBasedE2DataEle::getName(quint32 idx) const
{
    if (idx < (quint32)ELEMEASCURSOR::ELEMCURSOR_END)
    {
        return m_metaEle[idx].getName();
    }
    return QStringLiteral("");
}

quint8 CfgItemMeasBasedE2DataEle::getMotorIdx(quint32 idx) const
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


