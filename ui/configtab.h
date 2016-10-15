#ifndef CONFIGTAB_H
#define CONFIGTAB_H

#include <QWidget>

class DeviceInfoConfig;
class OptionsFrame;
class CfgResHandler;

class ConfigTab : public QWidget
{
    Q_OBJECT
    void createSubPanel();
public:
    explicit ConfigTab(CfgResHandler* hdl, QWidget *parent = 0);
    ~ConfigTab();

signals:

public slots:

public:
   DeviceInfoConfig* m_deviceInfoWidget;
   OptionsFrame* m_optionsWidget;
   CfgResHandler* m_hdl;
};


#endif // CONFIGTAB_H
