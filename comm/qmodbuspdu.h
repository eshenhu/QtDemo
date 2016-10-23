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
#ifndef QMODBUSPDU_H
#define QMODBUSPDU_H

#include <QtCore/qdatastream.h>
#include <QtCore/qmetatype.h>
#include <QtCore/qvector.h>
#include "qserialbusglobal.h"

QT_BEGIN_NAMESPACE

class QModbus2Pdu
{
public:
    enum ExceptionCode {
        IllegalFunction = 0x01,
        IllegalDataAddress = 0x02,
        IllegalDataValue = 0x03,
        ServerDeviceFailure = 0x04,
        Acknowledge = 0x05,
        ServerDeviceBusy = 0x06,
        NegativeAcknowledge = 0x07,
        MemoryParityError = 0x08,
        GatewayPathUnavailable = 0x0A,
        GatewayTargetDeviceFailedToRespond = 0x0B,
        ExtendedException = 0xFF,
    };

    enum FunctionCode {
        Invalid = 0x00,
        ResetCode = 0x01,
        HandShakeCode = 0x02,
        FreqAdjustCode = 0x03,
        AlarmInfoCode = 0x05,
        MeasStartCode = 0x07,
//        MeasEndCode = 0x08,
        ManualMeasStartCode = 0x09,
        QueryAlarmInfoCode = 0x0b
//        Invalid = 0x00,
//        ReadCoils = 0x01,
//        ReadDiscreteInputs = 0x02,
//        ReadHoldingRegisters = 0x03,
//        ReadInputRegisters = 0x04,
//        WriteSingleCoil = 0x05,
    };

    QModbus2Pdu() = default;
    virtual ~QModbus2Pdu() = default;

    bool isValid() const {
        return (m_data.size() < 253);
    }

    static const quint8 ExceptionByte = 0x00;
//    ExceptionCode exceptionCode() const {
//        if (!m_data.size() || !isException())
//            return ExtendedException;
//        return static_cast<ExceptionCode>(m_data.at(0));
//    }
    //bool isException() const { return m_code & ExceptionByte; }
    bool isException() const { return false; }

    qint16 size() const { return dataSize() + 1; }
    qint16 dataSize() const { return m_data.size(); }

    FunctionCode functionCode() const {
        return FunctionCode(quint8(m_code) &~ ExceptionByte);
    }

    FunctionCode compFunctionCode() const {
        return FunctionCode(quint8(~m_code) &~ ExceptionByte);
    }

    virtual void setFunctionCode(FunctionCode code) { m_code = code; }

    QByteArray data() const { return m_data; }
    void setData(const QByteArray &newData) { m_data = newData; }

    template <typename ... Args> void encodeData(Args ... newData) {
        encode(std::forward<Args>(newData)...);
    }

    template <typename ... Args> void decodeData(Args && ... newData) const {
        decode(std::forward<Args>(newData)...);
    }

protected:
    QModbus2Pdu(FunctionCode code, const QByteArray &newData)
        : m_code(code)
        , m_data(newData)
    {}

    QModbus2Pdu(const QModbus2Pdu &) = default;
    QModbus2Pdu &operator=(const QModbus2Pdu &) = default;

    template <typename ... Args>
    QModbus2Pdu(FunctionCode code, Args ... newData)
        : m_code(code)
    {
        encode(std::forward<Args>(newData)...);
    }

private:
    template <typename T, typename ... Ts> struct IsType { enum { value = false }; };
    template <typename T, typename T1, typename ... Ts> struct IsType<T, T1, Ts...> {
        enum { value = std::is_same<T, T1>::value || IsType<T, Ts...>::value };
    };

    template <typename T> void encode(QDataStream *stream, const T &t) {
        static_assert(std::is_pod<T>::value, "Only POD types supported.");
        static_assert(IsType<T, quint8, quint16, quint32>::value, "Only quint8 and quint16 and quint32 supported.");
        (*stream) << t;
    }
    template <typename T> void decode(QDataStream *stream, T &t) const {
        static_assert(std::is_pod<T>::value, "Only POD types supported.");
        static_assert(IsType<T, quint8 *, quint16 *, quint32>::value, "Only quint8* and quint16* and quint32* supported.");
        (*stream) >> *t;
    }
    template <typename T> void encode(QDataStream *stream, const QVector<T> &vector) {
        static_assert(std::is_pod<T>::value, "Only POD types supported.");
        static_assert(IsType<T, quint8, quint16, quint32>::value, "Only quint8 and quint16 and quint32 supported.");
        for (int i = 0; i < vector.count(); ++i)
            (*stream) << vector[i];
    }

