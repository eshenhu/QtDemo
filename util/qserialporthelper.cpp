#include <QSerialPortInfo>
#include <QDebug>
#include "qserialporthelper.h"
#include "util/qserialporthelper.h"

QSerialPortSetting::Settings QSerialPortHelper::getPluggedInPort()
{
    QSerialPortSetting::Settings p;

    QSerialPort* serial = new QSerialPort();
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);

    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos)
    {
        p.name = info.portName();
        serial->setPortName(p.name);

        if (serial->open(QIODevice::ReadOnly)) {
            qDebug() << "util.qSerialPortHelper Open Port" << info.portName()
                    << "successfully!";
            serial->close();
            return p;
        }

        qDebug() << "util.qSerialPortHelper Open Port" << info.portName()
                << "failed!";
    }
    p.name = QString("");
    return p;
}
