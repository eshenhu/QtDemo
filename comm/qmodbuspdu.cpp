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

#include "qmodbuspdu.h"
#include "qmodbus_symbols_p.h"

#include <QtCore/qdebug.h>
#include <QtCore/qhash.h>

QT_BEGIN_NAMESPACE

using ReqSizeCalc = QHash<quint8, QModbus2Request::CalcFuncPtr>;
Q_GLOBAL_STATIC(ReqSizeCalc, requestSizeCalculators)

using ResSizeCalc = QHash<quint8, QModbus2Response::CalcFuncPtr>;
Q_GLOBAL_STATIC(ResSizeCalc, responseSizeCalculators)

namespace Private {

enum struct Type {
    Request,
    Response
};

static int minimumDataSize(const QModbus2Pdu &pdu, Type type)
{
    if (pdu.isException())
        return 1;

    // 2 bytes HEAD
    // 2 bytes LENGHT
    // 2 bytes ~LENGTH
    // 1 byte  FunctionCode
    // 2 byte CRC
    //return 2 + 2 + 2 + 1 + 2;
    return 2;

//    switch (pdu.functionCode()) {
//    case QModbus2Pdu::ReadCoils:
//    case QModbus2Pdu::ReadDiscreteInputs:
//        return type == Type::Request ? 4 : 2;
//    case QModbus2Pdu::WriteSingleCoil:
//    case QModbus2Pdu::WriteSingleRegister:
//        return 4;
//    case QModbus2Pdu::ReadHoldingRegisters:
//    case QModbus2Pdu::ReadInputRegisters:
//        return type == Type::Request ? 4 : 3;
//    case QModbus2Pdu::ReadExceptionStatus:
//        return type == Type::Request ? 0 : 1;
//    case QModbus2Pdu::Diagnostics:
//        return 4;
//    case QModbus2Pdu::GetCommEventCounter:
//        return type == Type::Request ? 0 : 4;
//    case QModbus2Pdu::GetCommEventLog:
//        return type == Type::Request ? 0 : 8;
//    case QModbus2Pdu::WriteMultipleCoils:
//        return type == Type::Request ? 6 : 4;
//    case QModbus2Pdu::WriteMultipleRegisters:
//        return type == Type::Request ? 7 : 4;
//    case QModbus2Pdu::ReportServerId:
//        return type == Type::Request ? 0 : 3;
//    case QModbus2Pdu::ReadFileRecord:
//        return type == Type::Request ? 8 : 5;
//    case QModbus2Pdu::WriteFileRecord:
//        return 10;
//    case QModbus2Pdu::MaskWriteRegister:
//        return 6;
//    case QModbus2Pdu::ReadWriteMultipleRegisters:
//        return type == Type::Request ? 11 : 3;
//    case QModbus2Pdu::ReadFifoQueue:
//        return type == Type::Request ? 2 : 6;
//    case QModbus2Pdu::EncapsulatedInterfaceTransport:
//        return 2;
//    case QModbus2Pdu::Invalid:
//    case QModbus2Pdu::UndefinedFunctionCode:
//        return -1;
//    }
//    return -1;
}

//static QDataStream &pduFromStream(QDataStream &stream, QModbus2Pdu &pdu, Type type)
//{
//    QModbus2Pdu::FunctionCode code = QModbus2Pdu::Invalid;
//    if (stream.readRawData((char *) (&code), sizeof(quint8)) != sizeof(quint8))
//        return stream;
//    pdu.setFunctionCode(code);

//    auto needsAdditionalRead = [](QModbus2Pdu &pdu, int size) -> bool {
//        if (size < 0)
//            pdu.setFunctionCode(QModbus2Response::Invalid);
//        if (size <= 0)
//            return false;
//        return true;
//    };

//    const bool isResponse = (type == Type::Response);
//    int size = isResponse ? QModbus2Response::minimumDataSize(pdu)
//                          : QModbus2Request::minimumDataSize(pdu);
//    if (!needsAdditionalRead(pdu, size))
//        return stream;

//    QByteArray data(size, Qt::Uninitialized);
//    if (stream.device()->peek(data.data(), data.size()) != size)
//        return stream;

//    pdu.setData(data);
//    size = isResponse ? QModbus2Response::calculateDataSize(pdu)
//                      : QModbus2Request::calculateDataSize(pdu);
//    if (!needsAdditionalRead(pdu, size))
//        return stream;

//    if (isResponse && (code == QModbus2Pdu::EncapsulatedInterfaceTransport)) {
//        quint8 meiType;
//        pdu.decodeData(&meiType);
//        if (meiType == EncapsulatedInterfaceTransport::ReadDeviceIdentification) {
//            int left = size, offset = 0;
//            while ((left > 0) && (size <= 252)) { // The maximum PDU data size is 252 bytes.
//                data.resize(size);
//                const int read = stream.readRawData(data.data() + offset, size - offset);
//                if ((read < 0) || (read != (size - offset))) {
//                    size = 255; // bogus size
//                    stream.setStatus(QDataStream::ReadCorruptData);
//                    break; // error reading, bail, reset further down
//                }
//                offset += read;
//                left = QModbus2Response::calculateDataSize(QModbus2Response(code, data)) - offset;
//                size += left;
//            }
//            if ((stream.status() == QDataStream::Ok) && (size <= 252)) {
//                pdu.setData(data);
//                return stream; // early return to avoid second read
//            }
//        } else {
//            data.resize(stream.device()->size() - 1); // One byte for the function code.
//        }
//    } else if (pdu.functionCode() == QModbus2Pdu::Diagnostics) {
//        quint16 subCode;
//        pdu.decodeData(&subCode);
//        if (subCode == Diagnostics::ReturnQueryData)
//            data.resize(stream.device()->size() - 1); // One byte for the function code.
//    }

//    // reset what we have so far, next read might fail as well
//    pdu.setData(QByteArray());
//    pdu.setFunctionCode(QModbus2Pdu::Invalid);

//    if (data.size() <= 252) { // The maximum PDU data size is 252 bytes.
//        data.resize(size);
//        if (stream.readRawData(data.data(), data.size()) == size) {
//            pdu.setData(data);
//            pdu.setFunctionCode(code);
//        }
//    }
//    return stream;
//}

}   // namespace Private

