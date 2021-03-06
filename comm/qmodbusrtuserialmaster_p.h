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

#ifndef QMODBUSSERIALMASTER_P_H
#define QMODBUSSERIALMASTER_P_H

#include <QtCore/qloggingcategory.h>
#include <QtCore/qmath.h>
#include <QtCore/qpointer.h>
#include <QtCore/qqueue.h>
#include <QtCore/qtimer.h>

#include "qmodbusrtuserialmaster.h"
#include "qserialport.h"

#include "qmodbusadu_p.h"
#include "qmodbusclient_p.h"
#include "qmodbus_symbols_p.h"
#include "qmodbusdataunit.h"

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API. It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

QT_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(QT_MODBUS2)
Q_DECLARE_LOGGING_CATEGORY(QT_MODBUS2_LOW)

class QModbus2RtuSerialMasterPrivate : public QModbus2ClientPrivate
{
    Q_DECLARE_PUBLIC(QModbus2RtuSerialMaster)
    enum State {
        Idle,
        Schedule,
        Send,
        Receive,
    } m_state = Idle;

public:
    void setupSerialPort()
    {
        Q_Q(QModbus2RtuSerialMaster);

        m_sendTimer.setSingleShot(true);
        QObject::connect(&m_sendTimer, &QTimer::timeout, q, [this]() { processQueue(); });

        m_responseTimer.setSingleShot(true);
        QObject::connect(&m_responseTimer, &QTimer::timeout, q, [this]() { processQueue(); });

        m_serialPort = new QSerialPort(q);
        QObject::connect(m_serialPort, &QSerialPort::readyRead, q, [this]() {
            responseBuffer += m_serialPort->read(m_serialPort->bytesAvailable());
            qCDebug(QT_MODBUS2_LOW) << "(RTU client) Response buffer:" << responseBuffer.toHex();

            if (responseBuffer.size() < QModbus2Response::getMinDataSize()) {
                qCDebug(QT_MODBUS2) << "(RTU client) Modbus ADU not complete";
                return;
            }

            quint16 serverAddr, pduSize, compPduSize;
            QDataStream out(&responseBuffer, QIODevice::ReadOnly);
            out.setByteOrder(QDataStream::LittleEndian);
            out >> serverAddr >> pduSize >> compPduSize;

            if ((quint16)~pduSize != (quint16)compPduSize)
            {
                // data corrupt!
                qCWarning(QT_MODBUS2) << "(RTU client) data corrupt! :" << "aduSize = "
                                   << pduSize << "compAduSize = " << compPduSize
                                   << ", delaying pending frame";
                return;
            }

            //pduSize = (pduSize & 0x00FF) << 8 | (pduSize & 0xFF00) >> 8;

            qCDebug(QT_MODBUS2) << "(RTU client) :" << "aduSize = "
                               << pduSize << "compAduSize = " << compPduSize;

//            const QModbusSerialAdu tmpAdu(QModbusSerialAdu::Rtu, responseBuffer);

//            int pduSizeWithoutFcode = QModbus2Response::calculateDataSize(tmpAdu.pdu());
//            if (pduSizeWithoutFcode < 0) {
//                // wait for more data
//                qCDebug(QT_MODBUS2) << "(RTU client) Cannot calculate PDU size for function code:"
//                                   << tmpAdu.pdu().functionCode() << ", delaying pending frame";
//                return;
//            }


            //server address byte + function code byte + PDU size + 2 bytes CRC
            qint32 aduSize = 2 + 2 + 2 + pduSize + 2;
            if (responseBuffer.size() < aduSize) {
                qCDebug(QT_MODBUS2) << "(RTU client) Incomplete ADU received, ignoring";
                return;
            }

            const QModbusSerialAdu adu(QModbusSerialAdu::Rtu, responseBuffer.left(aduSize));
            responseBuffer.remove(0, aduSize);

            qCDebug(QT_MODBUS2)<< "(RTU client) Received ADU:" << adu.data().toHex();
            if (QT_MODBUS2().isDebugEnabled() && !responseBuffer.isEmpty())
                qCDebug(QT_MODBUS2_LOW) << "(RTU client) Pending buffer:" << responseBuffer.toHex();

//            // check CRC
//            if (!adu.matchingChecksum()) {
//                qCWarning(QT_MODBUS2) << "(RTU client) Discarding response with wrong CRC, received:"
//                                     << adu.checksum<quint16>() << ", calculated CRC:"
//                                     << QModbusSerialAdu::calculateCRC(adu.data(), adu.size());
//                return;
//            }

            const QModbus2Response response = adu.pdu();
            if (!canMatchRequestAndResponse(response, adu.serverAddress())) {
                qCWarning(QT_MODBUS2) << "(RTU client) Cannot match response with open request, "
                    "ignoring";
                return;
            }

            m_sendTimer.stop();
            m_responseTimer.stop();
            processQueueElement(response, m_current);

            m_state = Schedule; // reschedule, even if empty
            m_serialPort->clear(QSerialPort::AllDirections);
            QTimer::singleShot(m_interFrameDelayMilliseconds, [this]() { processQueue(); });
        });

        using TypeId = void (QSerialPort::*)(QSerialPort::SerialPortError);
        QObject::connect(m_serialPort, static_cast<TypeId>(&QSerialPort::error),
                         [this](QSerialPort::SerialPortError error) {
            if (error == QSerialPort::NoError)
                return;

            qCDebug(QT_MODBUS2) << "(RTU server) QSerialPort error:" << error
                               << (m_serialPort ? m_serialPort->errorString() : QString());

            Q_Q(QModbus2RtuSerialMaster);

            switch (error) {
            case QSerialPort::DeviceNotFoundError:
                q->setError(QModbus2Device::tr("Referenced serial device does not exist."),
                            QModbus2Device::ConnectionError);
                break;
            case QSerialPort::PermissionError:
                q->setError(QModbus2Device::tr("Cannot open serial device due to permissions."),
                            QModbus2Device::ConnectionError);
                break;
            case QSerialPort::OpenError:
            case QSerialPort::NotOpenError:
                q->setError(QModbus2Device::tr("Cannot open serial device."),
                            QModbus2Device::ConnectionError);
                break;
            case QSerialPort::WriteError:
                q->setError(QModbus2Device::tr("Write error."), QModbus2Device::WriteError);
                break;
            case QSerialPort::ReadError:
                q->setError(QModbus2Device::tr("Read error."), QModbus2Device::ReadError);
                break;
            case QSerialPort::ResourceError:
                q->setError(QModbus2Device::tr("Resource error."), QModbus2Device::ConnectionError);
                break;
            case QSerialPort::UnsupportedOperationError:
                q->setError(QModbus2Device::tr("Device operation is not supported error."),
                            QModbus2Device::ConfigurationError);
                break;
            case QSerialPort::TimeoutError:
                q->setError(QModbus2Device::tr("Timeout error."), QModbus2Device::TimeoutError);
                break;
            case QSerialPort::UnknownError:
                q->setError(QModbus2Device::tr("Unknown error."), QModbus2Device::UnknownError);
                break;
            default:
                qCDebug(QT_MODBUS2) << "(RTU server) Unhandled QSerialPort error" << error;
                break;
            }
        });

        QObject::connect(m_serialPort, &QSerialPort::bytesWritten, q, [this](qint64 bytes) {
            qCDebug(QT_MODBUS2) << "(RTU client) bytesWritten to device" << m_current.bytesWritten;
            m_current.bytesWritten += bytes;
        });

        QObject::connect(m_serialPort, &QSerialPort::aboutToClose, q, [this]() {
            Q_Q(QModbus2RtuSerialMaster);
            if (q->state() != QModbus2Device::ClosingState)
                q->close();
            m_sendTimer.stop();
            m_responseTimer.stop();
        });
    }

