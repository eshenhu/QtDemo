#ifndef SIGNALOVERLINE_H
#define SIGNALOVERLINE_H

class QModbus2DataUnit;

enum class SignalType {
    USER,
    TIMEOUT,
    ECHO
};

enum class SignalTypeUserInfoE{
    START = 0,
    STOP = 1
};

enum class SignalTypeTMOInfoE{
    TMO500MS = 500,
    TMO1SEC = 1000,
    TMO5SEC = 5000,
};


union SignalAssistInfo {
    const QModbus2DataUnit* mp_dataUnit;
    SignalTypeUserInfoE userType;
    SignalTypeTMOInfoE tmo;
};

class SignalOverLine
{
public:
    SignalType m_type;
    SignalAssistInfo m_info;
public:
    ~SignalOverLine(){}
    SignalOverLine(const QModbus2DataUnit* pv)
    {
        m_type = SignalType::ECHO;
        m_info.mp_dataUnit = pv;
    }

    SignalOverLine(const SignalTypeUserInfoE type)
    {
        m_type = SignalType::USER;
        m_info.userType = type;
    }

    SignalOverLine(const SignalTypeTMOInfoE tmo)
    {
        m_type = SignalType::TIMEOUT;
        m_info.tmo = tmo;
    }
};

#endif // SIGNALOVERLINE_H
