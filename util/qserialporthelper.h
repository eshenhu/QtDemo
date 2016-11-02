#ifndef QSERIALPORTHELPER_H
#define QSERIALPORTHELPER_H

#include "util/dserialportsetting.h"
class QSerialPortHelper
{
public:
    explicit QSerialPortHelper(QObject *parent = 0) = delete;

    static QSerialPortSetting::Settings getPluggedInPort();
};

#endif // QSERIALPORTHELPER_H
