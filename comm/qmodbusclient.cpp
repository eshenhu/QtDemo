/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtSerialBus module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qmodbusclient.h"
#include "qmodbusclient_p.h"
#include "qmodbus_symbols_p.h"
#include "qmodbusdataunit.h"
#include <QtCore/qdebug.h>
#include <QtCore/qloggingcategory.h>

#include <bitset>

QT_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(QT_MODBUS2)

/*!
    \class QModbus2Client
    \inmodule QtSerialBus
    \since 5.6

    \brief The QModbus2Client class is the interface to send Modbus requests.

    The QModbus2Client API is constructed around one QModbus2Client object, which holds the common
    configuration and settings for the requests it sends. One QModbus2Client should be enough for
    the whole Qt application.

    Once a QModbus2Client object has been created, the application can use it to send requests.
    The returned object is used to obtain any data returned in response to the corresponding request.

    QModbus2Client has an asynchronous API. When the finished slot is called, the parameter
    it takes is the QModbus2Reply object containing the PDU as well as meta-data (Addressing, etc.).

    Note: QModbus2Client queues the requests it receives. The number of requests executed in
    parallel is dependent on the protocol. For example, the HTTP protocol on desktop platforms
    issues 6 requests in parallel for one host/port combination.
*/

/*!
    Constructs a Modbus client device with the specified \a parent.
*/
QModbus2Client::QModbus2Client(QObject *parent)
    : QModbus2Device(*new QModbus2ClientPrivate, parent)
{
}

/*!
    \internal
*/
QModbus2Client::~QModbus2Client()
{
}

/*!
    Sends a request to read the contents of the data pointed by \a read.
    Returns a new valid \l QModbus2Reply object if no error occurred, otherwise
    nullptr. Modbus network may have multiple servers, each server has unique
    \a serverAddress.
*/
QModbus2Reply *QModbus2Client::sendReadRequest(const QModbus2DataUnit &read, int serverAddress)
{
    Q_D(QModbus2Client);
    return d->sendRequest(d->createReadRequest(read), serverAddress, &read);
}

///*!
//    Sends a request to modify the contents of the data pointed by \a write.
//    Returns a new valid \l QModbus2Reply object if no error occurred, otherwise
//    nullptr. Modbus network may have multiple servers, each server has unique
//    \a serverAddress.
//*/
//QModbus2Reply *QModbus2Client::sendWriteRequest(const QModbus2DataUnit &write, int serverAddress)
//{
//    Q_D(QModbus2Client);
//    return d->sendRequest(d->createWriteRequest(write), serverAddress, &write);
//}

///*!
//    Sends a request to read the contents of the data pointed by \a read and to
//    modify the contents of the data pointed by \a write using Modbus function
//    code \l QModbus2Pdu::ReadWriteMultipleRegisters.
//    Returns a new valid \l QModbus2Reply object if no error occurred, otherwise
//    nullptr. Modbus network may have multiple servers, each server has unique
//    \a serverAddress.

//    \note: Sending this kind of request is only valid of both \a read and
//    \a write are of type QModbus2DataUnit::HoldingRegisters.
//*/
//QModbus2Reply *QModbus2Client::sendReadWriteRequest(const QModbus2DataUnit &read,
//                                                  const QModbus2DataUnit &write, int serverAddress)
//{
//    Q_D(QModbus2Client);
//    return d->sendRequest(d->createRWRequest(read, write), serverAddress, &read);
//}

/*!
    Sends a raw Modbus \a request. A raw request can contain anything that
    fits inside the Modbus PDU data section and has a valid function code.
    The only check performed before sending is therefore the validity check,
    see \l QModbus2Pdu::isValid. If no error occurred the function returns a
    a new valid \l QModbus2Reply; nullptr otherwise. Modbus networks may have
    multiple servers, each server has a unique \a serverAddress.

    \sa QModbus2Reply::rawResult()
*/
QModbus2Reply *QModbus2Client::sendRawRequest(const QModbus2Request &request, int serverAddress)
{
    return d_func()->sendRequest(request, serverAddress, nullptr);
}

