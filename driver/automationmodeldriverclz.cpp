#include "automationmodeldriverclz.h"

#include <QtDebug>
#include "comm/qmodbusdevice.h"
#include "comm/qmodbusdataunit.h"
#include "comm/qmodbusclient.h"
#include "comm/qmodbusrtuserialmaster.h"

AutomationModelDriverClz::AutomationModelDriverClz(QObject *parent) : BasedModelDriverClz(parent)
{
    state = State::InitState;
    m_sendTimer.setSingleShot(true);
    QObject::connect(&m_sendTimer, &QTimer::timeout, this, [this]() { processSendTimeout(); });

    setupModbusDevice();
}

AutomationModelDriverClz::~AutomationModelDriverClz()
{
    if (modbusDevice)
        modbusDevice->disconnectDevice();
    delete modbusDevice;
}

void AutomationModelDriverClz::setupModbusDevice()
{
    if (modbusDevice) {
        modbusDevice->disconnectDevice();
        delete modbusDevice;
        modbusDevice = nullptr;
    }

    modbusDevice = new QModbus2RtuSerialMaster(this);

    QObject::connect(modbusDevice, &QModbus2Client::errorOccurred, [this](QModbus2Device::Error) {
        emit statusBarChanged(modbusDevice->errorString(), 5000);
        qWarning() << "comm: Error occurred: " << modbusDevice->errorString();
    });

    if (!modbusDevice) {
        emit statusBarChanged(tr("Could not create Modbus master."), 5000);
        qWarning() << "comm: Could not create Modbus master.";
    } else {
        QObject::connect(modbusDevice, &QModbus2Client::stateChanged,
                [this](QModbus2Device::State state){
            if (state == QModbus2Device::UnconnectedState)
            {
                emit stateChanged(Disconnected);
                emit statusBarChanged(tr("comm: State changed : DisConnect"), 5000);
                qInfo() << "comm: State changed : DisConnect.";
            }
            else if (state == QModbus2Device::ConnectedState)
            {
                emit stateChanged(Connected);
                emit statusBarChanged(tr("comm: State changed : Connect"), 5000);
                qInfo() << "comm: State changed : Connect.";
            }
        });
    }
}

void AutomationModelDriverClz::startMeasTest(bool start)
{
    if (!modbusDevice)
        return;

    emit statusBarChanged(tr("comm: connection to server side"), 5000);
    if (modbusDevice->state() != QModbus2Device::ConnectedState) {
        modbusDevice->setConnectionParameter(QModbus2Device::SerialPortNameParameter,
                                             m_settingDialog.name);
        modbusDevice->setConnectionParameter(QModbus2Device::SerialParityParameter,
                                             m_settingDialog.parity);
        modbusDevice->setConnectionParameter(QModbus2Device::SerialBaudRateParameter,
                                             m_settingDialog.baud);
        modbusDevice->setConnectionParameter(QModbus2Device::SerialDataBitsParameter,
                                             m_settingDialog.dataBits);
        modbusDevice->setConnectionParameter(QModbus2Device::SerialStopBitsParameter,
                                             m_settingDialog.stopBits);
        modbusDevice->setTimeout(m_settingDialog.responseTime);
        modbusDevice->setNumberOfRetries(m_settingDialog.numberOfRetries);

        if (!modbusDevice->connectDevice()) {
            emit statusBarChanged(tr("comm: Connect failed: ") + modbusDevice->errorString(), 5000);
            qInfo() << "comm: Connect failed: " <<  modbusDevice->errorString();
        } else {
            emit statusBarChanged(tr("comm: Connect Success: "), 5000);
            qInfo()<< "comm: Connect Success: ";
            processDataHandlerSingleShot();
        }
    }
    else {
        modbusDevice->disconnectDevice();
        emit stateChanged(Disconnected);
    }
}

void AutomationModelDriverClz::readReady()
{
    auto reply = qobject_cast<QModbus2Reply *>(sender());
    if (!reply)
        return;

    if (reply->error() == QModbus2Device::NoError) {
        const QModbus2DataUnit unit = reply->result();
        processReceivedDataUnit(unit);
    }
    else if (reply->error() == QModbus2Device::ProtocolError) {
        QString str(tr("comm: Read response error: %1").
                    arg(reply->errorString()));

        emit statusBarChanged(str, 5000);
        qInfo() << str;
    } else {
        QString str(tr("Read response error: %1 (code: 0x%2)").
                    arg(reply->errorString()).
                    arg(reply->error(), -1, 16));
        emit statusBarChanged(str, 5000);
        qInfo() << str;
    }
    reply->deleteLater();
}

