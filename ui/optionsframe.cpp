#include "optionsframe.h"
#include "ui_optionsframe.h"
#include "cfg/cfgreshandler.h"
#include "actionwidget.h"

OptionsFrame::OptionsFrame(CfgMotorBootCfgModel* cfg, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::OptionsFrame),
    m_cfg(cfg)
{
    ui->setupUi(this);

    //CfgResHandlerInf* res = ActionWidget::getCfgResHdl();

    ui->startdelay_comboBox->setCurrentIndex(0);
    ui->PRP_comboBox->setCurrentIndex(0);
    ui->softdelay_comboBox->setCurrentIndex(0);

    connect(ui->apply_button, &QPushButton::clicked, [this](){
        m_cfg->set_boot_delay(ui->startdelay_comboBox->currentText().toInt());
        m_cfg->set_boot_rape(ui->softdelay_comboBox->currentText().toInt());
        m_cfg->set_PRP(ui->PRP_comboBox->currentText().toInt());
    });
}

OptionsFrame::~OptionsFrame()
{
    delete ui;
}