/*!
    \property QModbus2Client::timeout
    \brief the timeout value used by this client

    Returns the timeout value used by this QModbus2Client instance in ms.
    A timeout is indicated by a \l TimeoutError. The default value is 1000 ms.

    \sa setTimeout
*/
int QModbus2Client::timeout() const
{
    Q_D(const QModbus2Client);
    return d->m_responseTimeoutDuration;
}

/*!
    \fn void QModbus2Client::timeoutChanged(int newTimeout)

    This signal is emitted if the response is not received within the required
    timeout. The new response timeout for the device is passed as \a newTimeout.
*/

/*!
    Sets the \a newTimeout for this QModbus2Client instance. The minimum timeout
    is 50 ms.

    The timeout is used by the client to determine how long it waits for
    a response from the server. If the response is not received within the
    required timeout, the \l TimeoutError is set.

    Already active/running timeouts are not affected by such timeout duration
    changes.

    \sa timeout
*/
void QModbus2Client::setTimeout(int newTimeout)
{
    if (newTimeout < 50)
        return;

    Q_D(QModbus2Client);
    if (d->m_responseTimeoutDuration != newTimeout) {
        d->m_responseTimeoutDuration = newTimeout;
        emit timeoutChanged(newTimeout);
    }
}

/*!
    Returns the number of retries a client will perform before a
    request fails. The default value is set to \c 3.
*/
int QModbus2Client::numberOfRetries() const
{
    Q_D(const QModbus2Client);
    return d->m_numberOfRetries;
}

/*!
    Sets the \a number of retries a client will perform before a
    request fails. The default value is set to \c 3.

    \note The new value must be greater than or equal to \c 0. Changing this
    property will only effect new requests, not already scheduled ones.
*/
void QModbus2Client::setNumberOfRetries(int number)
{
    Q_D(QModbus2Client);
    if (number >= 0)
        d->m_numberOfRetries = number;
}

/*!
    \internal
*/
QModbus2Client::QModbus2Client(QModbus2ClientPrivate &dd, QObject *parent) :
    QModbus2Device(dd, parent)
{

}

/*!
    Processes a Modbus server \a response and stores the decoded information in \a data. Returns
    true on success; otherwise false.
*/
bool QModbus2Client::processResponse(const QModbus2Response &response, QModbus2DataUnit *data)
{
    return d_func()->processResponse(response, data);
}

/*!
    To be implemented by custom Modbus client implementation. The default implementation ignores
    \a response and \a data. It always returns false to indicate error.
*/
bool QModbus2Client::processPrivateResponse(const QModbus2Response &response, QModbus2DataUnit *data)
{
    Q_UNUSED(response)
    Q_UNUSED(data)
    return false;
}

QModbus2Reply *QModbus2ClientPrivate::sendRequest(const QModbus2Request &request, int serverAddress,
                                                const QModbus2DataUnit *const unit)
{
    Q_Q(QModbus2Client);

    if (!isOpen() || q->state() != QModbus2Device::ConnectedState) {
        qCWarning(QT_MODBUS2) << "(Client) Device is not connected";
        q->setError(QModbus2Client::tr("Device not connected."), QModbus2Device::ConnectionError);
        return nullptr;
    }

    if (!request.isValid()) {
        qCWarning(QT_MODBUS2) << "(Client) Refuse to send invalid request..";
        q->setError(QModbus2Client::tr("Invalid Modbus request."), QModbus2Device::ProtocolError);
        return nullptr;
    }

    if (unit)
    {
//        // eshenhu debugging...
//        qCWarning(QT_MODBUS2) << "(Client) Send request -->" << QString::number(serverAddress, 16) << request;
        return enqueueRequest(request, serverAddress, *unit, QModbus2Reply::Common);
    }
    return enqueueRequest(request, serverAddress, QModbus2DataUnit(), QModbus2Reply::Raw);
}

