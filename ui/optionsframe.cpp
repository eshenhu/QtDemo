#include "optionsframe.h"
#include "ui_optionsframe.h"

OptionsFrame::OptionsFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::OptionsFrame)
{
    ui->setupUi(this);

    ui->duration_comboBox->setCurrentIndex(3);
    ui->startdelay_comboBox->setCurrentIndex(2);
    ui->stopdelay_comboBox->setCurrentIndex(2);

    connect(ui->apply_button, &QPushButton::clicked, [this](){
        m_setting.duration = ui->duration_comboBox->currentText().toInt();
        m_setting.softDelay = ui->startdelay_comboBox->currentText().toInt();
        m_setting.startDelay = ui->startdelay_comboBox->currentText().toInt();
    });
}

OptionsFrame::~OptionsFrame()
{
    delete ui;
}

OptionsFrame::Settings OptionsFrame::setting() const
{
    return m_setting;
}
