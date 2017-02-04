#ifndef CFGRESHANDLERINF_H
#define CFGRESHANDLERINF_H

#include <qglobal.h>
#include <QtPlugin>
#include <QVector>
#include <QString>

class CfgResHandlerInf
{
public:
    enum class ProductVersion : quint8 {INVALID = 0, PV1, PV2, PV3, PV4, PV5, PV6,
                                         PV7, PV8, PV9, PV10, PV11, PV12, MAX};

    static const QString getPVString(ProductVersion v)
    {
        static const QVector<QString> constStrPV{ QStringLiteral("INVALID"),
                                          QStringLiteral("PV1"),
                                          QStringLiteral("PV2"),
                                          QStringLiteral("PV3"),
                                          QStringLiteral("PV4"),
                                          QStringLiteral("PV5"),
                                          QStringLiteral("PV6"),
                                          QStringLiteral("PV7"),
                                          QStringLiteral("PV8"),
                                          QStringLiteral("PV9"),
                                          QStringLiteral("PV10"),
                                          QStringLiteral("PV11"),
                                          QStringLiteral("PV12")};

        QString result(constStrPV.at(0));
        const quint8 idx = static_cast<quint8>(v);
        if (idx < static_cast<quint8>(ProductVersion::MAX))
            result = constStrPV.at(idx);
        return result;
    }

    enum class MotorType: quint8 { POIL = 0, PELEC = 1, INVALID = 2 };

public:
    virtual ~CfgResHandlerInf() {}

    virtual quint32 boot_delay() const = 0;
    virtual quint32 boot_rape() const = 0;
    virtual quint32 boot_PRP() const = 0;
//    virtual quint32 bootVol() const = 0;

    virtual QByteArray key() const = 0;

    virtual quint32 vane() const = 0;
    virtual quint32 HZ() const = 0;
    virtual quint32 lowThroLimit() const = 0;
    virtual quint32 highThroLimit() const = 0;

    virtual MotorType motor_type() const = 0;
    virtual quint32 num_of_motor() const = 0;
    virtual quint32 max_power() const = 0;
    virtual quint32 max_vol() const = 0;
    virtual quint32 max_cur() const = 0;
    virtual quint32 max_torque() const = 0;
    virtual quint32 max_thrust() const = 0;
    virtual quint32 max_throttle() const = 0;
    virtual quint32 max_distance() const = 0;
    virtual ProductVersion prod_version() const = 0;


    virtual double getDivisionThrustCaliOnMotor(const quint32 idxMotor) const = 0;
    virtual double getDivisionTorqueCaliOnMotor(const quint32 idxMotor) const = 0;
};

Q_DECLARE_INTERFACE(CfgResHandlerInf, "CfgResHandlerInf/1.0")

#endif // CFGRESHANDLERINF_H