QModbus2Request QModbus2ClientPrivate::createReadRequest(const QModbus2DataUnit &data) const
{
    if (!data.isValid())
        return QModbus2Request();

    switch (data.registerType()) {
    case QModbus2DataUnit::ResetCode:
    {
        return QModbus2Request(QModbus2Request::ResetCode);
    }
        break;

    case QModbus2DataUnit::HandShakeCode:
    {
        const QModbus2DataUnit::HandShakeStruct& cast_p = data.uvalues().s.p;
        return QModbus2Request(QModbus2Request::HandShakeCode, cast_p.randomNum_1, cast_p.randomNum_2,
                                                               cast_p.randomNum_3, cast_p.randomNum_4);
    }
        break;

    case QModbus2DataUnit::FreqAdjustCode:
    {
        const QModbus2DataUnit::FreqAdjustStruct& cast_q = data.uvalues().s.q;
        return QModbus2Request(QModbus2Request::FreqAdjustCode, cast_q.freqValue_1, cast_q.freqValue_2);
    }
        break;

    case QModbus2DataUnit::AlarmInfoCode:
        return QModbus2Request(QModbus2Request::AlarmInfoCode);
        break;

    case QModbus2DataUnit::MeasStartCode:
    {
        const QModbus2DataUnit::MeasStartStruct& cast_q = data.uvalues().s.s;
        return QModbus2Request(QModbus2Request::MeasStartCode, (quint16)cast_q.vol, (quint8)cast_q.thro_1,
                               (quint8)cast_q.thro_2, (quint32)cast_q.distance);
    }
        break;

//    case QModbus2DataUnit::MeasEndCode:
//        return QModbus2Request(QModbus2Request::MeasEndCode);
//        break;

//    case QModbus2DataUnit::ManualMeasStartCode:
//    {
//        const QModbus2DataUnit::ManualMeasStruct& cast_s = data.uvalues().s.s;
//        return QModbus2Request(QModbus2Request::ManualMeasStartCode,
//                               cast_s.vol, cast_s.thro, cast_s.distance);
//    }
//        break;

//    case QModbus2DataUnit::ThroCalibrateCode:
//    {
//        const QModbus2DataUnit::ThroCalibrateStruct& cast_t = data.uvalues().s.t;
//        return QModbus2Request(QModbus2Request::ThroCalibrateCode,
//                               cast_t.timeInHigh, cast_t.timeInLow);
//    }
//        break;

    default:
        break;
    }
    return QModbus2Request();
}

//QModbus2Request QModbus2ClientPrivate::createWriteRequest(const QModbus2DataUnit &data) const
//{
//    switch (data.registerType()) {
//    case QModbus2DataUnit::Coils: {
//        if (data.valueCount() == 1) {
//            return QModbus2Request(QModbus2Request::WriteSingleCoil, quint16(data.startAddress()),
//                                  quint16((data.value(0) == 0u) ? Coil::Off : Coil::On));
//        }

//        quint8 byteCount = data.valueCount() / 8;
//        if ((data.valueCount() % 8) != 0)
//            byteCount += 1;

//        quint8 address = 0;
//        QVector<quint8> bytes;
//        for (quint8 i = 0; i < byteCount; ++i) {
//            std::bitset<8> byte;
//            for (int currentBit = 0; currentBit < 8; ++currentBit)
//                byte[currentBit] = data.value(address++);
//            bytes.append(static_cast<quint8> (byte.to_ulong()));
//        }

//        return QModbus2Request(QModbus2Request::WriteMultipleCoils, quint16(data.startAddress()),
//                              quint16(data.valueCount()), byteCount, bytes);
//    }   break;

//    case QModbus2DataUnit::HoldingRegisters: {
//        if (data.valueCount() == 1) {
//            return QModbus2Request(QModbus2Request::WriteSingleRegister, quint16(data.startAddress()),
//                                  data.value(0));
//        }