/*!
    \class QModbus2Pdu
    \inmodule QtSerialBus
    \since 5.6

    \brief QModbus2Pdu is a abstract container class containing the function code and
    payload that is stored inside a Modbus ADU.

    The class provides access to the raw Modbus protocol packets as defined by
    the  Modbus Application Protocol Specification 1.1b.
*/

/*!
    \enum QModbus2Pdu::ExceptionCode

    This enum describes all the possible error conditions as defined by Modbus Exception Codes.
    They are set by the server after checking the appropriate error conditions in the reply to a
    request and must be decoded by the client to operate on the exception code.

    \value IllegalFunction                      Function code is not supported by device.
    \value IllegalDataAddress                   The received data address in the query is not an
                                                allowable address for the Modbus server.
    \value IllegalDataValue                     The contained value in the request data field is
                                                not an allowable value for the Modbus server.
    \value ServerDeviceFailure                  An irrecoverable error occurred while the server
                                                was attempting to perform the requested action.
    \value Acknowledge                          Specialized use in conjunction with programming
                                                commands.
    \value ServerDeviceBusy                     The server is engaged in processing a long duration
                                                program command.
    \value NegativeAcknowledge                  The server cannot perform the program function
                                                received in the query. This code is returned for an
                                                unsuccessful programming request. The client should
                                                request diagnostic or error information from the
                                                server.
    \value MemoryParityError                    Indicates that the extended file area failed to
                                                pass a consistency check. Used in conjunction with
                                                function codes 20 and 21. The exception code does
                                                not refer to any parity settings of the
                                                transmission line but only to the servers' internal
                                                memory of file records.
    \value GatewayPathUnavailable               Indicates that the gateway was unable to allocate
                                                an internal communication path from the input port
                                                to the output port for processing the request.
    \value GatewayTargetDeviceFailedToRespond   Indicates that no response was obtained from the
                                                target device behind a gateway.
                                                Usually this means the target device is not online
                                                on the network.
    \value ExtendedException                    This is an extended exception as per Modbus
                                                specification. Generally this code is used to
                                                describe an exception that is otherwise further
                                                described.
*/

