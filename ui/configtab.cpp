#include "configtab.h"
#include "optionsframe.h"
#include "testinfoconfig.h"

#include <QHBoxLayout>

ConfigTab::ConfigTab(QWidget *parent) : QWidget(parent)
{
    createSubPanel();
    QHBoxLayout* layout = new QHBoxLayout();
    layout->addWidget(m_deviceInfoWidget);
    layout->addWidget(m_optionsWidget);
    layout->addStretch(1);
    setLayout(layout);
}

ConfigTab::~ConfigTab()
{
    //delete
}

void ConfigTab::createSubPanel()
{
    // create communication parameters
    m_deviceInfoWidget = new DeviceInfoConfig();
    m_optionsWidget = new OptionsFrame();

}