//        const quint8 byteCount = data.valueCount() * 2;
//        return QModbus2Request(QModbus2Request::WriteMultipleRegisters, quint16(data.startAddress()),
//                              quint16(data.valueCount()), byteCount, data.values());
//    }   break;

//    case QModbus2DataUnit::DiscreteInputs:
//    case QModbus2DataUnit::InputRegisters:
//    default:    // fall through on purpose
//        break;
//    }
//    return QModbus2Request();
//}

//QModbus2Request QModbus2ClientPrivate::createRWRequest(const QModbus2DataUnit &read,
//                                                     const QModbus2DataUnit &write) const
//{
//    if ((read.registerType() != QModbus2DataUnit::HoldingRegisters)
//        && (write.registerType() != QModbus2DataUnit::HoldingRegisters)) {
//        return QModbus2Request();
//    }

//    const quint8 byteCount = write.valueCount() * 2;
//    return QModbus2Request(QModbus2Request::ReadWriteMultipleRegisters, quint16(read.startAddress()),
//                          quint16(read.valueCount()), quint16(write.startAddress()),
//                          quint16(write.valueCount()), byteCount, write.values());
//}

void QModbus2ClientPrivate::processQueueElement(const QModbus2Response &pdu,
                                               const QueueElement &element)
{
    element.reply->setRawResult(pdu);
    if (pdu.isException()) {
        element.reply->setError(QModbus2Device::ProtocolError,
            QModbus2Client::tr("Modbus Exception Response."));
        return;
    }

    if (element.reply->type() == QModbus2Reply::Raw) {
        element.reply->setFinished(true);
        return;
    }

    QModbus2DataUnit unit = element.unit;
    if (!processResponse(pdu, &unit)) {
        element.reply->setError(QModbus2Device::UnknownError,
            QModbus2Client::tr("An invalid response has been received."));
        return;
    }

    element.reply->setResult(unit);
    element.reply->setFinished(true);
}

static bool isValid(const QModbus2Response &response, QModbus2Response::FunctionCode fc)
{
    if (!response.isValid())
        return false;
    if (response.isException())
        return false;
    if (response.functionCode() != fc)
        return false;
    return true;
}

bool QModbus2ClientPrivate::processResponse(const QModbus2Response &response, QModbus2DataUnit *data)
{
    if (!isValid(response, response.functionCode()))
        return false;

    switch (response.functionCode()) {
    case QModbus2Request::ResetPairCode:
        return processReadRestCodeResponse(response, data);
    case QModbus2Request::HandShakePairCode:
        return processReadHandShakeCodeResponse(response, data);
    case QModbus2Request::FreqAdjustPairCode:
        return processReadFreqAdjustCodeResponse(response, data);
//    case QModbus2Request::StartBtnCode:
//        return processReadStartBtnCodeResponse(response, data);
    case QModbus2Request::AlarmInfoPairCode:
        return processReadAlarmInfoCodeResponse(response, data);
    case QModbus2Request::MeasStartPairCode:
        return processReadMeasStartCodeResponse(response, data);
    case QModbus2Request::QueryAlarmInfoCode:
        return processReadQueryAlarmInfoCodeResponse(response, data);
    default:
        break;
    }
    return q_func()->processPrivateResponse(response, data);
}



bool QModbus2ClientPrivate::processReadRestCodeResponse(const QModbus2Response &response,
                                                        QModbus2DataUnit *data)
{
    // we will improve it later //eshenhu
//    if (response.dataSize() < QModbus2Response::minimumDataSize(response))
//        return false;

//    // byte count needs to match available bytes
//    const quint8 byteCount = response.data()[0];
//    if ((response.dataSize() - 1) != byteCount)
//        return false;

    if (data) {
        QDataStream stream(response.data());
        stream >> data->m_uvalues.r.p.status;
        data->setRegisterType(QModbus2DataUnit::ResetCode);
    }
    return true;
}

