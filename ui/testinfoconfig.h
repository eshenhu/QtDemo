#ifndef TESTINFOCONFIG_H
#define TESTINFOCONFIG_H

#include <QFrame>

QT_BEGIN_NAMESPACE

namespace Ui {
class DeviceInfoConfig;
}
QT_END_NAMESPACE

class CfgDeviceCfgModel;

class DeviceInfoConfig : public QFrame
{
    Q_OBJECT

public:
    enum Freq {
        B50HZ = 0,
        B400HZ = 1
    };
    Q_ENUMS(Freq)

    enum Vanes {
        V1 = 1,
        V2,
        V3,
        V4,
        V5,
        V6,
        V7
    };
    Q_ENUMS(Vanes)

    struct Settings {
        QString manufacture = tr("实录光电");
        QString serialNumber = "SN_";
        QString motorModel = "Motor_";
        QString escModel = "ESC" ;
        DeviceInfoConfig::Freq freq = B50HZ;
        QString propModel = "PRP_";
        DeviceInfoConfig::Vanes propIndex = V1;
        QString remark = tr("SH ShiLu");
    };

public:
    explicit DeviceInfoConfig(CfgDeviceCfgModel* cfg, QWidget *parent = 0);
    ~DeviceInfoConfig();

private:
    Ui::DeviceInfoConfig *ui;

    Q_PROPERTY(Settings setting READ get_setting WRITE set_setting)
    Settings m_setting;
    CfgDeviceCfgModel* const m_cfg;

public:
    Settings get_setting() const
    {
        return m_setting;
    }

    void set_setting(DeviceInfoConfig::Settings set)
    {
        m_setting = set;
    }
};

#endif // TESTINFOCONFIG_H