    /*!
        According to the Modbus specification, in RTU mode message frames
        are separated by a silent interval of at least 3.5 character times.
        Calculate the timeout if we are less than 19200 baud, use a fixed
        timeout for everything equal or greater than 19200 baud.
        If the user set the timeout to be longer than the calculated one,
        we'll keep the user defined.
    */
    void calculateInterFrameDelay() {
        // The spec recommends a timeout value of 1.750 msec. Without such
        // precise single-shot timers use a approximated value of 1.750 msec.
        int delayMilliSeconds = 2;
        if (m_baudRate < 19200) {
            // Example: 9600 baud, 11 bit per packet -> 872 char/sec
            // so: 1000 ms / 872 char = 1.147 ms/char * 3.5 character
            // Always round up because the spec requests at least 3.5 char.
            delayMilliSeconds = qCeil(3500. / (qreal(m_baudRate) / 11.));
        }
        if (m_interFrameDelayMilliseconds < delayMilliSeconds)
            m_interFrameDelayMilliseconds = delayMilliSeconds;
    }

    void setupEnvironment() {
        if (m_serialPort) {
            m_serialPort->setPortName(m_comPort);
            m_serialPort->setParity(m_parity);
            m_serialPort->setBaudRate(m_baudRate);
            m_serialPort->setDataBits(m_dataBits);
            m_serialPort->setStopBits(m_stopBits);
        }

        calculateInterFrameDelay();

        responseBuffer.clear();
        m_state = QModbus2RtuSerialMasterPrivate::Idle;
    }

    void scheduleNextRequest() {
        m_state = Schedule;
        m_serialPort->clear(QSerialPort::AllDirections);
        QTimer::singleShot(m_interFrameDelayMilliseconds, [this]() { processQueue(); });
    }

