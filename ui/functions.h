#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QDebug>
#include "cfg/cfgjsonprimaryelement.h"
#include "comm/qmodbusdataunit.h"
#include "unireslocation.h"

#include "driver/modelpoctype.h"
#include "cfg/cfgzerocalibrateclz.h"

#include "ui/qextcheckbox.h"

using namespace ModelPOC;

static const quint32 divideByPower = 1000;
static const quint32 divedeByEffi_G_W = 1000;
static const quint32 divedeByEffi_G_A = 1000;
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
using formulaT = std::function<double(qint32, Phase, quint32)>;


const static functionT functionDummy = [](const QModbus2DataUnit* data, const JsonPVConfig& config, const indexOnMotor idx){
    Q_UNUSED(data)
    Q_UNUSED(config)
    Q_UNUSED(idx)
    return 0;
};
const static formulaT formulaDummy = [](const qint32 v, Phase phase, quint32 idxMotor){
    Q_UNUSED(v)
    Q_UNUSED(phase)
    Q_UNUSED(idxMotor)
    return 0;
};

//static const quint32 divideByHumidity = 1000;
const static functionT functionEnvHumidity = [](const QModbus2DataUnit* data, const JsonPVConfig& config, const indexOnMotor idx){
    Q_UNUSED(data)
    Q_UNUSED(config)
    Q_UNUSED(idx)

    quint32 v = (qint32)data->uvalues().r.s.humidity;
    return v;
};
const static formulaT formulaEnvHumidity = [](const qint32 v, Phase phase, quint32 idxMotor){
    Q_UNUSED(v)
    Q_UNUSED(phase)
    Q_UNUSED(idxMotor)

    return (double)(v)/1000;
};

const static functionT functionEnvTemp = [](const QModbus2DataUnit* data, const JsonPVConfig& config, const indexOnMotor idx){
    Q_UNUSED(data)
    Q_UNUSED(config)
    Q_UNUSED(idx)
    quint32 v = (qint32)data->uvalues().r.s.envtemp;
    return v;
};
const static formulaT formulaEnvTemp = [](const qint32 v, Phase phase, quint32 idxMotor){
    Q_UNUSED(v)
    Q_UNUSED(phase)
    Q_UNUSED(idxMotor)
    return (double)(v)/100;
};

const static functionT functionEnvPressure = [](const QModbus2DataUnit* data, const JsonPVConfig& config, const indexOnMotor idx){
    Q_UNUSED(data)
    Q_UNUSED(config)
    Q_UNUSED(idx)
    quint32 v = (qint32)data->uvalues().r.s.pressure;
    return v;
};
const static formulaT formulaEnvPressure = [](const qint32 v, Phase phase, quint32 idxMotor){
    Q_UNUSED(v)
    Q_UNUSED(phase)
    Q_UNUSED(idxMotor)
    return (double)(v)/100;
};

