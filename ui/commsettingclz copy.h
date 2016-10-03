#ifndef COMMSETTINGCLZ_H
#define COMMSETTINGCLZ_H

#include <QWidget>
#include <QSerialPort>

class QComboBox;

class CommSettingClz : public QWidget
{
    Q_OBJECT
public:
    struct Settings {
        int parity = QSerialPort::EvenParity;
        int baud = QSerialPort::Baud115200;
        int dataBits = QSerialPort::Data8;
        int stopBits = QSerialPort::OneStop;
        int responseTime = 1000;
        int numberOfRetries = 3;
    };

    Q_PROPERTY(Settings commSetting READ commSetting WRITE setCommSetting NOTIFY commSettingChanged)

    QComboBox* parityCombo;
    QComboBox* baudCombo;
    QComboBox* dataBitsCombo;
    QComboBox* stopBitsCombo;
    QSpinBox* timeoutSpinner;
    QSpinBox* retriesSpinner;

public:
    explicit CommSettingClz(QWidget *parent = 0);
    ~CommSettingClz();

    CommSettingClz::Settings comSetting() const
    {
        return commSetting;
    }

    void setCommSetting(CommSettingClz::Settings set)
    {
        commSetting = set;
        emit commSettingChanged(set);
    }

signals:
    void commSettingChanged(Settings);

public slots:
};

#endif // COMMSETTINGCLZ_H
