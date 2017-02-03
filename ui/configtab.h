#ifndef CONFIGTAB_H
#define CONFIGTAB_H

#include <QWidget>

class DeviceInfoConfig;
class OptionsFrame;
class CfgResHandler;
class ProductVersion;

class ConfigTab : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigTab(CfgResHandler* hdl, QWidget *parent = 0);
    ~ConfigTab();

private:
    void createSubPanel();

signals:

public slots:

public:
   DeviceInfoConfig* m_deviceInfoWidget;
   OptionsFrame* m_optionsWidget;
   ProductVersion* m_versionWidget;
   CfgResHandler* m_hdl;
};


#endif // CONFIGTAB_H
