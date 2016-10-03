#include "testinfoconfig.h"
#include "ui_testinfoconfig.h"

DeviceInfoConfig::DeviceInfoConfig(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::DeviceInfoConfig)
{
    ui->setupUi(this);

//    ui->parityCombo->setCurrentIndex(1);
//    ui->baudCombo->setCurrentText(QString::number(m_settings.baud));
//    ui->dataBitsCombo->setCurrentText(QString::number(m_settings.dataBits));
//    ui->stopBitsCombo->setCurrentText(QString::number(m_settings.stopBits));
//    ui->timeoutSpinner->setValue(m_settings.responseTime);
//    ui->retriesSpinner->setValue(m_settings.numberOfRetries);

//    connect(ui->applyButton, &QPushButton::clicked, [this]() {
//        m_settings.parity = ui->parityCombo->currentIndex();
//        if (m_settings.parity > 0)
//            m_settings.parity++;
//        m_settings.baud = ui->baudCombo->currentText().toInt();
//        m_settings.dataBits = ui->dataBitsCombo->currentText().toInt();
//        m_settings.stopBits = ui->stopBitsCombo->currentText().toInt();
//        m_settings.responseTime = ui->timeoutSpinner->value();
//        m_settings.numberOfRetries = ui->retriesSpinner->value();

//        hide();
//    });
    //connect(ui->ESCFreq_combox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateFreqChanged(int)));
    connect(ui->ESCFreq_combox,
            static_cast<void (QComboBox::*) (int)>(&QComboBox::currentIndexChanged),
            [this](int index){
        if(index == 0)
            m_setting.freq = DeviceInfoConfig::Freq::B50HZ;
        else
            m_setting.freq = DeviceInfoConfig::Freq::B400HZ;
    });

    connect(ui->propVanes_combox,
            static_cast<void (QComboBox::*) (int)>(&QComboBox::currentIndexChanged),
            [this](int index){
        m_setting.propIndex = static_cast<DeviceInfoConfig::Vanes>(index);
    });
}

DeviceInfoConfig::~DeviceInfoConfig()
{
    delete ui;
}
