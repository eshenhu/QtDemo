#ifndef AUTOMATIONMODELDRIVERCLZ_H
#define AUTOMATIONMODELDRIVERCLZ_H

#include "basedmodeldriverclz.h"

#include <QTimer>
#include "util/qserialporthelper.h"
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
        Disconnected,
        HandShakeException,
    };

    const int fixedServerAddress = 0xF0CC;
    const int msecTimeInterval = 500;  //ms

public:
    explicit AutomationModelDriverClz(QObject *parent = 0);
    ~AutomationModelDriverClz();

private:
    QTimer m_stateTimer;
    QTimer m_sendTimer;
    State state = State::InitState;
    QSerialPortSetting::Settings m_settingDialog;

    MeasDataFormat* mp_data;
    CfgResHandlerInf* mp_cfgRes;
    MeasDataUpdateInf* mp_refresh;

    //ChartDataFormat data;
    QModbus2Reply *lastRequest = nullptr;
    QModbus2Client *modbusDevice = nullptr;

    //QQueue<QModbus2DataUnit> m_queue;

signals:
    void statusBarChanged(const QString&, int timeDuration);
    void stateChanged(const QModBusState, QString);
    void updateData(const QModbus2DataUnit* data);

public slots:
    void readReady();

public:
    void startMeasTest(const QSerialPortSetting::Settings setting);
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
