#include "testinfoconfig.h"
#include "ui_testinfoconfig.h"

DeviceInfoConfig::DeviceInfoConfig(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::DeviceInfoConfig)
{
    ui->setupUi(this);
}

DeviceInfoConfig::~DeviceInfoConfig()
{
    delete ui;
}
