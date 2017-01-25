#ifndef AUTOMATIONMODELDRIVERCLZ_H
#define AUTOMATIONMODELDRIVERCLZ_H

#include "basedmodeldriverclz.h"
#include <memory>
#include <QTimer>
#include "util/qserialporthelper.h"
#include "comm/qmodbusdataunit.h"
#include "driver/signaloverline.h"
#include "ui/uiheader.h"
#include "qtcore/qloggingcategory.h"
#include "driver/modelpoctype.h"
using namespace ModelPOC;


class QModbus2Client;
class QModbus2Reply;
class CharDataFormat;

class MeasDataFormat;
class CfgResHandlerInf;
class MeasDataUpdateInf;


Q_DECLARE_LOGGING_CATEGORY(DRONE_LOGGING)

class FatalErrorDrvClz
{
public:
    FatalErrorDrvClz():
        m_cntUpCRC(0),
        m_cntUpFrameError(0)
    {}

public:
    inline void cntUpCRC() {
        if (m_cntUpCRC < 0xFF)
            ++m_cntUpCRC;
    }

    inline void resetCRC() {
        if (m_cntUpCRC > 0)
            --m_cntUpCRC;
    }

    inline void cntUpFrameError() {
        if (m_cntUpFrameError < 0xFF)
            ++m_cntUpFrameError;
    }

    inline bool resetFrameError() {
        if (m_cntUpFrameError > 0)
            --m_cntUpFrameError;
    }

    inline bool isCRCError() {
        return m_cntUpCRC > 5 ? true : false;
    }

    inline bool isFrameError() {
        return m_cntUpFrameError > 5 ? true : false;
    }

private:
    quint8 m_cntUpCRC;
    quint8 m_cntUpFrameError;

};

class AutomationModelDriverClz : public BasedModelDriverClz
{
    Q_OBJECT

public:
    enum class State {
        InitState = 0,
        ResetState,
        HandShakeState,
        FreqAdjustState,
        StartBtnQueryState,
        AlarmQueryState,
        DistanceStepIntoLowState,
        DistanceStepIntoBegState,
        MeasRunningState,
        MeasFinishedState,
    };

    enum QModBusState {
        Connected,
        Disconnected,
        HandShakeException,
        FatalErrorException
    };

    const int fixedServerAddress = 0xCCF0;
    const int msecTimeInterval = 500;  //ms

public:
    explicit AutomationModelDriverClz(QObject *parent = 0);
    ~AutomationModelDriverClz();

private:
    QTimer m_stateTimer;
    QTimer m_sendTimer;
    State state = State::InitState;
    QSerialPortSetting::Settings m_settingDialog;

    UiCompMeasData  m_uiCfgData;
    MeasDataFormat* mp_data;
    CfgResHandlerInf* mp_cfgRes;
    MeasDataUpdateInf* mp_refresh;

    //ChartDataFormat data;
    QModbus2Reply *lastRequest = nullptr;
    QModbus2Client *modbusDevice = nullptr;

    std::unique_ptr<FatalErrorDrvClz> m_monitorError;

signals:
    void statusBarChanged(const QString&, int timeDuration);
    void stateChanged(const QModBusState, QString);
    void updateData(const QModbus2DataUnit* data, Phase phase);

public slots:
    void readReady();
    void syncDataDuringManualTest(const double vol, const quint32 thro);

public:
    void startMeasTest(const UiCompMeasData data, const CfgResHandlerInf* res, const QSerialPortSetting::Settings setting);

public:
    void enterFSMInitState();
    void enterFSMResetState(const QString&);

private:
    void connect();

    void resetMeasDataUnit();

    void processSendTimeout();
    void setupModbusDevice();
    void sendRequestCmd(const QModbus2DataUnit& writeUnit);

    void sendResetCmd();
    void sendHandShakeCmd();
    void sendFreqAdjustCmd();
    void sendAlarmQueryCmd();
    void sendMeasDisStartCmd(const quint32);
    void sendMeasDisSpecCmd();
    void sendMeasStartCmd();

    void processDataHandlerSingleShot(const SignalOverLine& signal);
    void processReceivedDataUnit(const QModbus2DataUnit& data);

    bool processReceivedHandShakeDataUnit(const QModbus2DataUnit* data);
    QModbus2DataUnit::LimitStatusEnum processReceivedMeasDistanceDataUnit(const QModbus2DataUnit* data);


    bool processReceivedMeasDataUnit(const QModbus2DataUnit* const data);
    void doLaterReceivedMeasDataUnit(const QModbus2DataUnit* const data);
};

#endif // AUTOMATIONMODELDRIVERCLZ_H
