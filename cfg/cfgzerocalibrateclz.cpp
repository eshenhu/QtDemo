#include "cfgzerocalibrateclz.h"

//const quint32 CfgZeroCalibrateClz::MAX_SUPPORT_MOTOR;
//default value. 0x7FFFFF indicate the 0;
quint32 CfgZeroCalibrateClz::zeroThrustCalibrate[MAX_SUPPORT_MOTOR] = { 0x7FFFFF, 0x7FFFFF};
//default value. 0x7FFFFF indicate the 0;
quint32 CfgZeroCalibrateClz::zeroTorqueCalibrate[MAX_SUPPORT_MOTOR] = { 0x7FFFFF, 0x7FFFFF};

quint32 CfgZeroCalibrateClz::divisionOnThrust[MAX_SUPPORT_MOTOR] = { 45.17, 1.00 };
quint32 CfgZeroCalibrateClz::divisionOnTorque[MAX_SUPPORT_MOTOR] = { 61.36, 1.00 };
