#include "cfgreshandler.h"

#include <QSettings>

CfgResHandler::CfgResHandler(QObject *parent) : QObject(parent),
    m_setting("mpth", "meas")
{
    m_bootCfg = new CfgMotorBootCfgModel(m_setting);
}

CfgResHandler::~CfgResHandler()
{
    if(m_bootCfg)
        delete m_bootCfg;
}

CfgMotorBootCfgModel *CfgResHandler::bootCfg() const
{
    return m_bootCfg;
}

CfgDeviceCfgModel *CfgResHandler::deviceCfg() const
{
    return m_deviceCfg;
}

CfgMotorBootCfgModel::CfgMotorBootCfgModel(QSettings& set):
    m_boot_delay(5),
    m_boot_rape(10),
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
    m_set.endGroup();
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