/*!
    \enum QModbus2Pdu::FunctionCode

    Defines the function code and the implicit type of action required by the server. Not all
    Modbus devices can handle the same set of function codes.

    \value Invalid                          Set by the default constructor, do not use.
    \value ReadCoils                        Requests the status of one or more coils from a device.
    \value ReadDiscreteInputs               Requests the status of one or more input registers from
                                            a device.
    \value ReadHoldingRegisters             Requests the status of one or more holding register
                                            values from a device.
    \value ReadInputRegisters               Requests the status of one or more input register
                                            values from a device.
    \value WriteSingleCoil                  Requests to write a single coil on a device.
    \value WriteSingleRegister              Requests to write a single holding register on a device.
    \value ReadExceptionStatus              Requests the status of the eight Exception Status
                                            outputs on a device.
    \value Diagnostics                      Used to provide a series of tests for checking the
                                            client server communication system, or checking internal
    \value GetCommEventCounter              Requests a status word and an event count from the
                                            device's communication event counter.
    \value GetCommEventLog                  Requests a status word, event count, message count,
                                            and a field of event bytes from a device.
    \value WriteMultipleCoils               Requests to write one or more coils on a device.
    \value WriteMultipleRegisters           Requests to write one or more holding registers on a
                                            device.
    \value ReportServerId                   Requests the description of the type, the current
                                            status, and other information specific to a device.
    \value ReadFileRecord                   Requests a file record read.
    \value WriteFileRecord                  Requests a file record write.
    \value MaskWriteRegister                Requests to modify the contents of a specified holding
                                            register using a combination of an AND or OR mask, and
                                            the register's current contents.
    \value ReadWriteMultipleRegisters       Requests the status of one or more holding register and
                                            at the same time to write one or more holding registers
                                            on a device.
    \value ReadFifoQueue                    Requests to read the contents of a First-In-First-Out
                                            (FIFO) queue of register in a remote device.
    \value EncapsulatedInterfaceTransport   Please refer to Annex A of the Modbus specification.
    \value UndefinedFunctionCode            Do not use.
*/

/*!
    \fn QModbus2Pdu::QModbus2Pdu()

    Constructs an invalid QModbus2Pdu.
*/

/*!
    \fn QModbus2Pdu::~QModbus2Pdu()

    Destroys a QModbus2Pdu.
*/

/*!
    \fn QModbus2Pdu::QModbus2Pdu(const QModbus2Pdu &other)

    Constructs a QModbus2Pdu that is a copy of \a other.

*/

/*!
    \fn QModbus2Pdu &QModbus2Pdu::operator=(const QModbus2Pdu &other)

    Makes a copy of the \a other and assigns it to this QModbus2Pdu object.
*/

/*!
    \fn QModbus2Pdu::QModbus2Pdu(FunctionCode code, const QByteArray &data)

    Constructs a QModbus2Pdu with function code set to \a code and payload set to \a data.
    The data is expected to be stored in big-endian byte order already.
*/

/*!
    \internal
    \fn QModbus2Pdu::QModbus2Pdu(FunctionCode code, Args... data)

    Constructs a QModbus2Pdu with function code set to \a code and payload set to \a data.
    The data is converted and stored in big-endian byte order.

    \note Usage is limited \c quint8 and \c quint16 only. This is because
    \c QDataStream stream operators will not only append raw data, but also
    e.g. size, count etc. for complex types.
*/

/*!
    \fn bool QModbus2Pdu::isValid() const

    Returns true if the PDU is valid; otherwise false.

    A PDU is considered valid if the message code is in the range of 1 to 255 decimal and the
    PDU's compound size (function code + data) does not exceed 253 bytes. A default constructed
    PDU is invalid.
*/

/*!
    \variable QModbus2Pdu::ExceptionByte

    The variable is initialized to 0x80.

    Exceptions are reported in a defined packet format. A function code
    is returned to the requesting client equal to the original function code,
    except with its most significant bit set. This is equivalent to adding 0x80
    to the value of the original function code.

    This field may be used to mask the exception bit in the function field
    of a raw Modbus packet.
*/