    QModbus2Reply *enqueueRequest(const QModbus2Request &request, int serverAddress,
        const QModbus2DataUnit &unit, QModbus2Reply::ReplyType type) override
    {
        Q_Q(QModbus2RtuSerialMaster);

        auto reply = new QModbus2Reply(type, serverAddress, q);
        QueueElement element(reply, request, unit, m_numberOfRetries + 1);
        element.adu = QModbusSerialAdu::create(QModbusSerialAdu::Rtu, serverAddress, request);
        qCWarning(QT_MODBUS2) << "(Client) send raw data" << element.adu.toHex();
        m_queue.enqueue(element);

        if (m_state == Idle)
            scheduleNextRequest();
        return reply;
    }

    void processQueue()
    {
        Q_ASSERT_X(!m_sendTimer.isActive(), "processQueue", "send timer active");
        Q_ASSERT_X(!m_responseTimer.isActive(), "processQueue", "response timer active");

        auto writeAdu = [this]() {
            responseBuffer.clear();
            m_current.bytesWritten = 0;
            m_current.numberOfRetries--;
            m_serialPort->write(m_current.adu);
            // Example: 9600 baud, 11 bit per packet -> 872 char/sec
            // so: 1000 ms / 872 char = 1.147 ms/char * 3.5 character
            m_sendTimer.start((1000. / (qreal(m_baudRate) / 11.)) * m_current.adu.size());

            qCDebug(QT_MODBUS2) << "(RTU client) Sent Serial PDU:" << m_current.requestPdu;
            qCDebug(QT_MODBUS2_LOW).noquote() << "(RTU client) Sent Serial ADU: 0x" + m_current.adu
                .toHex();
        };

        switch (m_state) {
        case Schedule:
            m_current = QueueElement();
            if (!m_queue.isEmpty()) {
                m_current = m_queue.dequeue();
                if (m_current.reply) {
                    m_state = Send;
                    QTimer::singleShot(0, [writeAdu]() { writeAdu(); });
                } else {
                    QTimer::singleShot(0, [this]() { processQueue(); });
                }
            } else {
                m_state = Idle;
            }
            break;

        case Send:
            // send timeout will always happen
            if (m_current.reply.isNull()) {
                scheduleNextRequest();
            }
//            else if (m_current.bytesWritten < m_current.adu.size()) {
//                qCDebug(QT_MODBUS2) << "(RTU client) Send failed:" << m_current.requestPdu
//                                    << "byteWrite: " << m_current.bytesWritten
//                                    << "adu.Size:" << m_current.adu.size();

//                if (m_current.numberOfRetries <= 0) {
//                    if (m_current.reply) {
//                        m_current.reply->setError(QModbus2Device::TimeoutError,
//                            QModbus2Client::tr("Request timeout."));
//                    }
//                    scheduleNextRequest();
//                } else {
//                    m_serialPort->clear(QSerialPort::AllDirections);
//                    QTimer::singleShot(m_interFrameDelayMilliseconds, [writeAdu]() { writeAdu(); });
//                }
//            }
            else {
                qCDebug(QT_MODBUS2) << "(RTU client) Send successful:" << m_current.requestPdu;
                m_state = Receive;
                m_responseTimer.start(m_responseTimeoutDuration);
            }
            break;

        case Receive:
            // receive timeout will only happen after successful send
            qCDebug(QT_MODBUS2) << "(RTU client) Receive timeout:" << m_current.requestPdu;
            if (m_current.reply.isNull()) {
                scheduleNextRequest();
            } else if (m_current.numberOfRetries <= 0) {
                if (m_current.reply) {
                    m_current.reply->setError(QModbus2Device::TimeoutError,
                        QModbus2Client::tr("Response timeout."));
                }
                scheduleNextRequest();
            } else {
                m_state = Send;
                m_serialPort->clear(QSerialPort::AllDirections);
                QTimer::singleShot(m_interFrameDelayMilliseconds, [this, writeAdu]() { writeAdu(); });
            }
            break;

        case Idle:
        default:
            Q_ASSERT_X(false, "processQueue", QByteArray("unexpected state: ").append(m_state));
            break;
        }
    }

    bool canMatchRequestAndResponse(const QModbus2Response &response, int sendingServer) const
    {
        if (m_current.reply.isNull())
            return false;   // reply deleted
        if (m_current.reply->serverAddress() != sendingServer)
            return false;   // server mismatch
        //if (m_current.requestPdu.functionCode() != response.compFunctionCode())
        if (!(( (m_current.requestPdu.functionCode() + 1) == response.functionCode() ) ||
              (response.functionCode() == QModbus2Pdu::FunctionCode::QueryAlarmInfoCode)))
            return false;   // request for different function code
        return true;
    }

    bool isOpen() const override
    {
        if (m_serialPort)
            return m_serialPort->isOpen();
        return false;
    }

    QTimer m_sendTimer;
    QTimer m_responseTimer;

    QueueElement m_current;
    QByteArray responseBuffer;

    QQueue<QueueElement> m_queue;
    QSerialPort *m_serialPort = nullptr;

    int m_interFrameDelayMilliseconds = 2; // A approximated value of 1.750 msec.
};

QT_END_NAMESPACE

#endif // QMODBUSSERIALMASTER_P_H
