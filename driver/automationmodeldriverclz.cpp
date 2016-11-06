#include "automationmodeldriverclz.h"

#include <QtDebug>
#include "comm/qmodbusdevice.h"
#include "comm/qmodbusdataunit.h"
#include "comm/qmodbusclient.h"
#include "comm/qmodbusrtuserialmaster.h"

#include "driver/measdataformat.h"
#include "cfg/cfgreshandler.h"
#include "util/qserialporthelper.h"

AutomationModelDriverClz::AutomationModelDriverClz(QObject *parent) :
    BasedModelDriverClz(parent),
    state(State::InitState),
    mp_data(new MeasDataFormat()),
    mp_cfgRes(new CfgResHandler(this)),
    mp_refresh(nullptr),
    m_monitorError(new FatalErrorDrvClz)
{
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
        qCWarning(DRONE_LOGGING) << "comm: Error occurred: " << modbusDevice->errorString();
    });

    if (!modbusDevice) {
        emit statusBarChanged(tr("Could not create Modbus master."), 5000);
        qCWarning(DRONE_LOGGING) << "comm: Could not create Modbus master.";
    } else {
        QObject::connect(modbusDevice, &QModbus2Client::stateChanged,
                [this](QModbus2Device::State state){
            if (state == QModbus2Device::UnconnectedState)
            {
                emit stateChanged(Disconnected, "Disconnected");
                emit statusBarChanged(tr("comm: State changed : DisConnect"), 5000);
                qCInfo(DRONE_LOGGING) << "comm: State changed : DisConnect.";
            }
            else if (state == QModbus2Device::ConnectedState)
            {
                emit stateChanged(Connected, "Connected");
                emit statusBarChanged(tr("comm: State changed : Connect"), 5000);
                qCInfo(DRONE_LOGGING) << "comm: State changed : Connect.";
            }
        });
    }
}

//QSerialPortSetting::Settings AutomationModelDriverClz::startMeasTestSlot(bool checked)
//{

//    //setting.name = QStringLiteral("/dev/tty.usbserial");
//    //setting.name = QStringLiteral("tty.usbserial-A50285BI");
//    QSerialPortSetting::Settings setting = QSerialPortHelper::getPluggedInPort();

//    if (setting.name.isEmpty())
//    {
//        qCWarning(DRONE_LOGGING) << "driver.automationmodeldriverclz  Failed to find the proper port !";
//        return setting;
//    }
//    startMeasTest(setting);
//    return true;
//}

void AutomationModelDriverClz::resetMeasDataUnit()
{
    // delete the previous meas data struct;
    if (mp_refresh){
        delete mp_refresh;
    }
}


void AutomationModelDriverClz::startMeasTest(const UiCompMeasData data,const CfgResHandlerInf* res, const QSerialPortSetting::Settings setting)
{
    if (mp_refresh)
        delete mp_refresh;
//    PeriodicalVolMeasDataUpdate(const quint32 start, const quint32 end, const quint32 step, const quint32 thro,
//                                const quint32 delay_start, const quint32 soft_delay, const quint32 boot_voltage,
//                                const quint32 durationInSec, const quint32 intervalInMSec = 500);
    if (data.type == JsonGUIPrimType::VOLTAGE){
        mp_refresh = new PeriodicalVolMeasDataUpdate(data.data.u.vol_beg, data.data.u.vol_end, data.data.u.vol_step,
                                                     data.data.u.thro, res->boot_delay(), res->boot_rape(), res->bootVol(),
                                                     data.data.u.duration);
        mp_refresh->setSeed(mp_data);
    }
    else if (data.type == JsonGUIPrimType::THROTTLE){
        mp_refresh = new PeriodicalThroMeasDataUpdate(data.data.v.thro_beg, data.data.v.thro_end, data.data.v.thro_step,
                                                      data.data.v.vol, res->boot_delay(), res->boot_rape(), res->bootVol(),
                                                      data.data.v.duration);
        mp_refresh->setSeed(mp_data);
    }
    else{
        qCWarning(DRONE_LOGGING) << tr("com.engine Sorry, we don't support this selection temporaily ");
        return;
    }

    if (!modbusDevice)
        return;

    emit statusBarChanged(tr("comm: connection to server side"), 5000);
    if (modbusDevice->state() != QModbus2Device::ConnectedState) {
        modbusDevice->setConnectionParameter(QModbus2Device::SerialPortNameParameter,
                                             setting.name);
        modbusDevice->setConnectionParameter(QModbus2Device::SerialParityParameter,
                                             setting.parity);
        modbusDevice->setConnectionParameter(QModbus2Device::SerialBaudRateParameter,
                                             setting.baudRate);
        modbusDevice->setConnectionParameter(QModbus2Device::SerialDataBitsParameter,
                                             setting.dataBits);
        modbusDevice->setConnectionParameter(QModbus2Device::SerialStopBitsParameter,
                                             setting.stopBits);
        modbusDevice->setTimeout(setting.responseTime);
        modbusDevice->setNumberOfRetries(setting.numberOfRetries);

        if (!modbusDevice->connectDevice()) {
            emit statusBarChanged(tr("comm: Connect failed: ") + modbusDevice->errorString(), 5000);
            qCInfo(DRONE_LOGGING) << "comm: Connect failed: " <<  modbusDevice->errorString();
        } else {
            emit statusBarChanged(tr("comm: Connect Success: "), 5000);
            qCInfo(DRONE_LOGGING)<< "comm: Connect Success: ";
            enterFSMInitState();
            SignalOverLine signal(SignalTypeUserInfoE::START);
            processDataHandlerSingleShot(signal);
        }
    }
    else {
        modbusDevice->disconnectDevice();
        emit stateChanged(Disconnected, "Disconnected");
    }
}

