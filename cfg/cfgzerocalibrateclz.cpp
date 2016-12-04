#include "cfgzerocalibrateclz.h"

//const quint32 CfgZeroCalibrateClz::MAX_SUPPORT_MOTOR;
//default value. 0x7FFFFF indicate the 0;
quint32 CfgZeroCalibrateClz::zeroThrustCalibrate[MAX_SUPPORT_MOTOR] = { 0x7FFFFF, 0x7FFFFF};
//default value. 0x7FFFFF indicate the 0;
quint32 CfgZeroCalibrateClz::zeroTorqueCalibrate[MAX_SUPPORT_MOTOR] = { 0x7FFFFF, 0x7FFFFF};

quint32 CfgZeroCalibrateClz::value = 0;
