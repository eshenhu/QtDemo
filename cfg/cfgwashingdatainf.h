#ifndef CFGWASHINGDATAINF_H
#define CFGWASHINGDATAINF_H

#include <QVector>
#include "cfg/datajsonrecelement.h"
#include "chartsview/qcustomplot.h"

class CfgMetaElement{
public:
    CfgMetaElement() = default;
    CfgMetaElement(QString lfh_name, quint32 lfh_motorIdx);
    ~CfgMetaElement(){}

public:
    QString getName() const;

    quint8 getMotorIdx() const;

    double getData() const;
    void setData(double value);

private:
    QString name;
    quint8 motorIdx;
    double data;
};

enum class CfgWashingTypeEnum {
    CFGWASHINGVOL_E1,
    CFGWASHINGVOL_E2,
    CFGWASHINGTHROTTLE_E1,
    CFGWASHINGTHROTTLE_E2,
    CFGWASHINGMULTI_E1,
    CFGWASHINGMULTI_E2
};

class CfgWashingDataInf
{
public:
    explicit CfgWashingDataInf(const CfgWashingTypeEnum type);
    virtual ~CfgWashingDataInf() {}
    virtual void wash(const QVector<DataJsonRecElementE2>&) = 0;
    virtual void generateData(quint32 idx, QVector<QCPGraphData>& pairs, QString& name, quint8& motorIdx) = 0;
    virtual QList<QString>& getGUIActionList(quint32) = 0;
    CfgWashingTypeEnum type() const;

private:
    CfgWashingTypeEnum m_type;
};

class KV
{
public:
    KV(){}
    KV(double x, double y): key(x), value(y){}

public:
    double key;
    double value;
};

//Q_DECLARE_INTERFACE(cfgWashingDataInf, "cfgWashingDataInf/1.0")


//---------------------------------------------
/*
 * The result shoudle be startwith the Vol on the X axis. Others on the Y asix.
*/

//---------------------------------------------
/*
 * The result shoudle be startwith the Vol on the X axis. Others on the Y asix.
*/

class CfgMeasBasedThrottlePerMotorE2DataEle
{
public:
    ~CfgMeasBasedThrottlePerMotorE2DataEle(){}

public:
    CfgMetaElement current;
    CfgMetaElement thrust;
    CfgMetaElement torque;
    CfgMetaElement speed;
    CfgMetaElement temp1;
    CfgMetaElement temp2;
    CfgMetaElement effi_power;
    CfgMetaElement effi_ele;
    CfgMetaElement power;
};

class CfgItemMeasBasedE2DataEle
{
public:
    enum class ELEMEASCURSOR : quint8
    {
        REC_VOL_POS = 0,
        REC_THRO_POS,

        REC_CUR1_POS,
        REC_THU1_POS,
        REC_TORQUE1_POS,
        REC_SPEED1_POS,
        REC_MOTOR1TMP1_POS,
        REC_MOTOR1TMP2_POS,
        REC_M1EFFICI1_POS,
        REC_M1EFFICI2_POS,
        REC_M1POWER,
        REC_M1MECHAPWR,
        REC_M1MECHAEFFI,

        REC_CUR2_POS,
        REC_THU2_POS,
        REC_TORQUE2_POS,
        REC_SPEED2_POS,
        REC_MOTOR2TMP1_POS,
        REC_MOTOR2TMP2_POS,
        REC_M2EFFICI1_POS,
        REC_M2EFFICI2_POS,
        REC_M2POWER,
        REC_M2MECHAPWR,
        REC_M2MECHAEFFI,

        ELEMCURSOR_END
    };

public:
    explicit CfgItemMeasBasedE2DataEle();
    ~CfgItemMeasBasedE2DataEle(){}

    void setData(quint32 idx, double value);
    double getData(quint32 idx) const;

    QString getName(quint32 idx) const;
    quint8 getMotorIdx(quint32 idx) const;

public:
    QVector<CfgMetaElement> m_metaEle;
};

class CfgThrottleWashingDataE2Clz : public CfgWashingDataInf
{
public:
    CfgThrottleWashingDataE2Clz();
    ~CfgThrottleWashingDataE2Clz() {}
    // cfgWashingDataInf interface
public:
    void wash(const QVector<DataJsonRecElementE2>&) override;
    QVector<CfgItemMeasBasedE2DataEle>& data();
    void generateData(quint32 idx, QVector<QCPGraphData>& pairs, QString& name, quint8& motorIdx) override;
    QList<QString> &getGUIActionList(quint32) override { return m_guiList; }
private:
    CfgItemMeasBasedE2DataEle deserialize(const DataJsonRecElementE2& in);
    void accumulate(const CfgItemMeasBasedE2DataEle& data, CfgItemMeasBasedE2DataEle&);
private:
    QVector<CfgItemMeasBasedE2DataEle> m_data;
    QList<QString> m_guiList;
};


//---------------------------------------------
/*
 * The result shoudle be startwith the Vol on the X axis. Others on the Y asix.
*/

class CfgVolWashingDataE2Clz : public CfgWashingDataInf
{
public:
    CfgVolWashingDataE2Clz();
    ~CfgVolWashingDataE2Clz() {}
    // cfgWashingDataInf interface
public:
    void wash(const QVector<DataJsonRecElementE2>&) override;
    QVector<CfgItemMeasBasedE2DataEle>& data();
    void generateData(quint32 idx, QVector<QCPGraphData>& pairs, QString& name, quint8& motorIdx) override;
    QList<QString> &getGUIActionList(quint32) override { return m_guiList; }
private:
    CfgItemMeasBasedE2DataEle deserialize(const DataJsonRecElementE2& in);
    void accumulate(const CfgItemMeasBasedE2DataEle& data, CfgItemMeasBasedE2DataEle&);
private:
    QVector<CfgItemMeasBasedE2DataEle> m_data;
    QList<QString> m_guiList;
};


//---------------------------------------------
/*
 * The result shoudle be startwith the Vol on the X axis. Others on the Y asix.
*/

struct AsixDataItem
{
    QString str;
    quint32 idx;
};

struct CfgMultiWashingDataItem
{
    quint32 motorIdx;
    AsixDataItem XKey;
    AsixDataItem YValue;
    QVector<KV> data;
};

class CfgMultiWashingDataE2Clz : public CfgWashingDataInf
{
public:
    CfgMultiWashingDataE2Clz();
    ~CfgMultiWashingDataE2Clz() {}
    // cfgWashingDataInf interface
public:
    void wash(const QVector<DataJsonRecElementE2>&) override;
//    QVector<CfgMultiWashingDataItem>& data();
    void generateData(quint32 idx, QVector<QCPGraphData>& pairs, QString& name, quint8& motorIdx) override;
    QList<QString> &getGUIActionList(quint32 idx) override
    {
        if (idx < m_guiList.size())
            return m_guiList[idx];
        else
            return m_guiList[0];
    }
private:
    //CfgItemMeasBasedE2DataEle deserialize(const DataJsonRecElementE2& in);
    //void accumulate(const CfgItemMeasBasedE2DataEle& data, CfgItemMeasBasedE2DataEle&);
private:
    QVector<QVector<CfgMultiWashingDataItem>> m_data;
    QVector<QList<QString>> m_guiList;
};

#endif // CFGWASHINGDATAINF_H
