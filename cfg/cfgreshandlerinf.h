#ifndef CFGRESHANDLERINF_H
#define CFGRESHANDLERINF_H

#include <qglobal.h>
#include <QtPlugin>

class CfgResHandlerInf
{
public:
    enum class ProductVersion : quint8 {INVALID = 0, PV1, PV2, PV3, PV4, PV5, PV6,
                                         PV7, PV8, PV9, PV10, PV11, PV12, MAX};

    enum class MotorType: quint8 { INVALID, PELEC, POIL };

public:
    virtual ~CfgResHandlerInf() {}

    virtual quint32 boot_delay() const = 0;
    virtual quint32 boot_rape() const = 0;
    virtual quint32 boot_PRP() const = 0;
    virtual quint32 bootVol() const = 0;

    virtual quint32 vane() const = 0;
    virtual quint32 HZ() const = 0;

    virtual MotorType motor_type() const = 0;
    virtual quint32 num_of_motor() const = 0;
    virtual quint32 max_power() const = 0;
    virtual quint32 max_vol() const = 0;
    virtual quint32 max_cur() const = 0;
    virtual quint32 max_torque() const = 0;
    virtual quint32 max_thrust() const = 0;
    virtual quint32 max_throttle() const = 0;
    virtual ProductVersion prod_version() const = 0;
};

Q_DECLARE_INTERFACE(CfgResHandlerInf, "CfgResHandlerInf/1.0")

#endif // CFGRESHANDLERINF_H
