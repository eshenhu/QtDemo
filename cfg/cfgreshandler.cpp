#include "cfgreshandler.h"

#include <QSettings>

CfgResHandler::CfgResHandler(QObject *parent) : QObject(parent),
    m_setting("drone", "meas")
{
    m_bootCfg = new CfgMotorBootCfgModel(m_setting);
    m_deviceCfg = new CfgDeviceCfgModel(m_setting);
    m_prodCfg = new CfgProductVersionCfgModel(m_setting);
}

CfgResHandler::~CfgResHandler()
{
    if(m_bootCfg)
        delete m_bootCfg;
    if(m_deviceCfg)
        delete m_deviceCfg;
    if(m_prodCfg)
        delete m_prodCfg;
}

CfgMotorBootCfgModel *CfgResHandler::bootCfg() const
{
    return m_bootCfg;
}

CfgDeviceCfgModel *CfgResHandler::deviceCfg() const
{
    return m_deviceCfg;
}

CfgProductVersionCfgModel *CfgResHandler::prodCfg() const
{
    return m_prodCfg;
}

CfgMotorBootCfgModel::CfgMotorBootCfgModel(QSettings& set):
    m_boot_delay(5),
    m_boot_rape(10),
    m_bootVol(5),
    m_set(set)
{
    loadSetting();
}

void CfgMotorBootCfgModel::loadSetting()
{
    m_set.beginGroup("cfg/motor");
    m_boot_delay = m_set.value("boot_delay", 5).toInt();
    m_boot_rape  = m_set.value("boot_rape", 10).toInt();
    m_duration = m_set.value("duration", 10).toInt();
    m_bootVol = m_set.value("boot_vol", 5).toInt();
    m_set.endGroup();
}

quint32 CfgMotorBootCfgModel::bootVol() const
{
    return m_bootVol;
}

void CfgMotorBootCfgModel::setBootVol(const quint32 &bootVol)
{
    m_bootVol = bootVol;
    m_set.setValue("cfg/motor/boot_vol", m_bootVol);
}

quint32 CfgMotorBootCfgModel::duration() const
{
    return m_duration;
}

void CfgMotorBootCfgModel::set_duration(const quint32 &duration)
{
    m_duration = duration;
    m_set.setValue("cfg/motor/duration", m_duration);
}

void CfgMotorBootCfgModel::set_boot_delay(const quint32 &boot_delay)
{
    m_boot_delay = boot_delay;
    m_set.setValue("cfg/motor/boot_delay", m_boot_delay);
}

void CfgMotorBootCfgModel::set_boot_rape(const quint32 &boot_rape)
{
    m_boot_rape = boot_rape;
    m_set.setValue("cfg/motor/boot_rape", m_boot_rape);
}

quint32 CfgMotorBootCfgModel::boot_delay() const
{
    return m_boot_delay;
}

quint32 CfgMotorBootCfgModel::boot_rape() const
{
    return m_boot_rape;
}

CfgDeviceCfgModel::CfgDeviceCfgModel(QSettings &set):
    m_vane(1),
    m_HZ(50),
    m_set(set)
{
    loadSetting();
}

quint32 CfgDeviceCfgModel::vane() const
{
    return m_vane;
}

void CfgDeviceCfgModel::setVane(const quint32 &vane)
{
    m_vane = vane;
}

quint32 CfgDeviceCfgModel::HZ() const
{
    return m_HZ;
}

void CfgDeviceCfgModel::setHZ(const quint32 &HZ)
{
    m_HZ = HZ;
}

void CfgDeviceCfgModel::loadSetting()
{
    m_set.beginGroup("cfg/device");
    m_vane = m_set.value("vane", 1).toInt();
    m_HZ  = m_set.value("HZ", 50).toInt();
    m_set.endGroup();
}

CfgProductVersionCfgModel::CfgProductVersionCfgModel(QSettings &set):
    m_set(set)
{
    loadSetting();
}

CfgResHandlerInf::MotorType CfgProductVersionCfgModel::motor_type() const
{
    return tabelCfgMotorProdVer[(quint8)m_prod].m;
}

quint32 CfgProductVersionCfgModel::num_of_motor() const
{
    return tabelCfgMotorProdVer[(quint8)m_prod].numberOfMotor;
}

quint32 CfgProductVersionCfgModel::max_power() const
{
    return tabelCfgMotorProdVer[(quint8)m_prod].maxPower;
}

quint32 CfgProductVersionCfgModel::max_vol() const
{
    return tabelCfgMotorProdVer[(quint8)m_prod].maxVol;
}

quint32 CfgProductVersionCfgModel::max_cur() const
{
    return tabelCfgMotorProdVer[(quint8)m_prod].maxCur;
}

quint32 CfgProductVersionCfgModel::max_torque() const
{
    return tabelCfgMotorProdVer[(quint8)m_prod].maxTorque;
}

CfgResHandlerInf::ProductVersion CfgProductVersionCfgModel::prod_version() const
{
    return CfgResHandlerInf::ProductVersion::PV11;
}

void CfgProductVersionCfgModel::loadSetting()
{
    m_set.beginGroup("cfg/product");

    m_prod = static_cast<CfgResHandlerInf::ProductVersion>(m_set.value("version", 0).toInt());
    if ((quint8)m_prod >= (quint8)CfgResHandlerInf::ProductVersion::MAX)
        m_prod = CfgResHandlerInf::ProductVersion::INVALID;

    m_set.endGroup();
}
