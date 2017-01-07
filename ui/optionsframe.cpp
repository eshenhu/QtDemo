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

    int idx = ui->startdelay_comboBox->findText(QString::number(m_cfg->boot_delay()));
    idx = (idx == -1 ? 0 : idx);
    ui->startdelay_comboBox->setCurrentIndex(idx);

    idx = ui->PRP_comboBox->findText(QString::number(m_cfg->PRP()));
    idx = (idx == -1 ? 0 : idx);
    ui->PRP_comboBox->setCurrentIndex(idx);

    idx = ui->softdelay_comboBox->findText(QString::number(m_cfg->boot_rape()));
    idx = (idx == -1 ? 0 : idx);
    ui->softdelay_comboBox->setCurrentIndex(idx);

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