void AutomationModelDriverClz::enterFSMInitState()
{
    state = State::InitState;
    qCDebug(DRONE_LOGGING) << "com.automationModeDriver State Changed -> State::Reset";
}

void AutomationModelDriverClz::enterFSMResetState(const QString& str)
{
    state = State::ResetState;
    qCDebug(DRONE_LOGGING) << "com.automationModeDriver State Changed -> State::Reset";
    QTimer::singleShot(msecTimeInterval, [this](){ sendResetCmd();});
    emit stateChanged(QModBusState::FatalErrorException, str);
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
        qCInfo(DRONE_LOGGING) << str;
    } else {
        QString str(tr("Read response error: %1 (code: 0x%2)").
                    arg(reply->errorString()).
                    arg(reply->error(), -1, 16));
        emit statusBarChanged(str, 5000);
        qCInfo(DRONE_LOGGING) << str;
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
    qCWarning(DRONE_LOGGING) << "it should be got improved here -- eshenhu";
    //if (data->uvalues().r.q.productRev != static_cast<quint8>(mp_cfgRes->prod_version()))
    if (0)
    {
        qCWarning(DRONE_LOGGING) << "com.comm.state --HandShake-- received product version " << data->uvalues().r.q.productRev
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

    /*
     *
     *   Handle Exception Case:
     *
     */
    switch (state)
    {
    case State::InitState:
    case State::HandShakeState:
    case State::FreqAdjustState:
    case State::StartBtnQueryState:
    case State::AlarmQueryState:
    case State::MeasOptionsState:
    case State::MeasRunningState:
    case State::MeasFinishedState:
    {
        bool isSendErrorSigNeeded = true;
        QString str;

        if(signal.m_type == SignalType::ECHO
                && signal.m_info.mp_dataUnit->registerType() == QModbus2DataUnit::RegisterType::FatalErrorInfoCode)
        {
            switch (signal.m_info.mp_dataUnit->uvalues().r.u.errorCode)
            {
            case (quint8)QModbus2DataUnit::FatalErrorCodeE::FatalErrorCRC:
            {
                str = tr("Fatal Error : Please check the cable connection, Internal Error with error code .")
                        + QString::number((quint8)QModbus2DataUnit::FatalErrorCodeE::FatalErrorCRC);
                m_monitorError->cntUpCRC();
                if (!m_monitorError->isCRCError())
                    isSendErrorSigNeeded = false;
            }
                break;
            case (quint8)QModbus2DataUnit::FatalErrorCodeE::FatalErrorFrame:
            {
                str = tr("Fatal Error : Internal Error with error code %1. Please check the cable connection ")
                        + QString::number((quint8)QModbus2DataUnit::FatalErrorCodeE::FatalErrorFrame);
                m_monitorError->cntUpFrameError();
                if (!m_monitorError->isFrameError())
                    isSendErrorSigNeeded = false;
            }
                break;
            case (quint8)QModbus2DataUnit::FatalErrorCodeE::FatalErrorEmerBtnDown:
            {
                str = tr("Fatal Error : Emergency Button was Pressed");
            }
                break;
            case (quint8)QModbus2DataUnit::FatalErrorCodeE::FatalErrorCageOpen:
            {
                str = tr("Fatal Error : Cage was opened");
            }
                break;
            case (quint8)QModbus2DataUnit::FatalErrorCodeE::FatalErrorCable2Cnt2Samp:
            {
                str = tr("Fatal Error : Cable between maincontrol and sample was broken");
            }
                break;
            case (quint8)QModbus2DataUnit::FatalErrorCodeE::FatalErrorCable2Cnt2Pwr:
            {
                str = tr("Fatal Error : Cable between maincontrol and power was broken");
            }
                break;
            default:
            {
                str = tr("Unspecified FatalErrorCode was received during Testing... ERRORCODE= %1")
                        .arg(signal.m_info.mp_dataUnit->uvalues().r.u.errorCode);
                qCWarning(DRONE_LOGGING) << "Unspecified FatalErrorCode was received during Testing... ERRORCODE="
                           << signal.m_info.mp_dataUnit->uvalues().r.u.errorCode;
                break;
            }
            }

            if (isSendErrorSigNeeded)
            {
                enterFSMResetState(str);
            }
        }
        break;
    }
    default:
        break;
    }
    /*
     *
     *   Simple FSM engine.
     *
     */
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
            qCWarning(DRONE_LOGGING) << "unexpected signal was received during state --State::InitState-- with signal name "
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
                if (modbusDevice->state() == QModbus2Device::ConnectedState
                   || modbusDevice->state() == QModbus2Device::ConnectingState){
                    modbusDevice->disconnectDevice();
                }
                qCInfo(DRONE_LOGGING) << "com.automationModel Enter into the Idle Mode";
                emit stateChanged(Disconnected, "Disconnected");
            }
        }
        else
        {
            if (modbusDevice->state() == QModbus2Device::ConnectedState
               || modbusDevice->state() == QModbus2Device::ConnectingState){
                modbusDevice->disconnectDevice();
                QTimer::singleShot(msecTimeInterval, [this](){ sendResetCmd();});
            }
            qCWarning(DRONE_LOGGING) << "unexpected signal was received during state -- State::ResetState"
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
                qCInfo(DRONE_LOGGING) << "com.comm.state changed from State::" << (quint32)State::HandShakeState
                        << "to State" << (int)State::FreqAdjustState;
                sendFreqAdjustCmd();
                state = State::FreqAdjustState;
            }
            else
            {
                qCWarning(DRONE_LOGGING) << "unexpected signal was received during state " << (quint32)state
                           << "  with signal name " << (quint32)signal.m_type;
            }
        }
        else
        {
            qCWarning(DRONE_LOGGING) << "unexpected signal was received during state " << (int)state
                       << "  with signal name " << (quint32)signal.m_type;
            enterFSMResetState(tr("Unexpected signal was received!"));
        }
    }
        break;

    case State::FreqAdjustState:
    {
        if(signal.m_type == SignalType::ECHO
                && signal.m_info.mp_dataUnit->registerType() == QModbus2DataUnit::RegisterType::FreqAdjustCode)
        {
//            const QModbus2DataUnit::MeasDataUnion& data = signal.m_info.mp_dataUnit->uvalues();
//            int size = sizeof(QModbus2DataUnit::MeasDataUnion);
//            const char* rawdata = (const char*)&data;
//            QByteArray barray = QByteArray::fromRawData(rawdata, size);
//            qCInfo(DRONE_LOGGING) << "com.comm receive" << barray.toHex();

            if (signal.m_info.mp_dataUnit->uvalues().r.r.status == static_cast<quint8>(QModbus2DataUnit::FreqAdjustRecStatus::WAITING))
            {
                qCInfo(DRONE_LOGGING) << "com.comm.state signal received during State " << (quint32)state
                        << "status == WAITTING";
                QTimer::singleShot(msecTimeInterval, [this](){ sendFreqAdjustCmd();});
            }
            else
            {
                qCInfo(DRONE_LOGGING) << "com.comm.state changed from State::" << (quint32)State::FreqAdjustState
                        << "to State" << (quint32)State::AlarmQueryState;
                sendAlarmQueryCmd();
                state = State::AlarmQueryState;
            }
        }
        else
        {
            qCWarning(DRONE_LOGGING) << "unexpected signal was received during state " << (quint32)state
                       << "  with signal name " << (quint32)signal.m_type;
            enterFSMResetState(tr("Unexpected signal was received!"));
        }
    }
        break;

    case State::AlarmQueryState:
    {
        if(signal.m_type == SignalType::ECHO
                && signal.m_info.mp_dataUnit->registerType() == QModbus2DataUnit::RegisterType::AlarmInfoCode)
        {
            if (signal.m_info.mp_dataUnit->uvalues().r.t.status == static_cast<quint8>(QModbus2DataUnit::WarningRecStatus::WarningNotFinished))
            {
                qCInfo(DRONE_LOGGING) << "com.comm.state signal received during State " << (quint32)state
                        << "status == WarningNotFinished";
                QTimer::singleShot(msecTimeInterval, [this](){ sendAlarmQueryCmd();});
            }
            else
            {
                qCInfo(DRONE_LOGGING) << "com.comm.state changed from State::" << (quint32)state
                        << "to State" << (qint32)State::MeasRunningState;
                mp_refresh->update();
                sendMeasStartCmd();
                state = State::MeasRunningState;
            }
        }
        else
        {
            qCWarning(DRONE_LOGGING) << "unexpected signal was received during state " << (quint32)state
                       << "  with signal name " << (quint32)signal.m_type;
            enterFSMResetState(tr("Unexpected signal was received!"));
        }
    }
        break;

    case State::MeasRunningState:
    {
        if( signal.m_type == SignalType::ECHO &&
                (  signal.m_info.mp_dataUnit->registerType() == QModbus2DataUnit::RegisterType::MeasStartCode
                || signal.m_info.mp_dataUnit->registerType() == QModbus2DataUnit::RegisterType::FatalErrorInfoCode
                ))
        {
            if (processReceivedMeasDataUnit(signal.m_info.mp_dataUnit))
            {
                emit updateData(signal.m_info.mp_dataUnit);
            }
            QTimer::singleShot(msecTimeInterval, [this](){
                if (mp_refresh->update())
                {
                    qCInfo(DRONE_LOGGING) << "com.comm.state changed from State::" << (quint32)state
                            << "to State - State::MeasFinishedState";
                    enterFSMResetState(tr("Measurement Finished!"));
                }
                else
                {
                    sendMeasStartCmd();
                }
            });
        }
        else
        {
            qCWarning(DRONE_LOGGING) << "unexpected signal was received during state " << (quint32)state
                       << "  with signal name " << (quint32)signal.m_type;
            enterFSMResetState(tr("Unexpected signal was received!"));
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

    if (auto *reply = modbusDevice->sendReadRequest(writeUnit, fixedServerAddress)) {
        QObject::connect(reply, &QModbus2Reply::finished, this,
                         &AutomationModelDriverClz::readReady);
    } else {
        const QString str(tr("comm: Write error: ") + modbusDevice->errorString());
        emit statusBarChanged(str, 5000);
        qCInfo(DRONE_LOGGING) << str;
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
    v.randomNum_1 = 0;
    v.randomNum_2 = 0;
    v.randomNum_3 = 0;
    v.randomNum_4 = 0;

    QModbus2DataUnit data(QModbus2DataUnit::HandShakeCode, v);
    sendRequestCmd(data);
}

void AutomationModelDriverClz::sendFreqAdjustCmd()
{
    QModbus2DataUnit::FreqAdjustStruct v;
//    v.freqValue_1 = mp_cfgRes->HZ();
//    v.freqValue_2 = mp_cfgRes->HZ();
    v.freqValue_1 = 0;
    v.freqValue_2 = 1;
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
    v.distance = static_cast<quint32>(mp_data->getDis());
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

Q_LOGGING_CATEGORY(DRONE_LOGGING, "drone.engine", QtDebugMsg)
