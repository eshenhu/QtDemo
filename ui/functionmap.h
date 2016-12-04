#ifndef FUNCTIONMAP_H
#define FUNCTIONMAP_H


#include "ui/functions.h"

struct FunctionPair
{
    Functions::functionT functionF;
    Functions::formulaT formulaF;
};

static std::map<JsonGUIPrimType, FunctionPair> m_funMap =
{
    { JsonGUIPrimType::INVALID,    { Functions::functionDummy,   Functions::formulaDummy    }},
    { JsonGUIPrimType::VOLTAGE,    { Functions::functionVol,     Functions::formulaVol      }},
    { JsonGUIPrimType::CURRENT,    { Functions::functionCurrent, Functions::formulaCurrent  }},
    { JsonGUIPrimType::THRUST,     { Functions::functionThrust,  Functions::formulaThrust  }},
    { JsonGUIPrimType::THROTTLE,   { Functions::functionThrottle,Functions::formulaThrottle  }},
    { JsonGUIPrimType::TORQUE,     { Functions::functionTorque,  Functions::formulaTorque  }},
    { JsonGUIPrimType::SPEED,      { Functions::functionSpeed,   Functions::formulaSpeed  }},
    { JsonGUIPrimType::TEMP,       { Functions::functionTemp,    Functions::formulaTemp  }},
    { JsonGUIPrimType::EFFICIENCY, { Functions::functionPowerEffect, Functions::formulaPowerEffect  }},
//    { JsonGUIPrimType::VIBRATE,    { Functions::functionVibrate, Functions::formulaVibrate  }},
    { JsonGUIPrimType::POWER,      { Functions::functionPower,   Functions::formulaPower  }},
    { JsonGUIPrimType::MECHAPOWER,      { Functions::functionMechaPower,   Functions::formulaMechaPower  }},
    { JsonGUIPrimType::MECHAEFFI,      { Functions::functionMechaEffi,   Functions::formulaMechaEffi }}
};

#endif // FUNCTIONMAP_H
