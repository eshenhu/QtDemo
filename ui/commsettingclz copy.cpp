#include "commsettingclz.h"
#include <QComboBox>

CommSettingClz::CommSettingClz(QWidget *parent) : QWidget(parent)
{

}

CommSettingClz::~CommSettingClz()
{
}

void CommSettingClz::createWidget()
{
    parityCombo = new QComboBox();
    parityCombo->addItems({"No","Even", "Odd", "Space", "Mark"});
    parityCombo->setCurrentIndex(1);

    baudCombo->setCurrentText(QString::number(commSetting.baud));
    dataBitsCombo->setCurrentText(QString::number(commSetting.dataBits));
    stopBitsCombo->setCurrentText(QString::number(commSetting.stopBits));
    timeoutSpinner->setValue(commSetting.responseTime);
    retriesSpinner->setValue(commSetting.numberOfRetries);

    connect(ui->applyButton, &QPushButton::clicked, [this]() {
        commSetting.parity = ui->parityCombo->currentIndex();
        if (m_settings.parity > 0)
            m_settings.parity++;
        commSetting.baud = ui->baudCombo->currentText().toInt();
        commSetting.dataBits = ui->dataBitsCombo->currentText().toInt();
        commSetting.stopBits = ui->stopBitsCombo->currentText().toInt();
        commSetting.responseTime = ui->timeoutSpinner->value();
        commSetting.numberOfRetries = ui->retriesSpinner->value();

        hide();
    });
}