bool QModbus2ClientPrivate::processReadHandShakeCodeResponse(const QModbus2Response &response,
                                                             QModbus2DataUnit *data)
{
    if (data) {
        QDataStream stream(response.data());
        stream >> data->m_uvalues.r.q.mainboardHWRev
                >> data->m_uvalues.r.q.mainboardFirmwareRev
                  >> data->m_uvalues.r.q.sampleboardHWRev
                  >> data->m_uvalues.r.q.sampleboardFirmwareRev
                  >> data->m_uvalues.r.q.sampleboardFirmwareRev
                  >> data->m_uvalues.r.q.productRev
                  >> data->m_uvalues.r.q.secrectKeyMain_1
                  >> data->m_uvalues.r.q.secrectKeyMain_2
                  >> data->m_uvalues.r.q.secrectKeyMain_3
                  >> data->m_uvalues.r.q.secrectKeyMain_4
                  >> data->m_uvalues.r.q.secrectKeySample_1
                  >> data->m_uvalues.r.q.secrectKeySample_2
                  >> data->m_uvalues.r.q.secrectKeySample_3
                  >> data->m_uvalues.r.q.secrectKeySample_4;

        // to be continue //eshenhu
        data->setRegisterType(QModbus2DataUnit::HandShakeCode);
    }
    return true;
}

bool QModbus2ClientPrivate::processReadFreqAdjustCodeResponse(const QModbus2Response &response,
                                                              QModbus2DataUnit *data)
{
    if (data) {
        QDataStream stream(response.data());
        stream >> data->m_uvalues.r.r.status;
        data->setRegisterType(QModbus2DataUnit::FreqAdjustCode);
    }
    return true;
}

//bool QModbus2ClientPrivate::processReadStartBtnCodeResponse(const QModbus2Response &response,
//                                                            QModbus2DataUnit *data)
//{
//    if (data) {
//        QDataStream stream(response.data());
//        stream >> data->m_uvalues.r.s.status;
//        data->setRegisterType(QModbus2DataUnit::StartBtnCode);
//    }
//    return true;
//}

bool QModbus2ClientPrivate::processReadAlarmInfoCodeResponse(const QModbus2Response &response,
                                                             QModbus2DataUnit *data)
{
    if (data) {
        QDataStream stream(response.data());
        stream >> data->m_uvalues.r.t.status;
        data->setRegisterType(QModbus2DataUnit::AlarmInfoCode);
    }
    return true;
}

