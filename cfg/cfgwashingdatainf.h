#ifndef CFGWASHINGDATAINF_H
#define CFGWASHINGDATAINF_H

#include <QVector>
#include "cfg/datajsonrecelement.h"

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

    CfgWashingTypeEnum type() const;

private:
    CfgWashingTypeEnum m_type;
};

//Q_DECLARE_INTERFACE(cfgWashingDataInf, "cfgWashingDataInf/1.0")


//---------------------------------------------
/*
 * The result shoudle be startwith the Vol on the X axis. Others on the Y asix.
*/
struct CfgMeasBasedVolE2DataEle
{
    double vol;
    double thro;
};

class CfgVolWashingDataE2Clz : public CfgWashingDataInf
{
public:
    CfgVolWashingDataE2Clz();
    ~CfgVolWashingDataE2Clz() {}
    // cfgWashingDataInf interface
public:
    void wash(const QVector<DataJsonRecElementE2>&) override;
};


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

class CfgMeasBasedThrottleE2DataEle
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

        REC_CUR2_POS,
        REC_THU2_POS,
        REC_TORQUE2_POS,
        REC_SPEED2_POS,
        REC_MOTOR2TMP1_POS,
        REC_MOTOR2TMP2_POS,
        REC_M2EFFICI1_POS,
        REC_M2EFFICI2_POS,
        REC_M2POWER,

        ELEMCURSOR_END
    };

public:
    explicit CfgMeasBasedThrottleE2DataEle();
    ~CfgMeasBasedThrottleE2DataEle(){}

    void setData(quint32 idx, double value);
    double getData(quint32 idx) const;

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
    QVector<CfgMeasBasedThrottleE2DataEle>& data();

private:
    CfgMeasBasedThrottleE2DataEle deserialize(const DataJsonRecElementE2& in);
    void accumulate(const CfgMeasBasedThrottleE2DataEle& data, CfgMeasBasedThrottleE2DataEle&);
private:
    QVector<CfgMeasBasedThrottleE2DataEle> m_data;
};

#endif // CFGWASHINGDATAINF_H
