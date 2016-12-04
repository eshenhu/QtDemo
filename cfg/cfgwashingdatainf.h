#ifndef CFGWASHINGDATAINF_H
#define CFGWASHINGDATAINF_H

#include <QVector>
#include "cfg/datajsonrecelement.h"

class cfgWashingDataInf
{
public:
    virtual ~CfgResHandlerInf() {}
    virtual void wash(QVector<DataJsonRecElementE2>&) = 0;
};

Q_DECLARE_INTERFACE(cfgWashingDataInf, "cfgWashingDataInf/1.0")

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
    cfgVolWashingDataClz();



    // cfgWashingDataInf interface
public:
    void wash(QVector<DataJsonRecElementE2>&);
};

#endif // CFGWASHINGDATAINF_H
