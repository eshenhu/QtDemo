#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QDebug>
#include "cfg/cfgjsonprimaryelement.h"
#include "comm/qmodbusdataunit.h"
#include "actionwidget.h"

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
    static quint32 lastNoneZeroVol;
    quint32 v;
    //if (config.motorType() == QModbus2DataUnit::MotorTypeEnum::ELECE)
    if (data->uvalues().r.s.motorType == (quint8)QModbus2DataUnit::MotorTypeEnum::ELECE)
    {
//        qDebug() << "ui.function.functionVol update Vol value with : humidity" << (qint32)data->uvalues().r.s.humidity
//                 << "envtemp" <<  (qint32)data->uvalues().r.s.envtemp
//                 << "pressure" <<  (qint32)data->uvalues().r.s.pressure
//                 << "thro_1" <<  (qint32)data->uvalues().r.s.thro_1
//                 << "thro_2" <<  (qint32)data->uvalues().r.s.thro_2
//                 << "motorType" <<  (qint32)data->uvalues().r.s.motorType
//                 << "numOfMotor" <<  (qint32)data->uvalues().r.s.numOfMotor
//                 << "limitStatus" << data->uvalues().r.s.motorInfo.elec.limitStatus
//                 << "voltage" << data->uvalues().r.s.motorInfo.elec.voltage
//                 << "current [0]" << data->uvalues().r.s.motorInfo.elec.elecMotorStruct[0].current
//                 << "lift [0]" << data->uvalues().r.s.motorInfo.elec.elecMotorStruct[0].lift
//                 << "torque [0]" << data->uvalues().r.s.motorInfo.elec.elecMotorStruct[0].torque
//                 << "speed [0]" << data->uvalues().r.s.motorInfo.elec.elecMotorStruct[0].speed
//                 << "temp_1 [0]" << data->uvalues().r.s.motorInfo.elec.elecMotorStruct[0].temp_1
//                 << "temp_2 [0]" << data->uvalues().r.s.motorInfo.elec.elecMotorStruct[0].temp_2
//                 << "current [1]" << data->uvalues().r.s.motorInfo.elec.elecMotorStruct[1].current
//                 << "lift [1]" << data->uvalues().r.s.motorInfo.elec.elecMotorStruct[1].lift
//                 << "torque [1]" << data->uvalues().r.s.motorInfo.elec.elecMotorStruct[1].torque
//                 << "speed [1]" << data->uvalues().r.s.motorInfo.elec.elecMotorStruct[1].speed
//                 << "temp_1 [1]" << data->uvalues().r.s.motorInfo.elec.elecMotorStruct[1].temp_1
//                 << "temp_2 [1]" << data->uvalues().r.s.motorInfo.elec.elecMotorStruct[1].temp_2;

//        qDebug() << "ui.function.functionVol update Vol value with :" << data->uvalues().r.s.motorInfo.elec.voltage;

        v = (qint32)data->uvalues().r.s.motorInfo.elec.voltage;
        if (v){
            lastNoneZeroVol = v;
        }
        else{
            v = lastNoneZeroVol;
        }
    }
    else
    {
        v = 0;
        qCritical() << "com.ui.functions Voltage can not enabled on non-elec motor type";
    }
    return v;
};

/*
 *  unit : 10mv
 */
const static formulaT formulaVol = [](const qint32 v){
    return (double)(v/100);
};

const static functionT functionCurrent = [](const QModbus2DataUnit* data, const JsonPVConfig& config, const indexOnMotor idx){
    static quint32 lastNoneZeroVol;
    qint32 rtn = 0;
    //if (config.motorType() == QModbus2DataUnit::MotorTypeEnum::ELECE)
    if (data->uvalues().r.s.motorType == (quint8)QModbus2DataUnit::MotorTypeEnum::ELECE)
    {
        // idx 0 ->1st one 1-> 2nd one
        if ((idx.idxMotor()+1) <= config.numOfMotor())
        {
            qDebug() << "ui.function.functionVol update Current value with :"
                     << static_cast<qint32>(data->uvalues().r.s.motorInfo.elec.elecMotorStruct[idx.idxMotor()].current);
            rtn = static_cast<qint32>(data->uvalues().r.s.motorInfo.elec.elecMotorStruct[idx.idxMotor()].current);

            if (rtn){
                lastNoneZeroVol = rtn;
            }
            else{
                rtn = lastNoneZeroVol;
            }
        }
        else
        {
            rtn = 0;
            qCritical() << "com.ui.functions Current idx was not legal(0 or 1) one value == " << idx.idxMotor();
        }
    }
    else
    {
        qCritical() << "com.ui.functions Voltage can not enabled on non-elec motor type";
    }
    return rtn;
};
/*
 *  unit:   10mA
 */
const static formulaT formulaCurrent = [](const qint32 v){
    return (double)(v/100);
};


