#ifndef BASEDSPINBOXATTRINF_H
#define BASEDSPINBOXATTRINF_H

#include <functional>
#include <QString>
#include <QtPlugin>
#include "cfg/cfgjsonprimaryelement.h"
#include "comm/qmodbusdataunit.h"

class BasedSpinBoxAttrInf
{
public:
    virtual ~BasedSpinBoxAttrInf(){}

    virtual JsonGUIPrimType type() const = 0;
    virtual QString str() const = 0;
    virtual indexOnMotor idx() const = 0;
    virtual qint16 lowLimit() const = 0;
    virtual qint16 upLimit() const = 0;
    virtual QString unit() const = 0;

    virtual void update(const QModbus2DataUnit* data) = 0;
};

Q_DECLARE_INTERFACE(BasedSpinBoxAttrInf, "BasedSpinBoxAttrInf/1.0")

#endif // BASEDSPINBOXATTRINF_H