void AutomationModelDriverClz::processReceivedDataUnit(const QModbus2DataUnit &data)
{
    int type = static_cast<int>(data.registerType());
    switch(type) {
    case QModbus2DataUnit::RegisterType::ResetCode:
    {
        if(data.uvalues().r.p.status == static_cast<quint8>(QModbus2DataUnit::ResetRecStatus::BUSY))
        {
            QTimer::singleShot(msecTimeInterval, [this](){ processDataHandlerSingleShot(); });
        }
        else
        {
            state = State::HandShakeState;
            QTimer::singleShot(msecTimeInterval, [this](){ processDataHandlerSingleShot(); });
        }
    }
        break;

    case QModbus2DataUnit::RegisterType::HandShakeCode:
    {
        state = State::FreqAdjustState;
        QTimer::singleShot(msecTimeInterval, [this](){ processDataHandlerSingleShot(); });
    }
        break;

    case QModbus2DataUnit::RegisterType::FreqAdjustCode:
    {
        state = State::FreqAdjustState;
        QTimer::singleShot(msecTimeInterval, [this](){ processDataHandlerSingleShot(); });
    }
        break;

    case QModbus2DataUnit::RegisterType::StartBtnCode:
    {
        state = State::AlarmQueryState;
        QTimer::singleShot(msecTimeInterval, [this](){ processDataHandlerSingleShot(); });
    }
        break;

    case QModbus2DataUnit::RegisterType::AlarmInfoCode:
    {
        state = State::MeasOptionsState;
        QTimer::singleShot(msecTimeInterval, [this](){ processDataHandlerSingleShot(); });
    }
        break;

    case QModbus2DataUnit::RegisterType::MeasConfigCode:
    {
        //state = State::AlarmQueryState;
        QTimer::singleShot(msecTimeInterval, [this](){ processDataHandlerSingleShot(); });
    }
        break;

    default:
        break;
    }
}

void AutomationModelDriverClz::processSendTimeout()
{

}

void AutomationModelDriverClz::processDataHandlerSingleShot()
{
    if (!modbusDevice)
        return;

    switch (state)
    {
    case State::InitState:
    {
        sendResetCmd();
    }
        break;

    case State::ResetState:
    {
        sendHandShakeCmd();
    }
        break;

    case State::HandShakeState:
    {
        sendFreqAdjustCmd();

    }
        break;

    case State::FreqAdjustState:
    {
        sendStartBtnQueryCmd();
    }
        break;

    case State::StartBtnQueryState:
    {
        sendAlarmQueryCmd();
    }
        break;


    case State::AlarmQueryState:
    {
        sendMeasStartOptionsCmd();
    }
        break;

    case State::MeasOptionsState:
    {
        sendMeasStartCmd();
    }
        break;

    case State::MeasRunningState:
    {
        sendMeasStopCmd();
    }
        break;

    case State::MeasStopState:
    {
    }
        break;

    default:
        break;
    }
}


void AutomationModelDriverClz::sendRequestCmd(QModbus2DataUnit writeUnit)
{
    if (!modbusDevice)
        return;
    //statusBar()->clearMessage();

    QModbus2DataUnit::RegisterType table = writeUnit.registerType();

    if (auto *reply = modbusDevice->sendReadRequest(writeUnit, fixedServerAddress)) {
        QObject::connect(reply, &QModbus2Reply::finished, this,
                         &AutomationModelDriverClz::readReady);
    } else {
        const QString str(tr("comm: Write error: ") + modbusDevice->errorString());
        emit statusBarChanged(str, 5000);
        qInfo() << str;
    }
}

void AutomationModelDriverClz::sendResetCmd()
{
    QModbus2DataUnit data(QModbus2DataUnit::ResetCode);
    sendRequestCmd(data);
}

void AutomationModelDriverClz::sendHandShakeCmd()
{
    QModbus2DataUnit::HandShakeStruct v;
    v.randomNumHigh = 0x55;
    v.randomNumLow = 0xAA;

    QModbus2DataUnit data(QModbus2DataUnit::HandShakeCode, v);
    sendRequestCmd(data);
}

void AutomationModelDriverClz::sendFreqAdjustCmd()
{
    QModbus2DataUnit data(QModbus2DataUnit::FreqAdjustCode);
    sendRequestCmd(data);
}

void AutomationModelDriverClz::sendStartBtnQueryCmd()
{
    QModbus2DataUnit data(QModbus2DataUnit::StartBtnCode);
    sendRequestCmd(data);
}

void AutomationModelDriverClz::sendAlarmQueryCmd()
{
    QModbus2DataUnit data(QModbus2DataUnit::AlarmInfoCode);
    sendRequestCmd(data);
}

void AutomationModelDriverClz::sendMeasStartOptionsCmd()
{
    QModbus2DataUnit::MeasConfigStruct v;
    v.measOptions = static_cast<quint8>(QModbus2DataUnit::MeasConfigRecStatus::VOL);
    v.throStart = 90;
    v.volStart = 10;
    v.volEnd = 20;
    v.volStep = 2;
    QModbus2DataUnit data(QModbus2DataUnit::MeasConfigCode, v);
    sendRequestCmd(data);
}

void AutomationModelDriverClz::sendMeasStartCmd()
{
    QModbus2DataUnit data(QModbus2DataUnit::MeasStartCode);
    sendRequestCmd(data);
}

void AutomationModelDriverClz::sendMeasStopCmd()
{
    QModbus2DataUnit data(QModbus2DataUnit::MeasEndCode);
    sendRequestCmd(data);
}

