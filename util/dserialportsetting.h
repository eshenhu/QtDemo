#ifndef DSERIALPORTSETTING_H
#define DSERIALPORTSETTING_H


#include <QString>
#include <QSerialPort>

class QSerialPortSetting
{
public:
    struct Settings {
        QString name = QString("");
        qint32 baudRate = QSerialPort::Baud115200;
        QSerialPort::DataBits dataBits = QSerialPort::Data8;
        QSerialPort::Parity parity = QSerialPort::NoParity;
        QSerialPort::StopBits stopBits = QSerialPort::OneStop;
        QSerialPort::FlowControl flowControl = QSerialPort::NoFlowControl;
        int responseTime = 1000;
        int numberOfRetries = 3;
    };

    explicit QSerialPortSetting(QWidget *parent = nullptr) = delete;
    ~QSerialPortSetting(){}
};

#endif // DSERIALPORTSETTING_H