/*!
    \fn bool QModbus2Pdu::isException() const

    Returns true if the PDU contains an exception code; otherwise false.
*/

/*!
    \fn QModbus2Pdu::ExceptionCode QModbus2Pdu::exceptionCode() const

    Returns the response's exception code.
*/

/*!
    \fn qint16 QModbus2Pdu::size() const

    Returns the PDU's full size, including function code and data size.
*/

/*!
    \fn qint16 QModbus2Pdu::dataSize() const

    Returns the PDU's data size, excluding the function code.
*/

/*!
    \fn FunctionCode QModbus2Pdu::functionCode() const

    Returns the PDU's function code.
*/

/*!
    \fn void QModbus2Pdu::setFunctionCode(FunctionCode code)

    Sets the PDU's function code to \a code.
*/

/*!
    \fn QByteArray QModbus2Pdu::data() const

    Returns the PDU's payload, excluding the function code.
    The payload is stored in big-endian byte order.
*/

/*!
    \fn void QModbus2Pdu::setData(const QByteArray &data)

    Sets the PDU's function payload to \a data. The data is expected to be stored in big-endian
    byte order already.
*/

/*!
    \fn void QModbus2Pdu::decodeData(Args && ... data) const

    Converts the payload into host endianness and reads it into \a data. Data can be a variable
    length argument list.

    \code
        QModbusResponsePdu response(QModbus2Pdu::ReportServerId);
        response.encodeData(quint8(0x02), quint8(0x01), quint8(0xff));
        quint8 count, id, run;
        response.decodeData(&count, &id, &run);
    \endcode

    \note Usage is limited \c quint8 and \c quint16 only. This is because
    \c QDataStream stream operators will not only append raw data, but also
    e.g. size, count etc. for complex types.
*/

/*!
    \fn void QModbus2Pdu::encodeData(Args ... data)

    Sets the payload to \a data. The data is converted and stored in big-endian byte order.

    \code
        QModbusRequestPdu request(QModbus2Pdu::ReadCoils);
        // starting address and quantity of coils
        request.encodeData(quint16(0x0c), quint16(0x0a));
    \endcode

    \note Usage is limited \c quint8 and \c quint16 only. This is because
    \c QDataStream stream operators will not only append raw data, but also
    e.g. size, count etc. for complex types.
*/

/*!
    \relates QModbus2Pdu

    Writes the Modbus \a pdu to the \a debug stream.
*/
QDebug operator<<(QDebug debug, const QModbus2Pdu &pdu)
{
    QDebugStateSaver _(debug);
    debug.nospace().noquote() << "0x" << hex << qSetFieldWidth(2) << qSetPadChar('0')
        << (pdu.isException() ? pdu.functionCode() | QModbus2Pdu::ExceptionByte : pdu.functionCode())
        << qSetFieldWidth(0) << pdu.data().toHex();
    return debug;
}

/*!
    \relates QModbus2Pdu

    Writes a \a pdu to the \a stream and returns a reference to the stream.
*/
QDataStream &operator<<(QDataStream &stream, const QModbus2Pdu &pdu)
{
    if (pdu.isException())
        stream << static_cast<quint8> (pdu.functionCode() | QModbus2Pdu::ExceptionByte);
    else
        stream << static_cast<quint8> (pdu.functionCode());
    if (!pdu.data().isEmpty())
        stream.writeRawData(pdu.data().constData(), pdu.data().size());

    return stream;
}

/*!
    \class QModbus2Request
    \inmodule QtSerialBus
    \since 5.6

    \brief QModbus2Request is a container class containing the function code and payload that is
    stored inside a Modbus ADU.

    A Modbus request usually consists of a single byte describing the \c FunctionCode and N bytes
    of payload

    A typical Modbus request can looks like this:
    \code
        QModbus2Request request(QModbus2Request::WriteMultipleCoils,
            QByteArray::fromHex("0013000a02cd01"));
    \endcode
    \note When using the constructor taking the \c QByteArray, please make sure to convert the
        containing data to big-endian byte order before creating the request.

    The same request can be created like this, if the values are know at compile time:
    \code
        quint16 startAddress = 19, numberOfCoils = 10;
        quint8 payloadInBytes = 2, outputHigh = 0xcd, outputLow = 0x01;
        QModbus2Request request(QModbus2Request::WriteMultipleCoils, startAddress, numberOfCoils,
            payloadInBytes, outputHigh, outputLow);
    \endcode
*/

