#include "configtab.h"
#include "testinfoconfig.h"

#include <QHBoxLayout>

ConfigTab::ConfigTab(QWidget *parent) : QWidget(parent)
{
    createSubPanel();
    QHBoxLayout* layout = new QHBoxLayout();
    layout->addWidget(deviceInfoWidget);
    setLayout(layout);
}

ConfigTab::~ConfigTab()
{
    //delete
}

void ConfigTab::createSubPanel()
{
    // create communication parameters
    deviceInfoWidget = new DeviceInfoConfig();
}