bool QModbus2ClientPrivate::processReadMeasStartCodeResponse(const QModbus2Response &response,
                                                             QModbus2DataUnit *data)
{
    if (data) {
        QDataStream stream(response.data());
        QModbus2DataUnit::MeasStartRecStruct& val = data->m_uvalues.r.s;

//        const QModbus2DataUnit::MeasDataUnion& data2 = data->uvalues();
//        int size = sizeof(QModbus2DataUnit::MeasDataUnion);
//        const char* rawdata = (const char*)&data2;
//        QByteArray barray = QByteArray::fromRawData(rawdata, size);
//        qCDebug(QT_MODBUS2) << "com.comm.qmodbusclient receive" << barray.toHex();

//        qCDebug(QT_MODBUS2) << "com.comm.qmodbusclient receive" << response.data().toHex();

        stream >> val.humidity >> val.envtemp >> val.pressure >> val.thro_1 >> val.thro_2
               >> val.backup_1 >> val.backup_2 >> val.motorType >> val.numOfMotor;

        qCDebug(QT_MODBUS2) << "com.comm.data = val.humidity" << val.humidity << "val.envtemp" << val.envtemp
                << "val.pressure" << val.pressure << "val.thro_1 " << val.thro_1 << "val.thro_2" << val.thro_2
                << "val.backup_1" << val.backup_1 << "val.backup_2" << val.backup_2
                << "val.motorType" << val.motorType << "val.numOfMotor" << val.numOfMotor;

        if (val.motorType == static_cast<quint8>(QModbus2DataUnit::MotorTypeEnum::ELECE))
        {
            stream >> val.motorInfo.elec.limitStatus >> val.motorInfo.elec.voltage
                    >> val.motorInfo.elec.elecMotorStruct[0].current >> val.motorInfo.elec.elecMotorStruct[0].lift
                    >> val.motorInfo.elec.elecMotorStruct[0].torque >> val.motorInfo.elec.elecMotorStruct[0].speed
                    >> val.motorInfo.elec.elecMotorStruct[0].temp_1 >> val.motorInfo.elec.elecMotorStruct[0].temp_2;

            qCDebug(QT_MODBUS2) << "com.comm.data = elec.limitStatus" << val.motorInfo.elec.limitStatus
                                << "elec.voltage" << val.motorInfo.elec.voltage
                                << "elec.elecMotorStruct[0].current " << val.motorInfo.elec.elecMotorStruct[0].current;

            if (val.numOfMotor >= 2)
            {
                stream >> val.motorInfo.elec.limitStatus >> val.motorInfo.elec.voltage
                        >> val.motorInfo.elec.elecMotorStruct[1].current >> val.motorInfo.elec.elecMotorStruct[1].lift
                        >> val.motorInfo.elec.elecMotorStruct[1].torque >> val.motorInfo.elec.elecMotorStruct[1].speed
                        >> val.motorInfo.elec.elecMotorStruct[1].temp_1 >> val.motorInfo.elec.elecMotorStruct[1].temp_2;
            }
        }
        else if (val.motorType == static_cast<quint8>(QModbus2DataUnit::MotorTypeEnum::OILE))
        {
            stream >> val.motorInfo.oil.lift >> val.motorInfo.oil.torque
                    >> val.motorInfo.oil.speed >> val.motorInfo.oil.temp_1 >> val.motorInfo.oil.temp_2;
        }
        else
        {
            qCCritical(QT_MODBUS2) << "com.qt_modbus2 The received moto type can't match any of existed motor";
        }


        data->setRegisterType(QModbus2DataUnit::MeasStartCode);
    }
    return true;
}

//bool QModbus2ClientPrivate::processReadMeasEndCodeResponse(const QModbus2Response &response,
//                                                           QModbus2DataUnit *data)
//{
//    if (data) {
//        data->setRegisterType(QModbus2DataUnit::MeasEndCode);
//    }
//    return true;

//}

bool QModbus2ClientPrivate::processReadManualMeasStartCodeResponse(const QModbus2Response &response,
                                                                   QModbus2DataUnit *data)
{
    if (data) {
        QDataStream stream(response.data());
        QModbus2DataUnit::MeasStartRecStruct& val = data->m_uvalues.r.s;

        const QModbus2DataUnit::MeasDataUnion& data2 = data->uvalues();
        int size = sizeof(QModbus2DataUnit::MeasDataUnion);
        const char* rawdata = (const char*)&data2;
        QByteArray barray = QByteArray::fromRawData(rawdata, size);
        qCDebug(QT_MODBUS2) << "com.comm.qmodbusclient receive" << barray.toHex();

        stream >> val.humidity >> val.envtemp >> val.pressure >> val.thro_1 >> val.thro_2
                >> val.motorType >> val.numOfMotor;

        if (val.motorType == static_cast<quint8>(QModbus2DataUnit::MotorTypeEnum::ELECE))
        {
            stream >> val.motorInfo.elec.limitStatus >> val.motorInfo.elec.voltage
                    >> val.motorInfo.elec.elecMotorStruct[0].current >> val.motorInfo.elec.elecMotorStruct[0].lift
                    >> val.motorInfo.elec.elecMotorStruct[0].torque >> val.motorInfo.elec.elecMotorStruct[0].speed
                    >> val.motorInfo.elec.elecMotorStruct[0].temp_1 >> val.motorInfo.elec.elecMotorStruct[0].temp_2;

            if (val.numOfMotor >= 2)
            {
                stream >> val.motorInfo.elec.limitStatus >> val.motorInfo.elec.voltage
                        >> val.motorInfo.elec.elecMotorStruct[1].current >> val.motorInfo.elec.elecMotorStruct[1].lift
                        >> val.motorInfo.elec.elecMotorStruct[1].torque >> val.motorInfo.elec.elecMotorStruct[1].speed
                        >> val.motorInfo.elec.elecMotorStruct[1].temp_1 >> val.motorInfo.elec.elecMotorStruct[1].temp_2;
            }
        }
        else
        {
            stream >> val.motorInfo.oil.lift >> val.motorInfo.oil.torque
                    >> val.motorInfo.oil.speed >> val.motorInfo.oil.temp_1 >> val.motorInfo.oil.temp_2;
        }

        qCDebug(QT_MODBUS2) << "com.comm.data = val.humidity" << val.humidity << "val.envtemp" << val.envtemp
                << "val.pressure" << val.pressure << "val.thro_1 " << val.thro_1 << "val.thro_2" << val.thro_2;
        data->setRegisterType(QModbus2DataUnit::MeasStartCode);
    }
    return true;
}

