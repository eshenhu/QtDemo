#include "testinfoconfig.h"
#include "ui_testinfoconfig.h"
#include "cfg/cfgreshandler.h"

DeviceInfoConfig::DeviceInfoConfig(CfgDeviceCfgModel* cfg, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::DeviceInfoConfig),
    m_cfg(cfg)

{
    ui->setupUi(this);

    const QIntValidator* validator = new QIntValidator(0, 100, this);
    ui->Thr_Min_LE->setValidator(validator);
    ui->Thr_Max_LE->setValidator(validator);

    ui->Thr_Min_LE->setText(QString("%1").arg(m_cfg->lowThroLimit()));
    ui->Thr_Max_LE->setText(QString("%1").arg(m_cfg->highThroLimit()));

    int idx = ui->ESCFreq_combox->findText(QString::number(m_cfg->HZ()));
    idx = (idx == -1 ? 0 : idx);
    ui->ESCFreq_combox->setCurrentIndex(idx);

    idx = ui->propVanes_combox->findText(QString::number(m_cfg->vane()));
    idx = (idx == -1 ? 0 : idx);
    ui->propVanes_combox->setCurrentIndex(idx);

    ui->SN_LE->setText(m_cfg->SerialNumber());

//    connect(ui->ESCFreq_combox,
//            static_cast<void (QComboBox::*) (int)>(&QComboBox::currentIndexChanged),
//            [this](int index){
//        if(index == 0)
//            m_cfg->setHZ(DeviceInfoConfig::Freq::B50HZ);
//        else
//            m_cfg->setHZ(DeviceInfoConfig::Freq::B400HZ);
//    });

//    connect(ui->propVanes_combox, static_cast<void (QComboBox::*) (const QString&)>(&QComboBox::currentTextChanged),
//            [this](const QString& text){
//        QString vanes = text;
//        m_cfg->setVane(static_cast<DeviceInfoConfig::Vanes>(vanes.toInt()));
//    });

    connect(ui->Apply, &QPushButton::clicked, [this](){
        m_cfg->setLowThroLimit(ui->Thr_Min_LE->text().toInt());
        m_cfg->setHighThroLimit(ui->Thr_Max_LE->text().toInt());

        if (ui->ESCFreq_combox->currentIndex() == 0)
            m_cfg->setHZ(DeviceInfoConfig::Freq::B50HZ);
        else
            m_cfg->setHZ(DeviceInfoConfig::Freq::B400HZ);

        m_cfg->setVane(ui->propVanes_combox->currentText().toInt());
        //m_cfg->setSerialNumber(ui->SN_LE->text());
    });

}

DeviceInfoConfig::~DeviceInfoConfig()
{
    delete ui;
}
