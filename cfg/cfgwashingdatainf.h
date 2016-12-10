#ifndef CFGWASHINGDATAINF_H
#define CFGWASHINGDATAINF_H

#include <QVector>
#include "cfg/datajsonrecelement.h"


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

struct CfgMeasBasedThrottlePerMotorE2DataEle
{
    double current;
    double thrust;
    double torque;
    quint32 speed;
    quint32 temp1;
    quint32 temp2;
    double  effi_power;
    double  effi_ele;
    quint32 power;
};

struct CfgMeasBasedThrottleE2DataEle
{
    double thro;
    double vol;
    CfgMeasBasedThrottlePerMotorE2DataEle data[2];
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
