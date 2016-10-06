#ifndef AUTOMATIONMODELDRIVERCLZ_H
#define AUTOMATIONMODELDRIVERCLZ_H

#include "basedmodeldriverclz.h"

#include <QTimer>
#include "ui/settingsdialog.h"
#include "comm/qmodbusdataunit.h"

class QModbus2Client;
class QModbus2Reply;
class CharDataFormat;

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
        MeasStopState,
    };

    enum QModBusState {
        Connected,
        Disconnected
    };
    const int fixedServerAddress = 0xF077;
    const int msecTimeInterval = 200;  //ms

public:
    explicit AutomationModelDriverClz(QObject *parent = 0);
    ~AutomationModelDriverClz();

private:
    QTimer m_stateTimer;
    QTimer m_sendTimer;
    State state = State::InitState;
    SettingsDialog::Settings m_settingDialog;


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

    void processReceivedDataUnit(const QModbus2DataUnit& data);

    void processSendTimeout();
    void setupModbusDevice();
    void sendRequestCmd(QModbus2DataUnit writeUnit);
    void processDataHandlerSingleShot();
    void sendResetCmd();
    void sendHandShakeCmd();
    void sendAlarmQueryCmd();
    void sendMeasStartOptionsCmd();
    void sendMeasStartCmd();
    void sendMeasStopCmd();
    void sendFreqAdjustCmd();
    void sendStartBtnQueryCmd();
};

#endif // AUTOMATIONMODELDRIVERCLZ_H
