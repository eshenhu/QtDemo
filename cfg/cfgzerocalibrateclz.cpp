#include "cfgzerocalibrateclz.h"

//const quint32 CfgZeroCalibrateClz::MAX_SUPPORT_MOTOR;
//default value. 0x7FFFFF indicate the 0;
quint32 CfgZeroCalibrateClz::zeroThrustCalibrate[CfgZeroCalibrateClz::MAX_SUPPORT_MOTOR] = { 0x7FFFFF, 0x7FFFFF};
//default value. 0x7FFFFF indicate the 0;
quint32 CfgZeroCalibrateClz::zeroTorqueCalibrate[CfgZeroCalibrateClz::MAX_SUPPORT_MOTOR] = { 0x7FFFFF, 0x7FFFFF};

// { 45.17, 46.64 }
double CfgZeroCalibrateClz::divisionOnThrust[CfgZeroCalibrateClz::MAX_SUPPORT_MOTOR] = { 45.17, 46.64 };
double CfgZeroCalibrateClz::divisionOnTorque[CfgZeroCalibrateClz::MAX_SUPPORT_MOTOR] = { 61.36, 58.56 };
