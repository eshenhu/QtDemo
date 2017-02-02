#include "configtab.h"
#include "optionsframe.h"
#include "testinfoconfig.h"
#include "cfg/cfgreshandler.h"
#include "productversion.h"
#include <QVBoxLayout>

ConfigTab::ConfigTab(CfgResHandler* hdl, QWidget *parent) : QWidget(parent),
    m_hdl(hdl)
{
    createSubPanel();
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(m_deviceInfoWidget);
    layout->addWidget(m_optionsWidget);
    layout->addStretch(1);
    layout->addWidget(m_versionWidget);
    setLayout(layout);
}

ConfigTab::~ConfigTab()
{
    //delete
}

void ConfigTab::createSubPanel()
{
    // create communication parameters
    m_deviceInfoWidget = new DeviceInfoConfig(m_hdl->deviceCfg());
    m_optionsWidget = new OptionsFrame(m_hdl->bootCfg());
    m_versionWidget = new ProductVersion(m_hdl->deviceCfg());
}
