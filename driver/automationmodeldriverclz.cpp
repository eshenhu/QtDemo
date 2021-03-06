#include "automationmodeldriverclz.h"

#include <QtDebug>
#include "comm/qmodbusdevice.h"
#include "comm/qmodbusdataunit.h"
#include "comm/qmodbusclient.h"
#include "comm/qmodbusrtuserialmaster.h"

#include "driver/measdataformat.h"
#include "cfg/cfgreshandler.h"
#include "util/qserialporthelper.h"

#include "cfg/datajsonrecelement.h"
#include "util/utildatarecordingclz.h"
#include "cfg/cfgjsonrecelement.h"
#include "cfg/cfgzerocalibrateclz.h"
#include "cfg/unireslocation.h"
#include "util/aes.h"
#include "util/simplecrypt_helper.h"
#include <climits>
#include <random>
#include <QStandardPaths>
AutomationModelDriverClz::AutomationModelDriverClz(QObject *parent) :
    BasedModelDriverClz(parent),
    state(State::InitState),
    mp_data(new MeasDataFormat()),
    mp_refresh(nullptr),
    m_monitorError(new FatalErrorDrvClz),
    m_randomNum(16, 'f')
{
    mp_cfgRes = UniResLocation::getCfgResHdl();
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
    /*
     *  Reset the Cfg
     */

    CfgZeroCalibrateClz::reset();
    /*
     * Data recording
    */
    UtilDataRecordingClz::getInstance().newRec(data.type);

    qInfo() << "com.automationModeDriver -> UtilDataRecordingClz.getInstance().getCfgName"
               << UtilDataRecordingClz::getInstance().getCfgFileName();
    qInfo() << "com.automationModeDriver -> UtilDataRecordingClz.getInstance().getRecName"
               << UtilDataRecordingClz::getInstance().getRecFileName();

    QModbus2DataUnit::MotorTypeEnum motorType = QModbus2DataUnit::MotorTypeEnum::ELECE;
    if (res->motor_type() == CfgResHandlerInf::MotorType::PELEC)
    {
        motorType = QModbus2DataUnit::MotorTypeEnum::ELECE;
    }
    else if (res->motor_type() == CfgResHandlerInf::MotorType::POIL)
    {
         motorType = QModbus2DataUnit::MotorTypeEnum::OILE;
    }

    CfgJsonRecElement ele = CfgJsonRecElement::CfgJsonRecElementBuilder()
            .manufacture(QStringLiteral("UDAT"))
            .PV(res->prod_version())
            .vanes(res->vane())
            .motorType(motorType)
            .numOfMotor(res->num_of_motor())
            .plans(data.type)
            .timeStamp(UtilDataRecordingClz::getInstance().getTimeStamp())
            .build();

    ele.saveCfg(UtilDataRecordingClz::getInstance().getCfgFileName());

    DataJsonRecElement::DataJsonRecElementFileHelper helper;
    helper.newFile(UtilDataRecordingClz::getInstance().getRecFileName());

    /*
     * create Test Case.
    */
    if (mp_refresh)
        delete mp_refresh;
//    PeriodicalVolMeasDataUpdate(const quint32 start, const quint32 end, const quint32 step, const quint32 thro,
//                                const quint32 delay_start, const quint32 soft_delay, const quint32 boot_voltage,
//                                const quint32 durationInSec, const quint32 intervalInMSec = 500);

    m_uiCfgData = data;
    mp_data->reset();

    if (data.type == TestPlanEnum::Voltage){
        mp_refresh = new PeriodicalVolMeasDataUpdate(data.data.u.vol_beg, data.data.u.vol_end, data.data.u.vol_step, data.data.u.thro,
                                                     res->boot_delay(), res->boot_PRP(), res->boot_rape(), data.data.u.vol_beg,
                                                     data.data.u.duration);
        mp_refresh->setSeed(mp_data);
    }
    else if (data.type == TestPlanEnum::Throttle || data.type == TestPlanEnum::Multiplue){
        mp_refresh = new PeriodicalThroMeasDataUpdate(data.data.v.thro_beg, data.data.v.thro_end, data.data.v.thro_step, data.data.v.vol,
                                                      res->boot_delay(), res->boot_PRP(), res->boot_rape(), data.data.v.vol,
                                                      data.data.v.duration);
        mp_refresh->setSeed(mp_data);
    }
    else if (data.type == TestPlanEnum::Distance){
        mp_refresh = new PeriodicalDisMeasDataUpdate(data.data.w.dis_beg, data.data.w.dis_end, data.data.w.dis_step, data.data.w.vol, data.data.w.thro,
                                                     res->boot_delay(), res->boot_PRP(), res->boot_rape(), data.data.w.vol,
                                                     data.data.w.duration);
        mp_refresh->setSeed(mp_data);
    }
    else if (data.type == TestPlanEnum::Manual){
        mp_refresh = new OneShotManualMeasDataUpdate(data.data.y.vol, data.data.y.thro,
                                                     res->boot_delay(), res->boot_PRP(), res->boot_rape(), data.data.y.vol);
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

void AutomationModelDriverClz::doTest()
{

    qDebug() << "doTest -- " << QStandardPaths::standardLocations(QStandardPaths::GenericConfigLocation);

    TestPlanEnum data = TestPlanEnum::Voltage;
    UtilDataRecordingClz::getInstance().newRec(data);

    qDebug() << "doTest -- " << UtilDataRecordingClz::getInstance().getCfgFileName()
             << UtilDataRecordingClz::getInstance().getRecFileName();
    /*--------------------------------------------------------------*/

    //51 FF 67 06 50 66 55 56 45 57 02 87
    const unsigned char keyRawInput[] = { 0x51, 0xFF, 0x68, 0x06,
                                 0x50, 0x66, 0x55, 0x56,
                                 0x47, 0x28, 0x02, 0x87};

    QByteArray in = QByteArray::fromRawData((const char*)keyRawInput, sizeof(keyRawInput));
    QByteArray out;
    SimpleCrypt_helper::encrypto(in, out);

    QByteArray out_decry_key;
    SimpleCrypt_helper::decrypto(out,out_decry_key);

    qCWarning(DRONE_LOGGING) << "CryperKeyTest: plainText"
                             << out.toHex()
                             << "decrypto"
                             << out_decry_key.toHex();

    /*--------------------------------------------------------------*/
    QByteArray cryptoText(16, ' ');
    generateRandomNumber();
    //QByteArray plainText = getRandomNumber();
    const char rawInput[] = { 0x00, 0x01, 0x02, 0x03,
                              0x04, 0x05, 0x06, 0x07,
                              0x08, 0x09, 0x0A, 0x0B,
                              0x0C, 0x0D, 0x0E, 0x0F};

    QByteArray plainText = QByteArray::fromRawData(rawInput, sizeof(rawInput));
    QModbus2DataUnit::HandShakeStruct v;

    char* rawdata = (char*)&v;

    for (int i = 0; i < 16; i++)
    {
        *(rawdata + i) = m_randomNum.at(i);
    }

    QByteArray keyArray(UniResLocation::getCfgResHdl()->key());

    crypto_aes_ctx ctx;
    crypto_aes_expand_key(&ctx, (const unsigned char*)(keyArray.constData()), AES_KEYSIZE_128);
    aes_encrypt(&ctx, (unsigned char*)cryptoText.data(), (const unsigned char*)plainText.constData());

    qCWarning(DRONE_LOGGING) << "CryperTest: plainText" << plainText.toHex() << "key " << keyArray.toHex() << "encrypt" << cryptoText.toHex();

//    const char* rawDataSrc = (char*)&(data->uvalues().r.q.secrectKeyMain_1);
//    QByteArray recCryptoText(rawDataSrc, AES_KEYSIZE_128);

//    /* compare the crypt one from main board with the calculate one*/
//    if (recCryptoText != cryptoText)
//    {
//        qCWarning(DRONE_LOGGING) << "com.comm.state --HandShake-- received product version " << data->uvalues().r.q.productRev
//                   << "was not matched with the software installed " << static_cast<quint8>(mp_cfgRes->prod_version());
//        return false;
//    }

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

void AutomationModelDriverClz::syncDataDuringManualTest(const double vol, const quint32 thro)
{
    if (mp_refresh != nullptr && m_uiCfgData.type != TestPlanEnum::Invaild)
    {
        if (state == State::MeasRunningState)
        {
            if (m_uiCfgData.type == TestPlanEnum::Manual)
            {
                OneShotManualMeasDataUpdate* refresh = dynamic_cast<OneShotManualMeasDataUpdate*>(mp_refresh);
                refresh->updateValueManually(vol, thro);
            }
            else
            {
                qCWarning(DRONE_LOGGING) << "Can't send manual sync data to non-manual test plan";
            }
        }
        else
        {
            qCInfo(DRONE_LOGGING) << "Can't change the real time value due to FSM state was not in RUNNING state";
        }
    }
    else
    {
        qCWarning(DRONE_LOGGING) << "Can't send manual sync data to non-ongoing test!";
    }
}

void AutomationModelDriverClz::processReceivedDataUnit(const QModbus2DataUnit &data)
{
    SignalOverLine signal(&data);
    processDataHandlerSingleShot(signal);
}

bool AutomationModelDriverClz::processReceivedHandShakeDataUnit(const QModbus2DataUnit* data)
{
    //qCWarning(DRONE_LOGGING) << "it should be got improved here -- eshenhu";
    //if (data->uvalues().r.q.productRev != static_cast<quint8>(mp_cfgRes->prod_version()))
    return true;

    QByteArray cryptoText(16, ' ');

    QByteArray plainText = getRandomNumber();

    QByteArray keyArray(UniResLocation::getCfgResHdl()->key());

    crypto_aes_ctx ctx;
    crypto_aes_expand_key(&ctx, (const unsigned char*)(keyArray.constData()), AES_KEYSIZE_128);
    aes_encrypt(&ctx, (unsigned char*)cryptoText.data(), (const unsigned char*)plainText.constData());

    const char* rawDataSrc = (char*)&(data->uvalues().r.q.secrectKeyMain_1);
    QByteArray recCryptoText(rawDataSrc, AES_KEYSIZE_128);

    /* compare the crypt one from main board with the calculate one*/
    if (recCryptoText != cryptoText)
    {
        qCWarning(DRONE_LOGGING) << "CryperTest: send Text on wire" << m_randomNum.toHex()
                                 << "in use text" << plainText.toHex()
                                 << "key " << keyArray.toHex()
                                 << "encrypt" << cryptoText.toHex()
                                 << "rec" << recCryptoText.toHex();

        qCWarning(DRONE_LOGGING) << "com.comm.state --HandShake-- received product version " << data->uvalues().r.q.productRev
                   << "was not matched with the software installed " << static_cast<quint8>(mp_cfgRes->prod_version());
        return false;
    }

    return true;
}

QModbus2DataUnit::LimitStatusEnum AutomationModelDriverClz::processReceivedMeasDistanceDataUnit(const QModbus2DataUnit *data)
{
    QModbus2DataUnit::LimitStatusEnum rtn = QModbus2DataUnit::LimitStatusEnum::REACHED;

    if (data->uvalues().r.s.motorType == static_cast<quint8>(QModbus2DataUnit::MotorTypeEnum::ELECE))
    {
        const quint8 value = data->uvalues().r.s.motorInfo.elec.limitStatus;
        if (value == (quint8)QModbus2DataUnit::LimitStatusHEXEnum::HEXINVALID)
            rtn = QModbus2DataUnit::LimitStatusEnum::INVALID;
        else if (value & (quint8)QModbus2DataUnit::LimitStatusHEXEnum::HEXZEROPOS)
            rtn = QModbus2DataUnit::LimitStatusEnum::ZEROPOS;
        else if (value & (quint8)QModbus2DataUnit::LimitStatusHEXEnum::HEXREACHED)
            rtn = QModbus2DataUnit::LimitStatusEnum::REACHED;
        else if (value & (quint8)QModbus2DataUnit::LimitStatusHEXEnum::HEXRUNNING)
            rtn = QModbus2DataUnit::LimitStatusEnum::RUNNING;
        else if (value & (quint8)QModbus2DataUnit::LimitStatusHEXEnum::HEXUPLIMIT)
            rtn = QModbus2DataUnit::LimitStatusEnum::UPLIMIT;
        else if (value & (quint8)QModbus2DataUnit::LimitStatusHEXEnum::HEXDOWNLIMIT)
            rtn = QModbus2DataUnit::LimitStatusEnum::DOWNLIMIT;
        else
            rtn = QModbus2DataUnit::LimitStatusEnum::NOTARR;
    }
    else
    {
        qCWarning(DRONE_LOGGING) << "com.comm.state -- MeasDistance-- other than ELECE dont support distance test";
    }

    DataJsonRecElement& e2 = DataJsonRecElement::DataJsonRecElementGetHelper().getElem(true);
    e2.setPosStatus(static_cast<quint32>(rtn));

    return rtn;
}

/*
 * Here there are two tasks:
 * 1. enqueue dataunit to the front GUI.
 * 2. persistent the dataunit for recording data.
*/
bool AutomationModelDriverClz::processReceivedMeasDataUnit(const QModbus2DataUnit * const data)
{
    Q_UNUSED(data)
    return true;
}

void AutomationModelDriverClz::doLaterReceivedMeasDataUnit(const QModbus2DataUnit * const data)
{
    Q_UNUSED(data)
}

void AutomationModelDriverClz::generateRandomNumber()
{
    std::random_device dev;

    // choose a random number between 0 and 0xFFFFFFFF;
    std::default_random_engine e1(dev());

    std::uniform_int_distribution<long long> uniform_dist(LLONG_MIN, LLONG_MAX);

    long long mean = uniform_dist(e1);
    m_randomNum = QByteArray::number(mean, 16);
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
    case State::DistanceStepIntoLowState:
    case State::DistanceStepIntoBegState:
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

                DataJsonRecElement::DataJsonRecElementFileHelper helper;
                helper.closeFile(UtilDataRecordingClz::getInstance().getRecFileName());

                state = State::MeasFinishedState;
                emit stateChanged(Disconnected, "Disconnected");
            }
        }
        else
        {
            if (modbusDevice->state() == QModbus2Device::ConnectedState
               || modbusDevice->state() == QModbus2Device::ConnectingState){
//                modbusDevice->disconnectDevice();
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

//                emit stateChanged(HandShakeException,
//                                  QString::number(signal.m_info.mp_dataUnit->uvalues().r.q.productRev));
                enterFSMResetState(tr("HandShake Failure!"));
//                state = State::MeasFinishedState;
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
                // What's a fuck implemention on this 'DISTANCE' type! fuck me!
                // A better way of this implemention here is introducing FSM.
                if (m_uiCfgData.type == TestPlanEnum::Distance)
                {
                    qCInfo(DRONE_LOGGING) << "com.comm.state changed from State::" << (quint32)state
                                          << "to State" << (qint32)State::DistanceStepIntoLowState;
//                    mp_data->reset();
//                    mp_data->setDis(DistanceTstDataEnum::ZEROPOS);
//                    sendMeasStartCmd();
                    QTimer::singleShot(msecTimeInterval, [this](){ sendMeasDisStartCmd(DistanceTstDataEnum::GOTOZEROPOS);});
                    state = State::DistanceStepIntoLowState;
                }
                else
                {
                    qCInfo(DRONE_LOGGING) << "com.comm.state changed from State::" << (quint32)state
                                          << "to State" << (qint32)State::MeasRunningState;
                    mp_refresh->update();
                    QTimer::singleShot(msecTimeInterval, [this](){ sendMeasStartCmd();});
                    state = State::MeasRunningState;
                }
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

    case State::DistanceStepIntoLowState:
        if( signal.m_type == SignalType::ECHO &&
                (  signal.m_info.mp_dataUnit->registerType() == QModbus2DataUnit::RegisterType::MeasStartCode
                   || signal.m_info.mp_dataUnit->registerType() == QModbus2DataUnit::RegisterType::FatalErrorInfoCode
                   ))
        {
            const QModbus2DataUnit::LimitStatusEnum disState = processReceivedMeasDistanceDataUnit(signal.m_info.mp_dataUnit);
            if (QModbus2DataUnit::LimitStatusEnum::DOWNLIMIT == disState)
            {
                qCInfo(DRONE_LOGGING) << "com.comm.state changed from State::" << (quint32)State::DistanceStepIntoLowState
                        << "to State" << (int)State::DistanceStepIntoBegState;
                QTimer::singleShot(msecTimeInterval, [this](){
                    sendMeasDisStartCmd(m_uiCfgData.data.w.dis_beg);
                });

                state = State::DistanceStepIntoBegState;
            }
            else if (QModbus2DataUnit::LimitStatusEnum::RUNNING == disState
                     || QModbus2DataUnit::LimitStatusEnum::REACHED == disState
                     || QModbus2DataUnit::LimitStatusEnum::INVALID == disState
                     || QModbus2DataUnit::LimitStatusEnum::NOTARR == disState
                     || QModbus2DataUnit::LimitStatusEnum::ZEROPOS == disState )
            {
                qCDebug(DRONE_LOGGING) << "other signal was received during state " << (quint32)state
                           << "  with signal name " << (quint32)signal.m_type
                           << " with POS state" << (quint32)disState;

                QTimer::singleShot(msecTimeInterval, [this](){
                    sendMeasDisStartCmd(DistanceTstDataEnum::GOTOZEROPOS);
                });
            }
            else
            {
                qCWarning(DRONE_LOGGING) << "unexpected signal was received during state " << (quint32)state
                           << "  with signal name " << (quint32)signal.m_type
                           << " with POS " << (quint32)disState;
                enterFSMResetState(tr("Unexpected signal was received!"));
            }
        }
        else
        {
            qCWarning(DRONE_LOGGING) << "unexpected signal was received during state " << (quint32)state
                       << "  with signal name " << (quint32)signal.m_type;
            enterFSMResetState(tr("Unexpected signal was received!"));
        }
        break;

    case State::DistanceStepIntoBegState:
        if( signal.m_type == SignalType::ECHO &&
                (  signal.m_info.mp_dataUnit->registerType() == QModbus2DataUnit::RegisterType::MeasStartCode
                   || signal.m_info.mp_dataUnit->registerType() == QModbus2DataUnit::RegisterType::FatalErrorInfoCode
                   ))
        {
            const QModbus2DataUnit::LimitStatusEnum disState = processReceivedMeasDistanceDataUnit(signal.m_info.mp_dataUnit);
            if (QModbus2DataUnit::LimitStatusEnum::REACHED == disState
                    || QModbus2DataUnit::LimitStatusEnum::UPLIMIT == disState)
            {
                qCInfo(DRONE_LOGGING) << "com.comm.state changed from State::" << (quint32)State::DistanceStepIntoBegState
                        << "to State" << (int)State::MeasRunningState;
                mp_refresh->update();
                QTimer::singleShot(msecTimeInterval, [this](){ sendMeasStartCmd();});
                state = State::MeasRunningState;
            }
            else if (QModbus2DataUnit::LimitStatusEnum::RUNNING == disState
                     || QModbus2DataUnit::LimitStatusEnum::DOWNLIMIT == disState
                     || QModbus2DataUnit::LimitStatusEnum::INVALID == disState
                     || QModbus2DataUnit::LimitStatusEnum::ZEROPOS == disState)

            {
                qCDebug(DRONE_LOGGING) << "other signal was received during state " << (quint32)state
                           << "  with signal name " << (quint32)signal.m_type;
                QTimer::singleShot(msecTimeInterval, [this](){ sendMeasDisStartCmd(m_uiCfgData.data.w.dis_beg); });
            }
            else
            {
                qCWarning(DRONE_LOGGING) << "unexpected signal was received during state " << (quint32)state
                           << "  with signal name " << (quint32)signal.m_type
                           << " with POS " << (quint32)disState;
                enterFSMResetState(tr("Unexpected signal was received!"));
            }
        }
        else
        {
            qCWarning(DRONE_LOGGING) << "unexpected signal was received during state " << (quint32)state
                       << "  with signal name " << (quint32)signal.m_type;
            enterFSMResetState(tr("Unexpected signal was received!"));
        }
        break;

    case State::MeasRunningState:
    {
        if( signal.m_type == SignalType::ECHO &&
                (  signal.m_info.mp_dataUnit->registerType() == QModbus2DataUnit::RegisterType::MeasStartCode
                   || signal.m_info.mp_dataUnit->registerType() == QModbus2DataUnit::RegisterType::FatalErrorInfoCode ))
        {
            if (processReceivedMeasDataUnit(signal.m_info.mp_dataUnit))
            {
                emit updateData(signal.m_info.mp_dataUnit, mp_refresh->phase());
            }

            QModbus2DataUnit::LimitStatusEnum limitStatus = processReceivedMeasDistanceDataUnit(signal.m_info.mp_dataUnit);

            QTimer::singleShot(msecTimeInterval, [this, limitStatus](){
                if (mp_refresh->update()
                        || (m_uiCfgData.type == TestPlanEnum::Distance
                            && QModbus2DataUnit::LimitStatusEnum::UPLIMIT == limitStatus)
                   )
                {
                    qCInfo(DRONE_LOGGING) << "com.comm.state changed from State::" << (quint32)state
                                          << "to State - State::MeasFinishedState";
                    enterFSMResetState(tr("Measurement Finished!"));

//                    DataJsonRecElement::DataJsonRecElementFileHelper helper;
//                    helper.closeFile();
                }
                else
                {
                    sendMeasStartCmd();
                }
            });
            // We lazy some hard work in each end of the loop.
            doLaterReceivedMeasDataUnit(signal.m_info.mp_dataUnit);
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
    generateRandomNumber();
    QModbus2DataUnit::HandShakeStruct v;

    char* rawdata = (char*)&v;

    for (int i = 0; i < 16; i++)
    {
        *(rawdata + i) = m_randomNum.at(i);
    }
//    v.randomNum_1 = m_randomNum.at(0);
//    v.randomNum_2 = m_randomNum.at(1);
//    v.randomNum_3 = m_randomNum.at(2);
//    v.randomNum_4 = m_randomNum.at(3);

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

void AutomationModelDriverClz::sendMeasDisStartCmd(const quint32 val)
{
    QModbus2DataUnit::MeasStartStruct v;
    v.distance = val;
    v.thro_1 = 0;
    v.thro_2 = 0;
    v.vol = 0;
    QModbus2DataUnit data(QModbus2DataUnit::MeasStartCode, v);
    sendRequestCmd(data);
}

void AutomationModelDriverClz::sendMeasStartCmd()
{
    QModbus2DataUnit::MeasStartStruct v;
    v.distance = mp_data->getDis();
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
