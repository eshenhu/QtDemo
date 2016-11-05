#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QDebug>
#include "cfg/cfgjsonprimaryelement.h"
#include "comm/qmodbusdataunit.h"


/*
 *     struct ElecMotorCompStruct
    {
        quint8   limitStatus;
        quint16  voltage;
        ElecMotorStruct elecMotorStruct[2];  // currently, we have 2 motors with maximum size.
    };

    struct OilMotorCompStruct
    {
        quint32 lift;
        quint32 torque;
        quint32 speed;
        quint16 temp_1;
        quint16 temp_2;
    };

    union MotorCompStruct
    {
        ElecMotorCompStruct elec;
        OilMotorCompStruct oil;
    };

    struct MeasStartRecStruct{
        quint16 humidity;
        quint32 envtemp;
        quint32 pressure;
        //quint16 altitude;
        quint8  thro_1;
        quint8  thro_2;
        quint8  backup_1;
        quint8  backup_2;
        quint8  motorType;
        quint8  numOfMotor;
        MotorCompStruct  motorInfo;
    };
 */
namespace Functions {

using functionT = std::function<qint32(const QModbus2DataUnit*, const JsonPVConfig&, const indexOnMotor)>;
using formulaT = std::function<double(qint32)>;


const static functionT functionDummy = [](const QModbus2DataUnit* data, const JsonPVConfig& config, const indexOnMotor idx){
    Q_UNUSED(data)
    Q_UNUSED(config)
    Q_UNUSED(idx)
    return 0;
};
const static formulaT formulaDummy = [](const qint32 v){
    Q_UNUSED(v)
    return 0;
};

const static functionT functionVol = [](const QModbus2DataUnit* data, const JsonPVConfig& config, const indexOnMotor idx){
    Q_UNUSED(idx)
    if (config.motorType() == QModbus2DataUnit::MotorTypeEnum::ELECE)
    {
        qDebug() << "ui.function.functionVol update Vol value with :" << (qint32)data->uvalues().r.s.motorInfo.elec.voltage;
        return (qint32)data->uvalues().r.s.motorInfo.elec.voltage;
    }
    else
    {
        qCritical() << "com.ui.functions Voltage can not enabled on non-elec motor type";
    }
    return (qint32)0;
};

const static formulaT formulaVol = [](const qint32 v){
    return (double)v;
};

const static functionT functionCurrent = [](const QModbus2DataUnit* data, const JsonPVConfig& config, const indexOnMotor idx){
    qint32 rtn = 0;
    if (config.motorType() == QModbus2DataUnit::MotorTypeEnum::ELECE)
    {
        // idx 0 ->1st one 1-> 2nd one
        if ((idx.idxMotor()+1) <= config.numOfMotor())
        {
            qDebug() << "ui.function.functionVol update Current value with :"
                     << static_cast<qint32>(data->uvalues().r.s.motorInfo.elec.elecMotorStruct[idx.idxMotor()].current);
            rtn = static_cast<qint32>(data->uvalues().r.s.motorInfo.elec.elecMotorStruct[idx.idxMotor()].current);
        }
        else
        {
            qCritical() << "com.ui.functions Current idx was not legal(0 or 1) one value == " << idx.idxMotor();
        }
    }
    else
    {
        qCritical() << "com.ui.functions Voltage can not enabled on non-elec motor type";
    }
    return rtn;
};

const static formulaT formulaCurrent = [](const qint32 v){
    return (double)v;
};


const static functionT functionForce = [](const QModbus2DataUnit* data, const JsonPVConfig& config, const indexOnMotor idx){
    qint32 rtn = 0;
    if (config.motorType() == QModbus2DataUnit::MotorTypeEnum::ELECE)
    {
        // idx 0 ->1st one 1-> 2nd one
        if ((idx.idxMotor()+1) <= config.numOfMotor())
        {     
            rtn = static_cast<qint32>(data->uvalues().r.s.motorInfo.elec.elecMotorStruct[idx.idxMotor()].lift);
            qDebug() << "ui.function.functionForce update Force value with :" << rtn;
        }
        else
        {
            qCritical() << "com.ui.functions Current idx was not legal(0 or 1) one value == " << idx.idxMotor();
        }
    }
    else
    {
        rtn = static_cast<qint32>(data->uvalues().r.s.motorInfo.oil.lift);
    }
    return rtn;
};

const static formulaT formulaForce = [](const qint32 v){
    return (double)v;
};

const static functionT functionThrottle = [](const QModbus2DataUnit* data, const JsonPVConfig& config, const indexOnMotor idx){
    Q_UNUSED(config)
    return idx.idxMotor() == 0 ? static_cast<qint32>(data->uvalues().r.s.thro_1) : static_cast<qint32>(data->uvalues().r.s.thro_2);
};

const static formulaT formulaThrottle = [](const qint32 v){
    return (double)v;
};

const static functionT functionSpeed = [](const QModbus2DataUnit* data, const JsonPVConfig& config, const indexOnMotor idx){
    qint32 rtn = 0;
    if (config.motorType() == QModbus2DataUnit::MotorTypeEnum::ELECE)
    {
        // idx 0 ->1st one 1-> 2nd one
        if ((idx.idxMotor()+1) <= config.numOfMotor())
        {
            rtn = static_cast<qint32>(data->uvalues().r.s.motorInfo.elec.elecMotorStruct[idx.idxMotor()].speed);
            qDebug() << "ui.function.functionForce update Speed value with :" << rtn;
        }
        else
        {
            qCritical() << "com.ui.functions Current idx was not legal(0 or 1) one value == " << idx.idxMotor();
        }
    }
    else
    {
        rtn = static_cast<qint32>(data->uvalues().r.s.motorInfo.oil.speed);
    }
    return rtn;
};

const static formulaT formulaSpeed = [](const qint32 v){
    return (double)v;
};

const static functionT functionTemp = [](const QModbus2DataUnit* data, const JsonPVConfig& config, const indexOnMotor idx){
    qint32 rtn = 0;
    if (config.motorType() == QModbus2DataUnit::MotorTypeEnum::ELECE)
    {
        // idx 0 ->1st one 1-> 2nd one
        if ((idx.idxMotor()+1) <= config.numOfMotor())
        {
            rtn = idx.idxOneMotor() == 0 ?
                        static_cast<qint32>(data->uvalues().r.s.motorInfo.elec.elecMotorStruct[idx.idxMotor()].temp_1) :
                        static_cast<qint32>(data->uvalues().r.s.motorInfo.elec.elecMotorStruct[idx.idxMotor()].temp_2);
        }
        else
        {
            qCritical() << "com.ui.functions Current idx was not legal(0 or 1) one value == " << idx.idxMotor();
        }
    }
    else
    {
        rtn = idx.idxOneMotor() == 0 ? static_cast<qint32>(data->uvalues().r.s.motorInfo.oil.temp_1) :
                                       static_cast<qint32>(data->uvalues().r.s.motorInfo.oil.temp_2);
    }
    return rtn;
};

const static formulaT formulaTemp = [](const qint32 v){
    return (double)v;
};

const static functionT functionPowerEffect = [](const QModbus2DataUnit* data, const JsonPVConfig& config, const indexOnMotor idx){
    qint32 rtn = 0;
    if (config.motorType() == QModbus2DataUnit::MotorTypeEnum::ELECE)
    {
        // idx 0 ->1st one 1-> 2nd one
        if ((idx.idxMotor()+1) <= config.numOfMotor())
        {
            rtn =  static_cast<qint32>(data->uvalues().r.s.motorInfo.elec.elecMotorStruct[idx.idxMotor()].torque);
        }
        else
        {
            qCritical() << "com.ui.functions Current idx was not legal(0 or 1) one value == " << idx.idxMotor();
        }
    }
    else
    {
        rtn = static_cast<qint32>(data->uvalues().r.s.motorInfo.oil.torque);
    }
    return rtn;
};

const static formulaT formulaPowerEffect = [](const qint32 v){
    return (double)v;
};

const static functionT functionVibrate = [](const QModbus2DataUnit* data, const JsonPVConfig& config, const indexOnMotor idx){
    qint32 rtn = 0;
    return rtn;
};

const static formulaT formulaVibrate = [](const qint32 v){
    return (double)v;
};

const static functionT functionPower = [](const QModbus2DataUnit* data, const JsonPVConfig& config, const indexOnMotor idx){
    qint32 rtn = 0;
    if (config.motorType() == QModbus2DataUnit::MotorTypeEnum::ELECE)
    {
        // idx 0 ->1st one 1-> 2nd one
        if ((idx.idxMotor()+1) <= config.numOfMotor())
        {
            rtn =  static_cast<qint32>(data->uvalues().r.s.motorInfo.elec.elecMotorStruct[idx.idxMotor()].torque);
        }
        else
        {
            qCritical() << "com.ui.functions Current idx was not legal(0 or 1) one value == " << idx.idxMotor();
        }
    }
    else
    {
        rtn = static_cast<qint32>(data->uvalues().r.s.motorInfo.oil.torque);
    }
    return rtn;
};

const static formulaT formulaPower = [](const qint32 v){
    return (double)v;
};
}

#endif // FUNCTIONS_H