    template<typename ... Args> void encode(Args ... newData) {
        m_data.clear();
        Q_CONSTEXPR quint32 argCount = sizeof...(Args);
        if (argCount > 0) {
            QDataStream stream(&m_data, QIODevice::WriteOnly);
            char tmp[argCount] = { (encode(&stream, newData), void(), '0')... };
            Q_UNUSED(tmp)
        }
    }
    template<typename ... Args> void decode(Args ... newData) const {
        Q_CONSTEXPR quint32 argCount = sizeof...(Args);
        if (argCount > 0 && !m_data.isEmpty()) {
            QDataStream stream(m_data);
            char tmp[argCount] = { (decode(&stream, newData), void(), '0')... };
            Q_UNUSED(tmp)
        }
    }

private:
    FunctionCode m_code = Invalid;
    QByteArray m_data;
    friend class QModbusSerialAdu;
};
Q_SERIALBUS_EXPORT QDebug operator<<(QDebug debug, const QModbus2Pdu &pdu);
Q_SERIALBUS_EXPORT QDataStream &operator<<(QDataStream &stream, const QModbus2Pdu &pdu);

class QModbus2Request : public QModbus2Pdu
{
public:
    QModbus2Request() = default;
    QModbus2Request(const QModbus2Pdu &pdu)
        : QModbus2Pdu(pdu)
    {}

    explicit QModbus2Request(FunctionCode code, const QByteArray &newData = QByteArray())
        : QModbus2Pdu(code, newData)
    {}

    Q_SERIALBUS_EXPORT static int minimumDataSize(const QModbus2Request &pdu);
    Q_SERIALBUS_EXPORT static int calculateDataSize(const QModbus2Request &pdu);

    using CalcFuncPtr = decltype(&calculateDataSize);
    Q_SERIALBUS_EXPORT static void registerDataSizeCalculator(FunctionCode fc, CalcFuncPtr func);

    template <typename ... Args>
    QModbus2Request(FunctionCode code, Args ... newData)
        : QModbus2Pdu(code, newData...)
    {}
};
Q_SERIALBUS_EXPORT QDataStream &operator>>(QDataStream &stream, QModbus2Request &pdu);

class QModbus2Response : public QModbus2Pdu
{
public:
    QModbus2Response() = default;
    QModbus2Response(const QModbus2Pdu &pdu)
        : QModbus2Pdu(pdu)
    {}

    explicit QModbus2Response(FunctionCode code, const QByteArray &newData = QByteArray())
        : QModbus2Pdu(code, newData)
    {}

    Q_SERIALBUS_EXPORT static int minimumDataSize(const QModbus2Response &pdu);
    Q_SERIALBUS_EXPORT static int calculateDataSize(const QModbus2Response &pdu);
    Q_SERIALBUS_EXPORT static int getMinDataSize();
    Q_SERIALBUS_EXPORT static int validateAduDataSize(const QModbus2Response &pdu);

    using CalcFuncPtr = decltype(&calculateDataSize);
    Q_SERIALBUS_EXPORT static void registerDataSizeCalculator(FunctionCode fc, CalcFuncPtr func);

    template <typename ... Args>
    QModbus2Response(FunctionCode code, Args ... newData)
        : QModbus2Pdu(code, newData...)
    {}
};

class QModbus2ExceptionResponse : public QModbus2Response
{
public:
    QModbus2ExceptionResponse() = default;
    QModbus2ExceptionResponse(const QModbus2Pdu &pdu)
        : QModbus2Response(pdu)
    {}
    QModbus2ExceptionResponse(FunctionCode fc, ExceptionCode ec)
        : QModbus2Response(FunctionCode(quint8(fc) | ExceptionByte), static_cast<quint8> (ec))
    {}

    void setFunctionCode(FunctionCode c) {
        QModbus2Pdu::setFunctionCode(FunctionCode(quint8(c) | ExceptionByte));
    }
    void setExceptionCode(ExceptionCode ec) { QModbus2Pdu::encodeData(quint8(ec)); }
};
Q_SERIALBUS_EXPORT QDataStream &operator>>(QDataStream &stream, QModbus2Response &pdu);

Q_DECLARE_TYPEINFO(QModbus2Pdu, Q_MOVABLE_TYPE);
Q_DECLARE_TYPEINFO(QModbus2Pdu::ExceptionCode, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QModbus2Pdu::FunctionCode, Q_PRIMITIVE_TYPE);

Q_DECLARE_TYPEINFO(QModbus2Request, Q_MOVABLE_TYPE);
Q_DECLARE_TYPEINFO(QModbus2Response, Q_MOVABLE_TYPE);
Q_DECLARE_TYPEINFO(QModbus2ExceptionResponse, Q_MOVABLE_TYPE);

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QModbus2Pdu::ExceptionCode)
Q_DECLARE_METATYPE(QModbus2Pdu::FunctionCode)

#endif // QMODBUSPDU_H
