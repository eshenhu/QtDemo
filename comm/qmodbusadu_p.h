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

#ifndef QMODBUSADU_P_H
#define QMODBUSADU_P_H

#include "qmodbuspdu.h"

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

class QModbusSerialAdu
{
public:
    enum Type {
        Ascii,
        Rtu
    };

    inline QModbusSerialAdu(Type type, const QByteArray &data)
        : m_type(type), m_data(data)
    {
        if (m_type == Ascii)
            m_data = QByteArray::fromHex(m_data.mid(1, m_data.size() - 3));

        m_pduData = QByteArray::fromHex(m_data.mid(6, m_data.size() - 8));
    }

    inline int size() const {
        if (m_type == Ascii)
            return m_data.size() - 1; // one byte, LRC
        return m_data.size() - 2; // two bytes, CRC
    }
    inline QByteArray data() const { return m_data.left(size()); }

//    inline int rawSize() const { return m_rawData.size(); }
//    inline QByteArray rawData() const { return m_rawData; }

    inline int serverAddress() const {
        Q_ASSERT_X(!m_data.isEmpty(), "QModbusAdu::serverAddress()", "Empty ADU.");
        //return quint8(m_data.at(0));
        // big-endian little-endian problem here?
        return quint16(quint8(m_data.at(0)) << 8 | quint8(m_data.at(1)));
    }

    inline int pduDataSize() const
    {
        Q_ASSERT_X(!m_data.isEmpty(), "QModbusAdu::pdu()", "Empty ADU.");
        // big-endian little-endian problem here?
        return quint16(quint8(m_data.at(2)) << 8 | quint8(m_data.at(3)));
    }

    inline QModbus2Pdu pdu() const {
        Q_ASSERT_X(!m_data.isEmpty(), "QModbusAdu::pdu()", "Empty ADU.");
        return QModbus2Pdu(QModbus2Pdu::FunctionCode(m_data.at(6)), m_data.mid(7, size() - 8));
    }

    template <typename T>
    auto checksum() const -> decltype(T()) {
        Q_ASSERT_X(!m_data.isEmpty(), "QModbusAdu::checksum()", "Empty ADU.");
        if (m_type == Ascii)
            return quint8(m_data[m_data.size() - 1]);
        return quint16(quint8(m_data[m_data.size() - 2]) << 8 | quint8(m_data[m_data.size() - 1]));
    }

    inline bool matchingChecksum() const {
        Q_ASSERT_X(!m_data.isEmpty(), "QModbusAdu::matchingChecksum()", "Empty ADU.");
        if (m_type == Ascii)
            return QModbusSerialAdu::calculateLRC(data(), size()) == checksum<quint8>();
        //return QModbusSerialAdu::calculateCRC(data(), size()) == checksum<quint16>();
        return QModbusSerialAdu::calculateCRC(m_pduData, size()) == checksum<quint16>();
    }

    /*!
        \internal
        \fn quint8 QModbusSerialAdu::calculateLRC(const char *data, qint32 len)

        Returns the LRC checksum of the first \a len bytes of \a data. The checksum is independent
        of the byte order (endianness).
    */
    inline static quint8 calculateLRC(const char *data, qint32 len)
    {
        quint32 lrc = 0;
        while (len--)
            lrc += *data++;
        return -(quint8(lrc));
    }

    /*!
        \internal
        \fn quint16 QModbusSerialAdu::calculateCRC(const char *data, qint32 len) const

        Returns the CRC checksum of the first \a len bytes of \a data.

        \note The code used by the function was generated with pycrc. There is no copyright assigned
        to the generated code, however, the author of the script requests to show the line stating
        that the code was generated by pycrc (see implementation).
    */
    inline static quint16 calculateCRC(const char *data, qint32 len)
    {
        // Generated by pycrc v0.8.3, https://pycrc.org
        // Width = 16, Poly = 0x8005, XorIn = 0xffff, ReflectIn = True,
        // XorOut = 0x0000, ReflectOut = True, Algorithm = bit-by-bit-fast

        quint16 crc = 0xFFFF;
        while (len--) {
            const quint8 c = *data++;
            for (qint32 i = 0x01; i & 0xFF; i <<= 1) {
                bool bit = crc & 0x8000;
                if (c & i)
                    bit = !bit;
                crc <<= 1;
                if (bit)
                    crc ^= 0x8005;
            }
            crc &= 0xFFFF;
        }
        crc = crc_reflect(crc & 0xFFFF, 16) ^ 0x0000;
        return (crc >> 8) | (crc << 8); // swap bytes
    }

    // ADU Frame was created like this.
    // HEAD    (16bit)  0xF0CC
    // LENGTH  (16bit)  0x0200 (little-endian)
    // ~LENGHT (16bit)  0xFDFF
    // PDU
    inline static QByteArray create(Type type, int serverAddress, const QModbus2Pdu &pdu,
                                    char delimiter = '\n') {
        QByteArray result;
        QDataStream out(&result, QIODevice::WriteOnly);
        quint16 aduSize = pdu.size();
        aduSize = (aduSize & 0x00FF) << 8 | (aduSize & 0xFF00) >> 8;

        out << quint16(serverAddress) << (quint16)aduSize << (quint16)~aduSize << pdu;

        if (type == Ascii) {
            out << calculateLRC(result, result.size());
            return ":" + result.toHex() + "\r" + delimiter;
        } else {
            out << calculateCRC(result.mid(6), result.size() - 6);
        }
        return result;
    }

private:
    inline static quint16 crc_reflect(quint16 data, qint32 len)
    {
        // Generated by pycrc v0.8.3, https://pycrc.org
        // Width = 16, Poly = 0x8005, XorIn = 0xffff, ReflectIn = True,
        // XorOut = 0x0000, ReflectOut = True, Algorithm = bit-by-bit-fast

        quint16 ret = data & 0x01;
        for (qint32 i = 1; i < len; i++) {
            data >>= 1;
            ret = (ret << 1) | (data & 0x01);
        }
        return ret;
    }

private:
    Type m_type = Rtu;
    QByteArray m_data;
//    QByteArray m_rawData;
    QByteArray m_pduData;
};

QT_END_NAMESPACE

#endif // QMODBUSADU_P_H
