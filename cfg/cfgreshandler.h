#ifndef CFGRESHANDLER_H
#define CFGRESHANDLER_H

#include "cfgreshandlerinf.h"
#include <climits>

#include <QObject>
#include <QSettings>
#include <QtGlobal>


static const quint32 MAX_SUPPORT_MOTOR = 2;

struct CfgMotorProdVersionStu
{
    CfgResHandlerInf::ProductVersion v;
    CfgResHandlerInf::MotorType m;
    quint8 numberOfMotor;  // 1 = 1
    quint8 maxPower;  // 1kW = 10  1.5kw = 15
    quint8 maxVol;    // 30v = 30
    quint8 maxCur;    // 50A = 50 2*50A = 100
    quint8 maxTorque; // 15N*M = 15
    quint8 maxThrust;  // 30kg = 30
    quint16 maxDistance; // 500mm = 500
};

const static CfgMotorProdVersionStu tabelCfgMotorProdVer[(quint8)CfgResHandlerInf::ProductVersion::MAX] =
{
    {CfgResHandlerInf::ProductVersion::INVALID, CfgResHandlerInf::MotorType::INVALID,0,  0,  0,  0, 0,  0,  0},
    {CfgResHandlerInf::ProductVersion::PV1,     CfgResHandlerInf::MotorType::PELEC,  1, 15, 20, 30, 15, 15,  0},
    {CfgResHandlerInf::ProductVersion::PV2,     CfgResHandlerInf::MotorType::PELEC,  1, 30, 30,100, 30, 0,  0},
    {CfgResHandlerInf::ProductVersion::PV3,     CfgResHandlerInf::MotorType::PELEC,  1, 30, 55, 55, 30, 0,  0},
    {CfgResHandlerInf::ProductVersion::PV4,     CfgResHandlerInf::MotorType::PELEC,  1, 15, 30, 50, 15, 0,  0},
    {CfgResHandlerInf::ProductVersion::PV5,     CfgResHandlerInf::MotorType::PELEC,  1, 30, 30,100, 30, 0,  0},
    {CfgResHandlerInf::ProductVersion::PV6,     CfgResHandlerInf::MotorType::PELEC,  1, 30, 55, 55, 30, 0,  0},
    {CfgResHandlerInf::ProductVersion::PV7,     CfgResHandlerInf::MotorType::PELEC,  1, 50, 55, 90, 50, 0,  0},
    {CfgResHandlerInf::ProductVersion::PV8,     CfgResHandlerInf::MotorType::PELEC,  1, 50,100, 50, 50, 0,  0},
    {CfgResHandlerInf::ProductVersion::PV9,     CfgResHandlerInf::MotorType::PELEC,  2, 60, 30,200, 60, 0,  0},
    {CfgResHandlerInf::ProductVersion::PV10,    CfgResHandlerInf::MotorType::PELEC,  2, 60, 55,110, 60, 0,  0},
    {CfgResHandlerInf::ProductVersion::PV11,    CfgResHandlerInf::MotorType::PELEC,  2, 50, 55, 90, 50, 30, 500},
    {CfgResHandlerInf::ProductVersion::PV12,    CfgResHandlerInf::MotorType::PELEC,  2, 50,100,100,100, 0,  0}
};

class CfgMotorBootCfgModel
{
public:
    CfgMotorBootCfgModel(QSettings& set);
    ~CfgMotorBootCfgModel(){}

public:
    void set_boot_delay(const quint32 &boot_delay);
    void set_PRP(const quint32 &PRP);
    void set_boot_rape(const quint32 &boot_rape);

    quint32 boot_delay() const;
    quint32 PRP() const;
    quint32 boot_rape() const;

    quint32 bootVol() const;
    void setBootVol(const quint32 &bootVol);

private:
    void loadSetting();

private:
    quint32 m_boot_delay;
    quint32 m_boot_rape;
    quint32 m_PRP;
    quint32 m_bootVol;
    QSettings& m_set;
};

