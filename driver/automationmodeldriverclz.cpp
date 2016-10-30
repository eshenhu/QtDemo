#include "automationmodeldriverclz.h"

#include <QtDebug>
#include "comm/qmodbusdevice.h"
#include "comm/qmodbusdataunit.h"
#include "comm/qmodbusclient.h"
#include "comm/qmodbusrtuserialmaster.h"

#include "driver/measdataformat.h"
#include "cfg/cfgreshandlerinf.h"

AutomationModelDriverClz::AutomationModelDriverClz(QObject *parent) : BasedModelDriverClz(parent)
{
    mp_data = new MeasDataFormat();
    state = State::InitState;
    m_sendTimer.setSingleShot(true);
    QObject::connect(&m_sendTimer, &QTimer::timeout, this, [this]() { processSendTimeout(); });

    setupModbusDevice();
}

AutomationModelDriverClz::~AutomationModelDriverClz()
{
    if (modbusDevice)
    {
        modbusDevice->disconnectDevice();
        delete modbusDevice;
    }

    if (mp_data)
        delete mp_data;
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
                emit stateChanged(Disconnected, "Disconnected");
                emit statusBarChanged(tr("comm: State changed : DisConnect"), 5000);
                qInfo() << "comm: State changed : DisConnect.";
            }
            else if (state == QModbus2Device::ConnectedState)
            {
                emit stateChanged(Connected, "Connected");
                emit statusBarChanged(tr("comm: State changed : Connect"), 5000);
                qInfo() << "comm: State changed : Connect.";
            }
        });
    }
}

void AutomationModelDriverClz::startMeasTestSlot(bool checked)
{
    SettingsDialog::Settings setting;
    setting.name = QStringLiteral("/dev/tty.usbserial");
    startMeasTest(setting);
}

void AutomationModelDriverClz::startMeasTest(const SettingsDialog::Settings setting)
{
    if (!modbusDevice)
        return;

    emit statusBarChanged(tr("comm: connection to server side"), 5000);
    if (modbusDevice->state() != QModbus2Device::ConnectedState) {
        modbusDevice->setConnectionParameter(QModbus2Device::SerialPortNameParameter,
                                             setting.name);
        modbusDevice->setConnectionParameter(QModbus2Device::SerialParityParameter,
                                             setting.parity);
        modbusDevice->setConnectionParameter(QModbus2Device::SerialBaudRateParameter,
                                             setting.baud);
        modbusDevice->setConnectionParameter(QModbus2Device::SerialDataBitsParameter,
                                             setting.dataBits);
        modbusDevice->setConnectionParameter(QModbus2Device::SerialStopBitsParameter,
                                             setting.stopBits);
        modbusDevice->setTimeout(setting.responseTime);
        modbusDevice->setNumberOfRetries(setting.numberOfRetries);

        if (!modbusDevice->connectDevice()) {
            emit statusBarChanged(tr("comm: Connect failed: ") + modbusDevice->errorString(), 5000);
            qInfo() << "comm: Connect failed: " <<  modbusDevice->errorString();
        } else {
            emit statusBarChanged(tr("comm: Connect Success: "), 5000);
            qInfo()<< "comm: Connect Success: ";
            SignalOverLine signal(SignalTypeUserInfoE::START);
            processDataHandlerSingleShot(signal);
        }
    }
    else {
        modbusDevice->disconnectDevice();
        emit stateChanged(Disconnected, "Disconnected");
    }
}

