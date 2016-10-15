#include "optionsframe.h"
#include "ui_optionsframe.h"
#include "cfg/cfgreshandler.h"

OptionsFrame::OptionsFrame(CfgMotorBootCfgModel* cfg, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::OptionsFrame),
    m_cfg(cfg)
{
    ui->setupUi(this);

    ui->duration_comboBox->setCurrentIndex(3);
    ui->startdelay_comboBox->setCurrentIndex(2);
    ui->stopdelay_comboBox->setCurrentIndex(2);

    connect(ui->apply_button, &QPushButton::clicked, [this](){
        m_cfg->set_boot_delay(ui->startdelay_comboBox->currentText().toInt());
        m_cfg->set_boot_rape(ui->startdelay_comboBox->currentText().toInt());
        m_cfg->set_duration(ui->duration_comboBox->currentText().toInt());
    });
}

OptionsFrame::~OptionsFrame()
{
    delete ui;
}