/*!
    \fn QModbus2Request::QModbus2Request()

    Constructs an invalid QModbus2Request.
*/

/*!
    \fn QModbus2Request::QModbus2Request(const QModbus2Pdu &pdu)

    Constructs a copy of \a pdu.
*/

/*!
    \fn QModbus2Request::QModbus2Request(FunctionCode code, Args... data)

    Constructs a QModbus2Request with function code set to \a code and payload set to \a data.
    The data is converted and stored in big-endian byte order.

    \note Usage is limited \c quint8 and \c quint16 only. This is because
    \c QDataStream stream operators will not only append raw data, but also
    e.g. size, count etc. for complex types.
*/

/*!
    \fn QModbus2Request::QModbus2Request(FunctionCode code, const QByteArray &data = QByteArray())

    Constructs a QModbus2Response with function code set to \a code and payload set to \a data.
    The data is expected to be stored in big-endian byte order already.
*/

/*!
    Returns the expected minimum data size for \a request based on the
    request's function code; \c {-1} if the function code is not known.
*/
int QModbus2Request::minimumDataSize(const QModbus2Request &request)
{
    return Private::minimumDataSize(request, Private::Type::Request);
}

/*!
    Calculates the expected data size for \a request based on the request's
    function code and data. Returns the full size of the request's data part;
    \c {-1} if the size could not be properly calculated.

    \sa minimumDataSize
    \sa registerDataSizeCalculator
*/
int QModbus2Request::calculateDataSize(const QModbus2Request &request)
{
    if (requestSizeCalculators.exists()) {
        if (auto ptr = requestSizeCalculators()->value(request.functionCode(), nullptr))
            return ptr(request);
    }

    if (request.isException())
        return 1;

    int size = -1;
    int minimum = Private::minimumDataSize(request, Private::Type::Request);
    if (minimum < 0)
        return size;

    return request.size();

//    switch (request.functionCode()) {
//    case QModbus2Pdu::WriteMultipleCoils:
//        minimum -= 1; // first payload payload byte
//        if (request.dataSize() >= minimum)
//            size = minimum + request.data()[minimum - 1] /*byte count*/;
//        break;
//    case QModbus2Pdu::WriteMultipleRegisters:
//    case QModbus2Pdu::ReadWriteMultipleRegisters:
//        minimum -= 2; // first 2 payload payload bytes
//        if (request.dataSize() >= minimum)
//            size = minimum + request.data()[minimum - 1] /*byte count*/;
//        break;
//    case QModbus2Pdu::ReadFileRecord:
//    case QModbus2Pdu::WriteFileRecord:
//        if (request.dataSize() >= 1)
//            size = 1 /*byte count*/ + request.data()[0] /*actual bytes*/;
//        break;
//    case QModbus2Pdu::EncapsulatedInterfaceTransport: {
//        if (request.dataSize() < minimum)
//            break;  // can't calculate, let's return -1 to indicate error
//        quint8 meiType;
//        request.decodeData(&meiType);
//        // ReadDeviceIdentification -> 3 == MEI type + Read device ID + Object Id
//        size = (meiType == EncapsulatedInterfaceTransport::ReadDeviceIdentification) ? 3 : minimum;
//    }   break;
//    default:
//        size = minimum;
//        break;
//    }
//    return size;
}

/*!
    This function registers a user-defined implementation to calculate the
    request data size for function code \a fc. It can be used to extend or
    override the implementation inside \l QModbus2Request::calculateDataSize().

    The \c CalcFuncPtr is a typedef for a pointer to a custom \a calculator
    function with the following signature:
    \code
        int myCalculateDataSize(const QModbus2Request &pdu);
    \endcode
*/
void QModbus2Request::registerDataSizeCalculator(FunctionCode fc, CalcFuncPtr calculator)
{
    requestSizeCalculators()->insert(fc, calculator);
}