class CfgDeviceCfgModel
{
public:
    CfgDeviceCfgModel(QSettings& set);
    ~CfgDeviceCfgModel(){}

public:
    QString manufacture() const;
    void setManufacture(const QString &manufacture);

    QString motorType() const;
    void setMotorType(const QString &motorType);

    QString ESCType() const;
    void setESCType(const QString &ESCType);

    quint32 vane() const;
    void setVane(const quint32 &vane);

    quint32 HZ() const;
    void setHZ(const quint32 &HZ);

    quint32 lowThroLimit() const;
    void setLowThroLimit(const quint32 &lowThroLimit);

    quint32 highThroLimit() const;
    void setHighThroLimit(const quint32 &highThroLimit);

    QString SerialNumber() const;
    void setSerialNumber(const QString &SerialNumber);

    QByteArray key() const;
    void setKey(const QByteArray &key);

    QString path() const;
    void setPath(const QString &path);

private:
    void loadSetting();

private:
    QByteArray m_key;
    QString m_path;

    QString m_manufacture;
    QString m_motorType;
    QString m_ESCType;
    quint32 m_vane;
    quint32 m_HZ;

    quint32 m_lowThroLimit;
    quint32 m_highThroLimit;

    QString m_SerialNumber;

    QSettings& m_set;
};

class CfgProductVersionCfgModel
{
public:
    CfgProductVersionCfgModel(QSettings& set);
    ~CfgProductVersionCfgModel(){}

public:
    CfgResHandlerInf::MotorType motor_type() const;
    quint32 num_of_motor() const;
    quint32 max_power() const;
    quint32 max_vol() const;
    quint32 max_cur() const;
    quint32 max_torque() const;
    quint32 max_thrust() const;
    quint32 max_distance() const;
    CfgResHandlerInf::ProductVersion prod_version() const;

private:
    void loadSetting();

private:
    QSettings& m_set;
    CfgResHandlerInf::ProductVersion m_prod;
};


static const double defaultDivisionOnThrust[MAX_SUPPORT_MOTOR] = { 45.17, 46.64 };
static const double defaultDivisionOnTorque[MAX_SUPPORT_MOTOR] = { 61.36, 58.56 };

class CfgCalibrateCfgModel
{
    struct CfgCalibrateCfgPerMotor
    {
        double divisionOnThrust;
        double divisionOnTorque;
    };

public:
    CfgCalibrateCfgModel(QSettings& set);
    ~CfgCalibrateCfgModel(){}

public:
    double getDivisionThrustCaliOnMotor(const quint32 idxMotor);
    bool setDivisionThrustCaliOnMotor(const quint32 idxMotor, const double value);

    double getDivisionTorqueCaliOnMotor(const quint32 idxMotor);
    bool setDivisionTorqueCaliOnMotor(const quint32 idxMotor, const double value);

private:
    void loadSetting();

private:
    QSettings& m_set;
    CfgCalibrateCfgPerMotor CfgCalibrateCfgPerMotor[MAX_SUPPORT_MOTOR];
};

class CfgResHandler : public QObject, public CfgResHandlerInf
{
    Q_OBJECT
    Q_INTERFACES(CfgResHandlerInf)
public:
    explicit CfgResHandler(QObject *parent = 0);
    ~CfgResHandler();

public:
    CfgMotorBootCfgModel *bootCfg() const;
    CfgDeviceCfgModel *deviceCfg() const;
    CfgProductVersionCfgModel *prodCfg() const;
    CfgCalibrateCfgModel *calibrateCfg() const;

private:
    QSettings m_setting;
    CfgMotorBootCfgModel* m_bootCfg = nullptr;
    CfgDeviceCfgModel* m_deviceCfg = nullptr;
    CfgProductVersionCfgModel* m_prodCfg = nullptr;
    CfgCalibrateCfgModel* m_calibrateCfg = nullptr;

signals:

public slots:

