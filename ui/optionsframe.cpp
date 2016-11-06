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
    ui->softdelay_comboBox->setCurrentIndex(2);
    ui->bootvol_comboBox->setCurrentIndex(0);

    connect(ui->apply_button, &QPushButton::clicked, [this](){
        m_cfg->set_boot_delay(ui->startdelay_comboBox->currentText().toInt());
        m_cfg->set_boot_rape(ui->softdelay_comboBox->currentText().toInt());
        m_cfg->set_duration(ui->duration_comboBox->currentText().toInt());
        m_cfg->setBootVol(ui->bootvol_comboBox->currentText().toInt());
    });
}

OptionsFrame::~OptionsFrame()
{
    delete ui;
}