/*!
    \relates QModbus2Request

    Reads a \a pdu from the \a stream and returns a reference to the stream.

    \note The function might fail to properly stream PDU's with function code
    \l QModbus2Pdu::Diagnostics or \l QModbus2Pdu::EncapsulatedInterfaceTransport
    because of the missing size indicator inside the PDU. In particular this may
    happen when the PDU is embedded into a stream that doesn't end with the
    diagnostic/encapsulated request itself.
*/
//QDataStream &operator>>(QDataStream &stream, QModbus2Request &pdu)
//{
//    return Private::pduFromStream(stream, pdu, Private::Type::Request);
//}

/*!
    \class QModbus2Response
    \inmodule QtSerialBus
    \since 5.6

    \brief QModbus2Response is a container class containing the function code and payload that is
        stored inside a Modbus ADU.

    A typical Modbus response can looks like this:
    \code
        QModbus2Response response(QModbus2Response::ReadCoils, QByteArray::fromHex("02cd01"));
    \endcode
    \note When using the constructor taking the \c QByteArray, please make sure to convert the
        containing data to big-endian byte order before creating the request.

    The same response can be created like this, if the values are know at compile time:
    \code
        quint8 payloadInBytes = 2, outputHigh = 0xcd, outputLow = 0x01;
        QModbus2Response response(QModbus2Response::ReadCoils, payloadInBytes, outputHigh, outputLow);
    \endcode
*/

/*!
    \fn QModbus2Response::QModbus2Response()

    Constructs an invalid QModbus2Response.
*/

/*!
    \fn QModbus2Response::QModbus2Response(const QModbus2Pdu &pdu)

    Constructs a copy of \a pdu.
*/

/*!
    \fn QModbus2Response::QModbus2Response(FunctionCode code, Args... data)

    Constructs a QModbus2Response with function code set to \a code and payload set to \a data.
    The data is converted and stored in big-endian byte order.

    \note Usage is limited \c quint8 and \c quint16 only. This is because
    \c QDataStream stream operators will not only append raw data, but also
    e.g. size, count etc. for complex types.
*/

/*!
    \fn QModbus2Response::QModbus2Response(FunctionCode code, const QByteArray &data = QByteArray())

    Constructs a QModbus2Response with function code set to \a code and payload set to \a data.
    The data is expected to be stored in big-endian byte order already.
*/

/*!
    Returns the expected minimum data size for \a response based on the
    response's function code; \c {-1} if the function code is not known.
*/
int QModbus2Response::minimumDataSize(const QModbus2Response &response)
{
    return Private::minimumDataSize(response, Private::Type::Response);
}


int QModbus2Response::getMinDataSize()
{
    return 2 + 2 + 2 + 1 + 2;
}