const static functionT functionVol = [](const QModbus2DataUnit* data, const JsonPVConfig& config, const indexOnMotor idx){
    Q_UNUSED(idx)
    Q_UNUSED(config)
    static quint32 lastNoneZeroVol;
    quint32 v;
    //if (config.motorType() == QModbus2DataUnit::MotorTypeEnum::ELECE)
    if (data->uvalues().r.s.motorType == (quint8)QModbus2DataUnit::MotorTypeEnum::ELECE)
    {
        qDebug() << "ui.function.functionVol update Vol value with : humidity" << (qint32)data->uvalues().r.s.humidity
                 << "envtemp" <<  (qint32)data->uvalues().r.s.envtemp
                 << "pressure" <<  (qint32)data->uvalues().r.s.pressure
                 << "thro_1" <<  (qint32)data->uvalues().r.s.thro_1
                 << "thro_2" <<  (qint32)data->uvalues().r.s.thro_2
                 << "motorType" <<  (qint32)data->uvalues().r.s.motorType
                 << "numOfMotor" <<  (qint32)data->uvalues().r.s.numOfMotor
                 << "limitStatus" << data->uvalues().r.s.motorInfo.elec.limitStatus
                 << "distancePos" << data->uvalues().r.s.motorInfo.elec.distancePos
                 << "voltage" << data->uvalues().r.s.motorInfo.elec.voltage
                 << "current [0]" << data->uvalues().r.s.motorInfo.elec.elecMotorStruct[0].current
                 << "lift [0]" << data->uvalues().r.s.motorInfo.elec.elecMotorStruct[0].lift
                 << "torque [0]" << data->uvalues().r.s.motorInfo.elec.elecMotorStruct[0].torque
                 << "speed [0]" << data->uvalues().r.s.motorInfo.elec.elecMotorStruct[0].speed
                 << "temp_1 [0]" << data->uvalues().r.s.motorInfo.elec.elecMotorStruct[0].temp_1
                 << "temp_2 [0]" << data->uvalues().r.s.motorInfo.elec.elecMotorStruct[0].temp_2
                 << "current [1]" << data->uvalues().r.s.motorInfo.elec.elecMotorStruct[1].current
                 << "lift [1]" << data->uvalues().r.s.motorInfo.elec.elecMotorStruct[1].lift
                 << "torque [1]" << data->uvalues().r.s.motorInfo.elec.elecMotorStruct[1].torque
                 << "speed [1]" << data->uvalues().r.s.motorInfo.elec.elecMotorStruct[1].speed
                 << "temp_1 [1]" << data->uvalues().r.s.motorInfo.elec.elecMotorStruct[1].temp_1
                 << "temp_2 [1]" << data->uvalues().r.s.motorInfo.elec.elecMotorStruct[1].temp_2;

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
const static formulaT formulaVol = [](const qint32 v, Phase phase, quint32 idxMotor){
    Q_UNUSED(phase)
    Q_UNUSED(idxMotor)
    return (double)(v)/100;
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
const static formulaT formulaCurrent = [](const qint32 v, Phase phase, quint32 idxMotor){
    Q_UNUSED(phase)
    Q_UNUSED(idxMotor)
    return (double)(v)/100;
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
const static formulaT formulaThrust = [](const qint32 v, Phase phase, quint32 idxMotor){
    Q_UNUSED(phase)

    double result = (double)v;

    qDebug() << "functions.h formulaThrust Motor " << idxMotor <<" Phase = " << (int)phase;

    if (phase == Phase::Phase_SoftStart)
    {
        CfgZeroCalibrateClz::setStaticThrustZeroCaliOnMotor(idxMotor, v);
        result = (double)CfgZeroCalibrateClz::getStaticThrustZeroCaliOnMotor(idxMotor);
    }
    else if (phase == Phase::Phase_PRPDelay)
    {
        CfgZeroCalibrateClz::addStaticThrustZeroCaliOnMotor(idxMotor, v);
        result = (double)CfgZeroCalibrateClz::getStaticThrustZeroCaliOnMotor(idxMotor);
    }

    if (phase == Phase::Phase_NomalRunning){
        QString str = QString("functions.h formulaThrust idxMotor = %1 formula = (%2 - %3)/ %4")
                    .arg(idxMotor)
                    .arg(v)
                    .arg(CfgZeroCalibrateClz::getStaticThrustZeroCaliOnMotor(idxMotor))
                    .arg(CfgZeroCalibrateClz::getDivisionThrustCaliOnMotor(idxMotor));
        qDebug() << str;

        result = (double)((qint32)(v) - (qint32)CfgZeroCalibrateClz::getStaticThrustZeroCaliOnMotor(idxMotor))
                                 / CfgZeroCalibrateClz::getDivisionThrustCaliOnMotor(idxMotor);
    }

    qDebug() << "functions.h formulaThrust  Motor " << idxMotor << " result = " << result;
    return result;
};

const static functionT functionThrottle = [](const QModbus2DataUnit* data, const JsonPVConfig& config, const indexOnMotor idx){
    Q_UNUSED(config)
    return idx.idxOneMotor() == 0 ? static_cast<qint32>(data->uvalues().r.s.thro_1) : static_cast<qint32>(data->uvalues().r.s.thro_2);
};

const static formulaT formulaThrottle = [](const qint32 v, Phase phase, quint32 idxMotor){
    Q_UNUSED(phase)
    Q_UNUSED(idxMotor)
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
const static formulaT formulaTorque = [](const qint32 v, Phase phase, quint32 idxMotor){
    Q_UNUSED(phase)
    Q_UNUSED(idxMotor)

    qDebug() << "functions.h formulaTorque Motor " << idxMotor << " Phase = " << (int)phase;

    double result = (double)v;
    if (phase == Phase::Phase_SoftStart)
    {
        CfgZeroCalibrateClz::setStaticTorqueZeroCaliOnMotor(idxMotor, v);
        result = CfgZeroCalibrateClz::getStaticTorqueZeroCaliOnMotor(idxMotor);
    }
    else if (phase == Phase::Phase_PRPDelay){
        CfgZeroCalibrateClz::addStaticTorqueZeroCaliOnMotor(idxMotor, v);
        result = CfgZeroCalibrateClz::getStaticTorqueZeroCaliOnMotor(idxMotor);
    }

    if (phase == Phase::Phase_NomalRunning)
    {
        QString str = QString("functions.h formulaTorque idxMotor = %1 formula = (%2 - %3)/ %4")
                    .arg(idxMotor)
                    .arg(v)
                    .arg(CfgZeroCalibrateClz::getStaticTorqueZeroCaliOnMotor(idxMotor))
                    .arg(CfgZeroCalibrateClz::getDivisionTorqueCaliOnMotor(idxMotor));
        qDebug() << str;

        result = ((double)((qint32)v - (qint32)CfgZeroCalibrateClz::getStaticTorqueZeroCaliOnMotor(idxMotor))
                  /CfgZeroCalibrateClz::getDivisionTorqueCaliOnMotor(idxMotor));
    }

    qDebug() << "functions.h formulaTorque Motor " << idxMotor << "  result = " << result/1000;

    return result/1000;
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

const static formulaT formulaSpeed = [](const qint32 v, Phase phase, quint32 idxMotor){
    Q_UNUSED(phase)
    Q_UNUSED(idxMotor)
    //y = 60 000 000 / (x * (pole / 2))
    CfgResHandlerInf* pCfgResHdl = UniResLocation::getCfgResHdl();
    quint32 vanes = pCfgResHdl->vane();

    if (v > 0)
        return (double)(60000000L/(v*(vanes/2)));
    else
        return (double)0;
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
const static formulaT formulaTemp = [](const qint32 v, Phase phase, quint32 idxMotor){
    Q_UNUSED(phase)
    Q_UNUSED(idxMotor)
    return (double)(v)*1023.75/4095;
};

const static functionT functionPower_G_W_Effect = [](const QModbus2DataUnit* data, const JsonPVConfig& config, const indexOnMotor idx){
    qint32 rtn = 0;
    if (data->uvalues().r.s.motorType == (quint8)QModbus2DataUnit::MotorTypeEnum::ELECE)
    {
        // idx 0 ->1st one 1-> 2nd one
        if ((idx.idxMotor()+1) <= config.numOfMotor())
        {
            //rtn =  static_cast<qint32>(data->uvalues().r.s.motorInfo.elec.elecMotorStruct[idx.idxMotor()].torque);
            double thrustData = 0.00;
            const QExtCheckBox* boxThrust = QExtCheckBox::searchExtCheckBox(JsonGUIPrimType::THRUST, idx.idxMotor());
            if (boxThrust)
                thrustData = boxThrust->pushData();

            double pwrData = 1.00;
            const QExtCheckBox* boxPwr = QExtCheckBox::searchExtCheckBox(JsonGUIPrimType::POWER, idx.idxMotor());
            if (boxPwr)
                pwrData = boxPwr->pushData();

            rtn = static_cast<quint32>(divedeByEffi_G_W * thrustData / pwrData);
        }
        else
        {
            qCritical() << "com.ui.functions Current idx was not legal(0 or 1) one value == " << idx.idxMotor();
        }
    }
    else
    {
        //need to be improved here -- eshenhu
        rtn = divedeByEffi_G_W;
    }
    return rtn;
};

const static formulaT formulaPower_G_W_Effect = [](const qint32 v, Phase phase, quint32 idxMotor){
    Q_UNUSED(phase)
    Q_UNUSED(idxMotor)
    return (double)v/divedeByEffi_G_W;
};

const static functionT functionPower_G_A_Effect = [](const QModbus2DataUnit* data, const JsonPVConfig& config, const indexOnMotor idx){
    qint32 rtn = 0;
    if (data->uvalues().r.s.motorType == (quint8)QModbus2DataUnit::MotorTypeEnum::ELECE)
    {
        // idx 0 ->1st one 1-> 2nd one
        if ((idx.idxMotor()+1) <= config.numOfMotor())
        {
            //rtn =  static_cast<qint32>(data->uvalues().r.s.motorInfo.elec.elecMotorStruct[idx.idxMotor()].torque);
            double thrustData = 0.00;
            const QExtCheckBox* boxThrust = QExtCheckBox::searchExtCheckBox(JsonGUIPrimType::THRUST, idx.idxMotor());
            if (boxThrust)
                thrustData = boxThrust->pushData();

            double currentData = 0.00;
            const QExtCheckBox* boxCurrent = QExtCheckBox::searchExtCheckBox(JsonGUIPrimType::CURRENT, idx.idxMotor());
            if (boxCurrent)
                currentData = boxCurrent->pushData();

            rtn = static_cast<quint32>(divedeByEffi_G_A * thrustData / currentData);
        }
        else
        {
            qCritical() << "com.ui.functions Current idx was not legal(0 or 1) one value == " << idx.idxMotor();
        }
    }
    else
    {
        // need to be improved here -- eshenhu
        rtn = divedeByEffi_G_A;
    }
    return rtn;
};

const static formulaT formulaPower_G_A_Effect = [](const qint32 v, Phase phase, quint32 idxMotor){
    Q_UNUSED(phase)
    Q_UNUSED(idxMotor)
    return (double)v/divedeByEffi_G_A;
};
/*
 *  for better presious requirement, we multipule 100 for this raw data. and divide by 100 in the formula.
 *  -- eshenhu
*/
const static functionT functionPower = [](const QModbus2DataUnit* data, const JsonPVConfig& config, const indexOnMotor idx){
    Q_UNUSED(data)
    qint32 rtn = 0;
    //if (config.motorType() == QModbus2DataUnit::MotorTypeEnum::ELECE)

    // idx 0 ->1st one 1-> 2nd one
    if ((idx.idxMotor()+1) <= config.numOfMotor())
    {
        double volData = 0.00;
        const QExtCheckBox* boxVol = QExtCheckBox::searchExtCheckBox(JsonGUIPrimType::VOLTAGE, idx.idxMotor());
        if (boxVol)
            volData = boxVol->pushData();

        double currentData = 0.00;
        const QExtCheckBox* boxCurrent = QExtCheckBox::searchExtCheckBox(JsonGUIPrimType::CURRENT, idx.idxMotor());
        if (boxCurrent)
            currentData = boxCurrent->pushData();

        rtn = divideByPower * volData * currentData;
    }
    else
    {
        qCritical() << "com.ui.functions Current idx was not legal(0 or 1) one value == " << idx.idxMotor();
    }

    return rtn;
};

const static formulaT formulaPower = [](const qint32 v, Phase phase, quint32 idxMotor){
    Q_UNUSED(phase)
    Q_UNUSED(idxMotor)
    return (double)v/divideByPower;
};

const static functionT functionMechaPower = [](const QModbus2DataUnit* data, const JsonPVConfig& config, const indexOnMotor idx)
{
    Q_UNUSED(data)
    Q_UNUSED(config)
    Q_UNUSED(idx)
    qint32 rtn = 0;
    return rtn;
};

/*
 * MechaPower = 2* pi * Torque * RPM / 60 = Torque * RPM / 9.549
 */
const static formulaT formulaMechaPower = [](const qint32 v, Phase phase, quint32 idxMotor){
    Q_UNUSED(phase)
    Q_UNUSED(idxMotor)
    Q_UNUSED(v)

    double torqueData = 0.00;
    const QExtCheckBox* boxTorque = QExtCheckBox::searchExtCheckBox(JsonGUIPrimType::TORQUE, idxMotor);
    if (boxTorque)
        torqueData = boxTorque->pushData();

    double rpmData = 0.00;
    const QExtCheckBox* boxSpeed = QExtCheckBox::searchExtCheckBox(JsonGUIPrimType::SPEED, idxMotor);
    if (boxSpeed)
        rpmData = boxSpeed->pushData();

    qDebug() << "Mecha Power is: torqueData" << torqueData << "rpmData" << rpmData;
    double result = torqueData * rpmData / 9.549;

    return result;
};

const static functionT functionMechaEffi = [](const QModbus2DataUnit* data, const JsonPVConfig& config, const indexOnMotor idx)
{
    Q_UNUSED(data)
    Q_UNUSED(config)
    Q_UNUSED(idx)
    qint32 rtn = 0;
    return rtn;
};
/*
 *  Mecha Power / Power * 100
 */
const static formulaT formulaMechaEffi = [](const qint32 v, Phase phase, quint32 idxMotor){
    Q_UNUSED(v)
    Q_UNUSED(phase)
    Q_UNUSED(idxMotor)

    double mechaPowerData = 0.00;
    const QExtCheckBox* boxMechaPower = QExtCheckBox::searchExtCheckBox(JsonGUIPrimType::MECHAPOWER, idxMotor);
    if (boxMechaPower)
        mechaPowerData = boxMechaPower->pushData();

    double powerData = 0.00;
    const QExtCheckBox* boxPower = QExtCheckBox::searchExtCheckBox(JsonGUIPrimType::POWER, idxMotor);
    if (boxPower)
        powerData = boxPower->pushData();

//    qDebug() << "Mecha Effi is: mechaPowerData" << mechaPowerData << "powerData" << powerData;

    if (powerData)
        return mechaPowerData * 100 / powerData;
    else
        return (double)0;
};

}

#endif // FUNCTIONS_H
