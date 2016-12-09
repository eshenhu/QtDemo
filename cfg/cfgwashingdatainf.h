#ifndef CFGWASHINGDATAINF_H
#define CFGWASHINGDATAINF_H

#include <QVector>
#include "cfg/datajsonrecelement.h"

class cfgWashingDataInf
{
public:
    virtual ~cfgWashingDataInf() {}
    virtual void wash(QVector<DataJsonRecElementE2>&) = 0;
};

Q_DECLARE_INTERFACE(cfgWashingDataInf, "cfgWashingDataInf/1.0")


//---------------------------------------------
/*
 * The result shoudle be startwith the Vol on the X axis. Others on the Y asix.
*/
struct cfgMeasBasedVolE2DataEle
{
    double vol;
    double thro;
};

class cfgVolWashingDataE2Clz : public cfgWashingDataInf
{
public:
    cfgVolWashingDataE2Clz();
    ~cfgVolWashingDataE2Clz() {}
    // cfgWashingDataInf interface
public:
    void wash(QVector<DataJsonRecElementE2>&) override;
};


//---------------------------------------------
/*
 * The result shoudle be startwith the Vol on the X axis. Others on the Y asix.
*/

struct cfgMeasBasedThrottlePerMotorE2DataEle
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

struct cfgMeasBasedThrottleE2DataEle
{
    double thro;
    double vol;
    cfgMeasBasedThrottlePerMotorE2DataEle data[2];
};

class cfgThrottleWashingDataE2Clz : public cfgWashingDataInf
{
public:
    cfgThrottleWashingDataE2Clz();
    ~cfgThrottleWashingDataE2Clz() {}
    // cfgWashingDataInf interface
public:
    void wash(QVector<DataJsonRecElementE2>&) override;
    QVector<cfgMeasBasedThrottleE2DataEle>& data() const;

private:
    cfgMeasBasedThrottleE2DataEle deserialize(const DataJsonRecElementE2& in);
    void accumulate(const cfgMeasBasedThrottleE2DataEle& data, cfgMeasBasedThrottleE2DataEle&);
private:
    QVector<cfgMeasBasedThrottleE2DataEle> m_data;
};

#endif // CFGWASHINGDATAINF_H