/*!
    Calculates the expected data size for \a response, based on the response's
    function code and data. Returns the full size of the response's data part;
    \c {-1} if the size could not be properly calculated.

    \sa minimumDataSize
    \sa registerDataSizeCalculator
*/
int QModbus2Response::calculateDataSize(const QModbus2Response &response)
{
    if (responseSizeCalculators.exists()) {
        if (auto ptr = responseSizeCalculators()->value(response.functionCode(), nullptr))
            return ptr(response);
    }

    if (response.isException())
        return 1;

    int size = -1;
    int minimum = Private::minimumDataSize(response, Private::Type::Response);
    if (minimum < 0)
        return size;


//    switch (response.functionCode()) {
//    case QModbus2Response::ReadCoils:
//    case QModbus2Response::ReadDiscreteInputs:
//    case QModbus2Response::ReadHoldingRegisters:
//    case QModbus2Response::ReadInputRegisters:
//    case QModbus2Response::GetCommEventLog:
//    case QModbus2Response::ReadFileRecord:
//    case QModbus2Response::WriteFileRecord:
//    case QModbus2Response::ReadWriteMultipleRegisters:
//    case QModbus2Response::ReportServerId:
//        if (response.dataSize() >= 1)
//            size = 1 /*byte count*/ + response.data()[0] /*actual bytes*/;
//        break;
//    case QModbus2Response::ReadFifoQueue: {
//        if (response.dataSize() >= 2) {
//            quint16 rawSize;
//            response.decodeData(&rawSize);
//            size = rawSize + 2; // 2 bytes size info
//        }
//    }   break;
//    case QModbus2Pdu::EncapsulatedInterfaceTransport: {
//        if (response.dataSize() < minimum)
//            break;  // can't calculate, let's return -1 to indicate error

//        quint8 meiType;
//        response.decodeData(&meiType);

//        // update size, header 6 bytes: mei type + read device id + conformity level + more follows
//        //                              + next object id + number of object
//        // response data part  2 bytes: + object id + object size of the first object -> 8
//        size = (meiType == EncapsulatedInterfaceTransport::ReadDeviceIdentification) ? 8 : minimum;
//        if (meiType != EncapsulatedInterfaceTransport::ReadDeviceIdentification
//            || response.dataSize() < size) {
//            break; // TODO: calculate CanOpenGeneralReference instead of break
//        }

//        const QByteArray data = response.data();
//        quint8 numOfObjects = quint8(data[5]);
//        quint8 objectSize = quint8(data[7]);

//        // 6 byte header size + (2 * n bytes fixed per object) + first object size
//        size = 6 + (2 * numOfObjects) + objectSize;
//        if ((numOfObjects == 1) || (data.size() < size))
//            break;

//        // header + object id + object size + second object id (9 bytes) + first object size
//        int nextSizeField = 9 + objectSize;
//        for (int i = 1; i < numOfObjects; ++i) {
//            if (data.size() <= nextSizeField)
//                break;
//            objectSize = data[nextSizeField];
//            size += objectSize;
//            nextSizeField += objectSize + 2; // object size + object id field + object size field
//        }
//    }   break;
//    default:
//        size = minimum;
//        break;
//    }


//    return size;
}

/*!
    This function registers a user-defined implementation to calculate the
    response data size for function code \a fc. It can be used to extend or
    override the implementation inside \l QModbus2Response::calculateDataSize().

    The \c CalcFuncPtr is a typedef for a pointer to a custom \a calculator
    function with the following signature:
    \code
        int myCalculateDataSize(const QModbus2Response &pdu);
    \endcode
*/
void QModbus2Response::registerDataSizeCalculator(FunctionCode fc, CalcFuncPtr calculator)
{
    responseSizeCalculators()->insert(fc, calculator);
}

/*!
    \relates QModbus2Response

    Reads a \a pdu from the \a stream and returns a reference to the stream.

    \note The function might fail to properly stream PDU's with function code
    \l QModbus2Pdu::Diagnostics or \l QModbus2Pdu::EncapsulatedInterfaceTransport
    because of the missing size indicator inside the PDU. In particular this may
    happen when the PDU is embedded into a stream that doesn't end with the
    diagnostic/encapsulated request itself.
*/
//QDataStream &operator>>(QDataStream &stream, QModbus2Response &pdu)
//{
//    return Private::pduFromStream(stream, pdu, Private::Type::Response);
//}

/*!
    \class QModbusExceptionResponse
    \inmodule QtSerialBus
    \since 5.6

    \brief QModbusExceptionResponse is a container class containing the function and error code
        inside a Modbus ADU.

    A typical QModbusExceptionResponse response can looks like this:
    \code
        QModbusExceptionResponse exception(QModbusExceptionResponse::ReportServerId,
            QModbusExceptionResponse::ServerDeviceFailure);
    \endcode
*/

/*!
    \fn QModbusExceptionResponse::QModbusExceptionResponse()

    Constructs an invalid QModbusExceptionResponse.
*/

/*!
    \fn QModbusExceptionResponse::QModbusExceptionResponse(const QModbus2Pdu &pdu)

    Constructs a copy of \a pdu.
*/

/*!
    \fn QModbusExceptionResponse::QModbusExceptionResponse(FunctionCode code, ExceptionCode ec)

    Constructs a QModbusExceptionResponse with function code set to \a code and exception error
    code set to \a ec.
*/

/*!
    \fn void QModbusExceptionResponse::setFunctionCode(FunctionCode c)

    Sets the response's function code to \a c.
*/

/*!
    \fn void QModbusExceptionResponse::setExceptionCode(ExceptionCode ec)

    Sets the response's exception code to \a ec.
*/

QT_END_NAMESPACE
