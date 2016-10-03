#ifndef CONFIGTAB_H
#define CONFIGTAB_H

#include <QWidget>

class DeviceInfoConfig;
class OptionsFrame;

class ConfigTab : public QWidget
{
    Q_OBJECT
    void createSubPanel();
public:
    explicit ConfigTab(QWidget *parent = 0);
    ~ConfigTab();

signals:

public slots:



public:
   DeviceInfoConfig* m_deviceInfoWidget;
   OptionsFrame* m_optionsWidget;

};


#endif // CONFIGTAB_H
