#ifndef ABSTRACTSPINBOXATTR_H
#define ABSTRACTSPINBOXATTR_H

#include "basedspinboxattrinf.h"
#include "ui/functions.h"

struct FunctionPair
{
    Functions::functionT functionF;
    Functions::formulaT formulaF;
};

class AbstractSpinBoxAttr : public BasedSpinBoxAttrInf
{
public:
    Q_INTERFACES(BasedSpinBoxAttrInf)

    AbstractSpinBoxAttr() = default;
    AbstractSpinBoxAttr(const JsonPVConfig& config, const JsonGUIElement& ele);
    ~AbstractSpinBoxAttr(){}

    // BasedSpinBoxAttrInf interface
public:
    JsonGUIPrimType type() const{
        return m_elem.type();
    }
    QString str() const{
        return m_elem.str();
    }
    indexOnMotor idx() const{
        return m_elem.idx();
    }
    qint16 lowLimit() const{
        return m_elem.lowLimit();
    }
    qint16 upLimit() const{
        return m_elem.upLimit();
    }
    QString unit() const{
        return m_elem.unit();
    }

    virtual void update(const QModbus2DataUnit *data);

private:
    JsonPVConfig   m_config;
    JsonGUIElement m_elem;
    qint32 m_data;
    double m_pushData;
};

static std::map<JsonGUIPrimType, FunctionPair> m_funMap =
{
    { JsonGUIPrimType::INVALID,    { Functions::functionDummy,   Functions::formulaDummy    }},
    { JsonGUIPrimType::VOLTAGE,    { Functions::functionVol,     Functions::formulaVol      }},
    { JsonGUIPrimType::CURRENT,    { Functions::functionCurrent, Functions::formulaCurrent  }},
    { JsonGUIPrimType::FORCE,      { Functions::functionForce,   Functions::formulaForce  }},
    { JsonGUIPrimType::THROTTLE,   { Functions::functionThrottle,Functions::formulaThrottle  }},
    { JsonGUIPrimType::SPEED,      { Functions::functionSpeed,   Functions::formulaSpeed  }},
    { JsonGUIPrimType::TEMP,       { Functions::functionTemp,    Functions::formulaTemp  }},
    { JsonGUIPrimType::POWEREFFEC, { Functions::functionPowerEffect, Functions::formulaPowerEffect  }},
    { JsonGUIPrimType::VIBRATE,    { Functions::functionVibrate, Functions::formulaVibrate  }},
    { JsonGUIPrimType::POWER,      { Functions::functionPower,   Functions::formulaPower  }}
};
#endif // ABSTRACTSPINBOXATTR_H