void AutomationModelDriverClz::readReady()
{
    auto reply = qobject_cast<QModbus2Reply *>(sender());
    if (!reply)
        return;

    if (reply->error() == QModbus2Device::NoError) {
        const QModbus2DataUnit unit = reply->result();
        emit update(&unit);
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
    SignalOverLine signal(&data);
    processDataHandlerSingleShot(signal);
}

bool AutomationModelDriverClz::processReceivedHandShakeDataUnit(const QModbus2DataUnit* data)
{
    qWarning() << "it should be got improved here -- eshenhu";
    if (data->uvalues().r.q.productRev != static_cast<quint8>(mp_cfgRes->prod_version()))
    {
        qWarning() << "com.comm.state --HandShake-- received product version " << data->uvalues().r.q.productRev
                   << "was not matched with the software installed " << static_cast<quint8>(mp_cfgRes->prod_version());
        emit stateChanged(HandShakeException, QString::number(data->uvalues().r.q.productRev));
        return false;
    }

    return true;
}

/*
 * Here there are two tasks:
 * 1. enqueue dataunit to the front GUI.
 * 2. persistent the dataunit for recording data.
*/
bool AutomationModelDriverClz::processReceivedMeasDataUnit(const QModbus2DataUnit * const data)
{
    return true;
}

void AutomationModelDriverClz::processSendTimeout()
{

}

void AutomationModelDriverClz::processDataHandlerSingleShot(const SignalOverLine& signal)
{
    if (!modbusDevice)
        return;

    switch (state)
    {
    case State::InitState:
    {
        if (signal.m_type == SignalType::USER && signal.m_info.userType == SignalTypeUserInfoE::START)
        {
            sendHandShakeCmd();
            state = State::HandShakeState;
        }
        else
        {
            qWarning() << "unexpected signal was received during state --State::InitState-- with signal name "
                       << (quint32)(signal.m_type);
        }
    }
        break;

    case State::ResetState:
    {
        if(signal.m_type == SignalType::ECHO
                && signal.m_info.mp_dataUnit->registerType() == QModbus2DataUnit::RegisterType::ResetCode)
        {
            if (signal.m_info.mp_dataUnit->uvalues().r.p.status == static_cast<quint8>(QModbus2DataUnit::ResetRecStatus::BUSY))
            {
                QTimer::singleShot(msecTimeInterval, [this](){ sendResetCmd();});
            }
            else
            {
                state = State::HandShakeState;
            }
        }
        else
        {
            qWarning() << "unexpected signal was received during state -- State::ResetState"
                       << "  with signal name " << static_cast<quint32>(signal.m_type);
        }
    }
        break;

    case State::HandShakeState:
    {
        if(signal.m_type == SignalType::ECHO
                && signal.m_info.mp_dataUnit->registerType() == QModbus2DataUnit::RegisterType::HandShakeCode)
        {
            if (processReceivedHandShakeDataUnit(signal.m_info.mp_dataUnit))
            {
                qInfo() << "com.comm.state changed from State::" << (quint32)State::HandShakeState
                        << "to State" << (int)State::FreqAdjustState;
                sendFreqAdjustCmd();
                state = State::FreqAdjustState;
            }
            else
            {
                qWarning() << "unexpected signal was received during state " << (quint32)state
                           << "  with signal name " << (quint32)signal.m_type;
            }
        }
        else
        {
            qWarning() << "unexpected signal was received during state " << (int)state
                       << "  with signal name " << (quint32)signal.m_type;
        }
    }
        break;

    case State::FreqAdjustState:
    {
        if(signal.m_type == SignalType::ECHO
                && signal.m_info.mp_dataUnit->registerType() == QModbus2DataUnit::RegisterType::FreqAdjustCode)
        {
            if (signal.m_info.mp_dataUnit->uvalues().r.r.status == static_cast<quint8>(QModbus2DataUnit::FreqAdjustRecStatus::WAITING))
            {
                qInfo() << "com.comm.state signal received during State " << (quint32)state
                        << "status == WAITTING";
                QTimer::singleShot(msecTimeInterval, [this](){ sendFreqAdjustCmd();});
            }
            else
            {
                qInfo() << "com.comm.state changed from State::" << (quint32)State::FreqAdjustState
                        << "to State" << (quint32)State::AlarmQueryState;
                sendAlarmQueryCmd();
                state = State::AlarmQueryState;
            }
        }
        else
        {
            qWarning() << "unexpected signal was received during state " << (quint32)state
                       << "  with signal name " << (quint32)signal.m_type;
        }

    }
        break;

    case State::AlarmQueryState:
    {
        if(signal.m_type == SignalType::ECHO
                && signal.m_info.mp_dataUnit->registerType() == QModbus2DataUnit::RegisterType::QueryAlarmInfoCode)
        {
            if (signal.m_info.mp_dataUnit->uvalues().r.t.status == static_cast<quint8>(QModbus2DataUnit::WarningRecStatus::WarningNotFinished))
            {
                qInfo() << "com.comm.state signal received during State " << (quint32)state
                        << "status == WarningNotFinished";
                QTimer::singleShot(msecTimeInterval, [this](){ sendAlarmQueryCmd();});
            }
            else
            {
                qInfo() << "com.comm.state changed from State::" << (quint32)state
                        << "to State" << (qint32)State::MeasRunningState;
                mp_refresh->update();
                sendMeasStartCmd();
                state = State::MeasRunningState;
            }
        }
        else
        {
            qWarning() << "unexpected signal was received during state " << (quint32)state
                       << "  with signal name " << (quint32)signal.m_type;
        }
    }
        break;

    case State::MeasRunningState:
    {
        if(signal.m_type == SignalType::ECHO
                && signal.m_info.mp_dataUnit->registerType() == QModbus2DataUnit::RegisterType::MeasStartCode)
        {

            if (processReceivedMeasDataUnit(signal.m_info.mp_dataUnit))
            {
            }
            QTimer::singleShot(msecTimeInterval, [this](){
                if (mp_refresh->update())
                {
                    qInfo() << "com.comm.state changed from State::" << (quint32)state
                            << "to State - State::MeasFinishedState";
                    state = State::MeasFinishedState;
                    QTimer::singleShot(msecTimeInterval, [this](){
                        SignalOverLine signal(SignalTypeTMOInfoE::TMO500MS);
                        processDataHandlerSingleShot(signal);
                    });
                }
                else
                {
                    sendMeasStartCmd();
                }
            });
        }
    }
        break;

    case State::MeasFinishedState:
    {
    }
        break;

    default:
        break;
    }
}


void AutomationModelDriverClz::sendRequestCmd(const QModbus2DataUnit& writeUnit)
{
    if (!modbusDevice)
        return;
    //statusBar()->clearMessage();

    //QModbus2DataUnit::RegisterType table = writeUnit.registerType();

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
    v.randomNum_1 = 0x55AA55AA;
    v.randomNum_1 = 0x55AA55AA;
    v.randomNum_1 = 0x55AA55AA;
    v.randomNum_1 = 0x55AA55AA;

    QModbus2DataUnit data(QModbus2DataUnit::HandShakeCode, v);
    sendRequestCmd(data);
}

void AutomationModelDriverClz::sendFreqAdjustCmd()
{
    QModbus2DataUnit::FreqAdjustStruct v;
    v.freqValue_1 = mp_cfgRes->HZ();
    v.freqValue_2 = mp_cfgRes->HZ();
    QModbus2DataUnit data(QModbus2DataUnit::FreqAdjustCode, v);
    sendRequestCmd(data);
}

void AutomationModelDriverClz::sendAlarmQueryCmd()
{
    QModbus2DataUnit data(QModbus2DataUnit::AlarmInfoCode);
    sendRequestCmd(data);
}

void AutomationModelDriverClz::sendMeasStartCmd()
{
    QModbus2DataUnit::MeasStartStruct v;
    v.distance = static_cast<quint16>(mp_data->getDis());
    v.thro_1 = static_cast<quint8>(mp_data->getThro_1());
    v.thro_2 = static_cast<quint8>(mp_data->getThro_2());
    v.vol = static_cast<quint16>(mp_data->getVol());
    QModbus2DataUnit data(QModbus2DataUnit::MeasStartCode, v);
    sendRequestCmd(data);
}

//void AutomationModelDriverClz::sendMeasStopCmd()
//{
//    QModbus2DataUnit data(QModbus2DataUnit::MeasEndCode);
//    sendRequestCmd(data);
//}