    // CfgResHandlerInf interface
public:
    inline quint32 boot_delay() const
    {
        Q_ASSERT(m_bootCfg != nullptr);
        return m_bootCfg->boot_delay();
    }
    inline quint32 boot_PRP() const
    {
        Q_ASSERT(m_bootCfg != nullptr);
        return m_bootCfg->PRP();
    }
    inline quint32 boot_rape() const
    {
        Q_ASSERT(m_bootCfg != nullptr);
        return m_bootCfg->boot_rape();
    }
//    inline quint32 bootVol() const
//    {
//        Q_ASSERT(m_bootCfg != nullptr);
//        return m_bootCfg->bootVol();
//    }
    inline QByteArray key() const
    {
        Q_ASSERT(m_deviceCfg != nullptr);
        return m_deviceCfg->key();
    }
    inline QString manufacture() const
    {
        Q_ASSERT(m_deviceCfg != nullptr);
        return m_deviceCfg->manufacture();
    }
    inline QString motorType() const
    {
        Q_ASSERT(m_deviceCfg != nullptr);
        return m_deviceCfg->motorType();
    }
    inline QString ESCType() const
    {
        Q_ASSERT(m_deviceCfg != nullptr);
        return m_deviceCfg->ESCType();
    }
    inline QString SerialNumber() const
    {
        Q_ASSERT(m_deviceCfg != nullptr);
        return m_deviceCfg->SerialNumber();
    }
    inline quint32 vane() const
    {
        Q_ASSERT(m_deviceCfg != nullptr);
        return m_deviceCfg->vane();
    }
    inline quint32 HZ() const
    {
        Q_ASSERT(m_deviceCfg != nullptr);
        return m_deviceCfg->HZ();
    }
    inline quint32 lowThroLimit() const
    {
        Q_ASSERT(m_deviceCfg != nullptr);
        return m_deviceCfg->lowThroLimit();
    }
    inline quint32 highThroLimit() const
    {
        Q_ASSERT(m_deviceCfg != nullptr);
        return m_deviceCfg->highThroLimit();
    }


    MotorType motor_type() const
    {
        Q_ASSERT(m_prodCfg != nullptr);
        return m_prodCfg->motor_type();
    }
    quint32 num_of_motor() const
    {
        Q_ASSERT(m_prodCfg != nullptr);
        return m_prodCfg->num_of_motor();
    }
    quint32 max_power() const
    {
        Q_ASSERT(m_prodCfg != nullptr);
        return m_prodCfg->max_power();
    }
    quint32 max_vol() const
    {
        Q_ASSERT(m_prodCfg != nullptr);
        return m_prodCfg->max_vol();
    }
    quint32 max_cur() const
    {
        Q_ASSERT(m_prodCfg != nullptr);
        return m_prodCfg->max_cur();
    }
    quint32 max_torque() const
    {
        Q_ASSERT(m_prodCfg != nullptr);
        return m_prodCfg->max_torque();
    }
    quint32 max_thrust() const
    {
        Q_ASSERT(m_prodCfg != nullptr);
        return m_prodCfg->max_thrust();
    }
    quint32 max_throttle() const
    {
        return 100;
    }
    quint32 max_distance() const
    {
        Q_ASSERT(m_prodCfg != nullptr);
        return m_prodCfg->max_distance();
    }
    ProductVersion prod_version() const
    {
        Q_ASSERT(m_prodCfg != nullptr);
        return m_prodCfg->prod_version();
    }
    double getDivisionThrustCaliOnMotor(const quint32 idxMotor) const
    {
        Q_ASSERT(idxMotor <= MAX_SUPPORT_MOTOR);
        return m_calibrateCfg->getDivisionThrustCaliOnMotor(idxMotor);
    }
    double getDivisionTorqueCaliOnMotor(const quint32 idxMotor) const
    {
        Q_ASSERT(idxMotor <= MAX_SUPPORT_MOTOR);
        return m_calibrateCfg->getDivisionTorqueCaliOnMotor(idxMotor);
    }

};

#endif // CFGRESHANDLER_H
