#ifndef AUTOMATIONMODELDRIVERCLZ_H
#define AUTOMATIONMODELDRIVERCLZ_H

#include "basedmodeldriverclz.h"

#include <QTimer>
#include "ui/settingsdialog.h"
#include "comm/qmodbusdataunit.h"
#include "driver/signaloverline.h"

class QModbus2Client;
class QModbus2Reply;
class CharDataFormat;

class MeasDataFormat;
class CfgResHandlerInf;
class MeasDataUpdateInf;

class AutomationModelDriverClz : public BasedModelDriverClz
{
    Q_OBJECT
    enum class State {
        InitState = 0,
        ResetState,
        HandShakeState,
        FreqAdjustState,
        StartBtnQueryState,
        AlarmQueryState,
        MeasOptionsState,
        MeasRunningState,
        MeasFinishedState,
    };

    enum QModBusState {
        Connected,
        Disconnected
    };

    const int fixedServerAddress = 0xF077;
    const int msecTimeInterval = 500;  //ms

public:
    explicit AutomationModelDriverClz(QObject *parent = 0);
    ~AutomationModelDriverClz();

private:
    QTimer m_stateTimer;
    QTimer m_sendTimer;
    State state = State::InitState;
    SettingsDialog::Settings m_settingDialog;

    MeasDataFormat* mp_data;
    CfgResHandlerInf* mp_cfgRes;
    MeasDataUpdateInf* mp_refresh;

    //ChartDataFormat data;
    QModbus2Reply *lastRequest = nullptr;
    QModbus2Client *modbusDevice = nullptr;

signals:
    void statusBarChanged(const QString&, int timeDuration);
    void stateChanged(const QModBusState);
public slots:
    void startMeasTest(bool start = true);
    void readReady();

private:
    void connect();


    void processSendTimeout();
    void setupModbusDevice();
    void sendRequestCmd(const QModbus2DataUnit& writeUnit);

    void sendResetCmd();
    void sendHandShakeCmd();
    void sendFreqAdjustCmd();
    void sendAlarmQueryCmd();
    void sendMeasStartCmd();

    void processDataHandlerSingleShot(const SignalOverLine& signal);
    void processReceivedDataUnit(const QModbus2DataUnit& data);
    bool processReceivedHandShakeDataUnit(const QModbus2DataUnit* data);
    bool processReceivedMeasDataUnit(const QModbus2DataUnit* const data);

};

#endif // AUTOMATIONMODELDRIVERCLZ_H
