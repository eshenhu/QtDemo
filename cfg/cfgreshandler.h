#ifndef CFGRESHANDLER_H
#define CFGRESHANDLER_H

#include "cfgreshandlerinf.h"

#include <QObject>
#include <QSettings>

class CfgMotorBootCfgModel
{
public:
    CfgMotorBootCfgModel(QSettings& set);
    ~CfgMotorBootCfgModel(){}

public:
    void set_boot_delay(const quint32 &boot_delay);
    void set_boot_rape(const quint32 &boot_rape);

    quint32 boot_delay() const;
    quint32 boot_rape() const;


    quint32 duration() const;
    void set_duration(const quint32 &duration);

private:
    void loadSetting();

private:
    quint32 m_boot_delay;
    quint32 m_boot_rape;
    quint32 m_duration;
    QSettings& m_set;
};

class CfgDeviceCfgModel
{
public:
    CfgDeviceCfgModel(QSettings& set);
    ~CfgDeviceCfgModel(){}

public:

    quint32 vane() const;
    void setVane(const quint32 &vane);

    quint32 HZ() const;
    void setHZ(const quint32 &HZ);
private:
    void loadSetting();

private:
    quint32 m_vane;
    quint32 m_HZ;
    QSettings& m_set;
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
private:
    QSettings m_setting;
    CfgMotorBootCfgModel* m_bootCfg = nullptr;
    CfgDeviceCfgModel* m_deviceCfg = nullptr;

signals:

public slots:

    // CfgResHandlerInf interface
public:
    inline quint32 boot_delay() const
     { return m_bootCfg->boot_delay(); }
    inline quint32 boot_rape() const
     { return m_bootCfg->boot_rape(); }
    inline quint32 duration() const
     { return m_bootCfg->duration(); }
    inline quint32 vane() const
     { return m_deviceCfg->vane();}
    inline quint32 HZ() const
     { return m_deviceCfg->HZ();}

};

#endif // CFGRESHANDLER_H
