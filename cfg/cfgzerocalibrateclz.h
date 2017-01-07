#ifndef CFGZEROCALIBRATECLZ_H
#define CFGZEROCALIBRATECLZ_H

#include <QtGlobal>
#include <QDebug>

//#define MAX_SUPPORT_MOTOR 2

class CfgZeroCalibrateClz {

public:
    CfgZeroCalibrateClz(){}

    static void reset()
    {
        for(auto&& value : zeroThrustCalibrate)
            value = 0x7FFFFF;
        for(auto&& value : zeroTorqueCalibrate)
            value = 0x7FFFFF;
    }

    static quint32 getStaticThrustZeroCaliOnMotor(const quint32 idxMotor)
    {
        if (idxMotor < MAX_SUPPORT_MOTOR){
            return zeroThrustCalibrate[idxMotor];
        }
        else{
            qWarning() << "Could not exceed the maximum supported motor " << MAX_SUPPORT_MOTOR;
            return 0;
        }
    }

    static void setStaticThrustZeroCaliOnMotor(const quint32 idxMotor, const quint32 v)
    {
        if (idxMotor < MAX_SUPPORT_MOTOR){
            if (v){
                // in case of overflow
                zeroThrustCalibrate[idxMotor] = v;
            }
        }
        else{
            qWarning() << "Could not exceed the maximum supported motor " << MAX_SUPPORT_MOTOR;
        }
    }

    static void addStaticThrustZeroCaliOnMotor(const quint32 idxMotor, const quint32 v)
    {
        if (idxMotor < MAX_SUPPORT_MOTOR){
            if (v){
                // in case of overflow
                zeroThrustCalibrate[idxMotor] = zeroThrustCalibrate[idxMotor]/2 + v/2 + 1;
            }
        }
        else{
            qWarning() << "Could not exceed the maximum supported motor " << MAX_SUPPORT_MOTOR;
        }
    }

    static quint32 getStaticTorqueZeroCaliOnMotor(const quint32 idxMotor)
    {
        if (idxMotor < MAX_SUPPORT_MOTOR){
            return zeroTorqueCalibrate[idxMotor];
        }
        else{
            qWarning() << "Could not exceed the maximum supported motor " << MAX_SUPPORT_MOTOR;
            return 0;
        }
    }

    static void setStaticTorqueZeroCaliOnMotor(const quint32 idxMotor, const quint32 v)
    {
        if (idxMotor < MAX_SUPPORT_MOTOR){
            if (v){
                // in case of overflow
                zeroTorqueCalibrate[idxMotor] = v;
            }
        }
        else
        {
            qWarning() << "Could not exceed the maximum supported motor " << MAX_SUPPORT_MOTOR;
        }
    }

    static void addStaticTorqueZeroCaliOnMotor(const quint32 idxMotor, const quint32 v)
    {
        if (idxMotor < MAX_SUPPORT_MOTOR){
            if (v){
                // in case of overflow
                zeroTorqueCalibrate[idxMotor] = zeroTorqueCalibrate[idxMotor]/2 + v/2 + 1;
            }
        }
        else{
            qWarning() << "Could not exceed the maximum supported motor " << MAX_SUPPORT_MOTOR;
        }
    }

//    static quint32 getDivisionThrustCaliOnMotor(const quint32 idxMotor){
//        if (idxMotor < MAX_SUPPORT_MOTOR){
//            return divisionOnThrust[idxMotor];
//        }
//        else
//        {
//            qWarning() << "Could not exceed the maximum supported motor " << MAX_SUPPORT_MOTOR;
//            return 1;
//        }
//    }

//    static bool setDivisionThrustCaliOnMotor(const quint32 idxMotor, const double value){
//        bool result = false;
//        if (idxMotor < MAX_SUPPORT_MOTOR){
//            divisionOnThrust[idxMotor] = value;
//            result = true;
//        }
//        else
//        {
//            qWarning() << "Could not exceed the maximum supported motor " << MAX_SUPPORT_MOTOR;
//        }
//        return result;
//    }

//    static quint32 getDivisionTorqueCaliOnMotor(const quint32 idxMotor){
//        if (idxMotor < MAX_SUPPORT_MOTOR){
//            return divisionOnTorque[idxMotor];
//        }
//        else
//        {
//            qWarning() << "Could not exceed the maximum supported motor " << MAX_SUPPORT_MOTOR;
//            return 1;
//        }
//    }

//    static bool setDivisionTorqueCaliOnMotor(const quint32 idxMotor, const double value){
//        bool result = false;
//        if (idxMotor < MAX_SUPPORT_MOTOR){
//            divisionOnTorque[idxMotor] = value;
//            result = true;
//        }
//        else
//        {
//            qWarning() << "Could not exceed the maximum supported motor " << MAX_SUPPORT_MOTOR;
//        }
//        return result;
//    }
public:
    static const quint32 MAX_SUPPORT_MOTOR = 2;
    //default value. 0x7FFFFF indicate the 0;
    static quint32 zeroThrustCalibrate[MAX_SUPPORT_MOTOR];
    //default value. 0x7FFFFF indicate the 0;
    static quint32 zeroTorqueCalibrate[MAX_SUPPORT_MOTOR];

//    // AD calibrate with 10KG.
//    static double divisionOnThrust[MAX_SUPPORT_MOTOR];
//    // AD calibrate with
//    static double divisionOnTorque[MAX_SUPPORT_MOTOR];
};

#endif // CFGZEROCALIBRATECLZ_H