bool QModbus2ClientPrivate::processReadQueryAlarmInfoCodeResponse(const QModbus2Response &response,
                                                                  QModbus2DataUnit *data)
{
    if (data) {
        QDataStream stream(response.data());
        stream >> data->m_uvalues.r.u.errorCode
               >> data->m_uvalues.r.u.errorInfo;
        data->setRegisterType(QModbus2DataUnit::FatalErrorInfoCode);
    }
    return true;
}

//bool QModbus2ClientPrivate::collateBits(const QModbus2Pdu &response,
//                                     QModbus2DataUnit::RegisterType type, QModbus2DataUnit *data)
//{
//    if (response.dataSize() < QModbus2Response::minimumDataSize(response))
//        return false;

//    const QByteArray payload = response.data();
//    // byte count needs to match available bytes
//    if ((payload.size() - 1) != payload[0])
//        return false;

//    if (data) {
//        uint value = 0;
//        for (qint32 i = 1; i < payload.size(); ++i) {
//            const std::bitset<8> byte = payload[i];
//            for (qint32 currentBit = 0; currentBit < 8 && value < data->valueCount(); ++currentBit)
//                data->setValue(value++, byte[currentBit]);
//        }
//        data->setRegisterType(type);
//    }
//    return true;
//}

//bool QModbus2ClientPrivate::processReadHoldingRegistersResponse(const QModbus2Response &response,
//                                                               QModbus2DataUnit *data)
//{
//    if (!isValid(response, QModbus2Response::ReadHoldingRegisters))
//        return false;
//    return collateBytes(response, QModbus2DataUnit::HoldingRegisters, data);
//}

//bool QModbus2ClientPrivate::collateBytes(const QModbus2Pdu &response,
//                                      QModbus2DataUnit::RegisterType type, QModbus2DataUnit *data)
//{
//    if (response.dataSize() < QModbus2Response::minimumDataSize(response))
//        return false;

//    // byte count needs to match available bytes
//    const quint8 byteCount = response.data()[0];
//    if ((response.dataSize() - 1) != byteCount)
//        return false;

//    // byte count needs to be odd to match full registers
//    if (byteCount % 2 != 0)
//        return false;

//    if (data) {
//        QDataStream stream(response.data().remove(0, 1));

//        QVector<quint16> values;
//        const quint8 itemCount = byteCount / 2;
//        for (int i = 0; i < itemCount; i++) {
//            quint16 tmp;
//            stream >> tmp;
//            values.append(tmp);
//        }
//        data->setValues(values);
//        data->setRegisterType(type);
//    }
//    return true;
//}

QT_END_NAMESPACE