const static functionT functionThrust = [](const QModbus2DataUnit* data, const JsonPVConfig& config, const indexOnMotor idx){
    qint32 rtn = 0;
    //if (config.motorType() == QModbus2DataUnit::MotorTypeEnum::ELECE)
    if (data->uvalues().r.s.motorType == (quint8)QModbus2DataUnit::MotorTypeEnum::ELECE)
    {
        // idx 0 ->1st one 1-> 2nd one
        if ((idx.idxMotor()+1) <= config.numOfMotor())
        {     
            rtn = static_cast<qint32>(data->uvalues().r.s.motorInfo.elec.elecMotorStruct[idx.idxMotor()].lift);
            qDebug() << "com.ui.function.functionForce update Force value with :" << rtn;
        }
        else
        {
            qCritical() << "com.ui.functions Current idx was not legal(0 or 1) one value == " << idx.idxMotor();
        }
    }
    else if (data->uvalues().r.s.motorType == (quint8)QModbus2DataUnit::MotorTypeEnum::OILE)
    {
        rtn = static_cast<qint32>(data->uvalues().r.s.motorInfo.oil.lift);
    }
    return rtn;
};

/*
 * Actually this value shoudle be calibrated at each boot time. -- eshenhu
 * Zero value = 0x007FFFFF = 8388607
 * (v - 0x007FFFFF)/46.86
*/
const static formulaT formulaThrust = [](const qint32 v){
    return ((double)(v - 8388607)/4686)*100;
};

const static functionT functionThrottle = [](const QModbus2DataUnit* data, const JsonPVConfig& config, const indexOnMotor idx){
    Q_UNUSED(config)
    return idx.idxOneMotor() == 0 ? static_cast<qint32>(data->uvalues().r.s.thro_1) : static_cast<qint32>(data->uvalues().r.s.thro_2);
};

const static formulaT formulaThrottle = [](const qint32 v){
    return (double)v;
};

const static functionT functionTorque = [](const QModbus2DataUnit* data, const JsonPVConfig& config, const indexOnMotor idx){
    Q_UNUSED(config)

    qint32 rtn = 0;
    if (data->uvalues().r.s.motorType == (quint8)QModbus2DataUnit::MotorTypeEnum::ELECE)
    {
        // idx 0 ->1st one 1-> 2nd one
        if ((idx.idxMotor()+1) <= config.numOfMotor()){
            rtn = data->uvalues().r.s.motorInfo.elec.elecMotorStruct[idx.idxMotor()].torque;
        }
        else
        {
            qCritical() << "com.ui.functions torque idx was not legal(0 or 1) one value == " << idx.idxMotor();
        }
    }
    else
    {
        qCritical() << "com.ui.functions torque can not enabled on non-elec motor type";
    }
    return rtn;
};

/*
 * Actually this value shoudle be calibrated at each boot time. -- eshenhu
 * Zero value = 0x007FFFFF = 8388607
 *
*/
const static formulaT formulaTorque = [](const qint32 v){
    return ((double)(v - 8388607)/6133)*100;
};

const static functionT functionSpeed = [](const QModbus2DataUnit* data, const JsonPVConfig& config, const indexOnMotor idx){
    qint32 rtn = 0;
    //if (config.motorType() == QModbus2DataUnit::MotorTypeEnum::ELECE)
    if (data->uvalues().r.s.motorType == (quint8)QModbus2DataUnit::MotorTypeEnum::ELECE)
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
    else if (data->uvalues().r.s.motorType == (quint8)QModbus2DataUnit::MotorTypeEnum::OILE)
    {
        rtn = static_cast<qint32>(data->uvalues().r.s.motorInfo.oil.speed);
    }

    return rtn;
};

const static formulaT formulaSpeed = [](const qint32 v){
    //y = 60 000 000 / (x * 6 * (pole / 2))
    CfgResHandlerInf* pCfgResHdl = ActionWidget::getCfgResHdl();
    quint32 vanes = pCfgResHdl->vane();

    return (double)(20000000L/(v*vanes));
};

const static functionT functionTemp = [](const QModbus2DataUnit* data, const JsonPVConfig& config, const indexOnMotor idx){
    qint32 rtn = 0;
    //if (config.motorType() == QModbus2DataUnit::MotorTypeEnum::ELECE)
    if (data->uvalues().r.s.motorType == (quint8)QModbus2DataUnit::MotorTypeEnum::ELECE)
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
    else if (data->uvalues().r.s.motorType == (quint8)QModbus2DataUnit::MotorTypeEnum::OILE)
    {
        rtn = idx.idxOneMotor() == 0 ? static_cast<qint32>(data->uvalues().r.s.motorInfo.oil.temp_1) :
                                       static_cast<qint32>(data->uvalues().r.s.motorInfo.oil.temp_2);
    }
    return rtn;
};

//y = x / 4095.00 * 1023.75
const static formulaT formulaTemp = [](const qint32 v){
    return (double)(v)*1023.75/4095;
};

const static functionT functionPowerEffect = [](const QModbus2DataUnit* data, const JsonPVConfig& config, const indexOnMotor idx){
    qint32 rtn = 0;
    //if (config.motorType() == QModbus2DataUnit::MotorTypeEnum::ELECE)
    if (data->uvalues().r.s.motorType == (quint8)QModbus2DataUnit::MotorTypeEnum::ELECE)
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
    //if (config.motorType() == QModbus2DataUnit::MotorTypeEnum::ELECE)
    if (data->uvalues().r.s.motorType == (quint8)QModbus2DataUnit::MotorTypeEnum::ELECE)
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
