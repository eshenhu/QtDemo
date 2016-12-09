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

#ifndef QMODBUSDATAUNIT_H
#define QMODBUSDATAUNIT_H

#include <QtCore/qmap.h>
#include <QtCore/qmetatype.h>
#include <QtCore/qvector.h>

QT_BEGIN_NAMESPACE

class QModbus2DataUnit
{
public:
    enum RegisterType {
        Invalid = 0x00,
        HandShakeCode = 0x01,
        FreqAdjustCode = 0x03,
        AlarmInfoCode = 0x05,
        MeasStartCode = 0x07,
        ResetCode = 0x09,
        FatalErrorInfoCode = 0x0b
    };

    enum class FatalErrorCodeE : quint8{
        FatalErrorCRC = 0x07,
        FatalErrorFrame = 0x08,
        FatalErrorEmerBtnDown = 0x0B,
        FatalErrorCageOpen = 0x0C,
        FatalErrorCable2Cnt2Samp = 0x0D,
        FatalErrorCable2Cnt2Pwr = 0x0E
    };

    enum class ResetRecStatus : quint8 { BUSY = 0, OK = 1 };
    struct ResetRecStruct {
        ResetRecStruct() = default;
        quint8 status;
    };

    struct HandShakeStruct{
        quint32 randomNum_1;
        quint32 randomNum_2;
        quint32 randomNum_3;
        quint32 randomNum_4;
    };

    struct HandShakeRecStruct {
        quint8 mainboardHWRev;
        quint8 mainboardFirmwareRev;
        quint8 sampleboardHWRev;
        quint8 sampleboardFirmwareRev;
        quint8 productRev;
        quint32 secrectKeyMain_1;
        quint32 secrectKeyMain_2;
        quint32 secrectKeyMain_3;
        quint32 secrectKeyMain_4;
        quint32 secrectKeySample_1;
        quint32 secrectKeySample_2;
        quint32 secrectKeySample_3;
        quint32 secrectKeySample_4;
    };

    enum class FreqAdjustStatus: quint8 { F50HZ = 0, F400HZ = 1, INVALID = 0xFF};
    struct FreqAdjustStruct{
        quint8 freqValue_1;
        quint8 freqValue_2;
    };

    enum class FreqAdjustRecStatus : quint8 { WAITING = 0, OK = 1 };
    struct FreqAdjustRecStruct {
        quint8 status;
    };

    enum class WarningRecStatus : quint8 { WarningNotFinished = 0, WarningFinished = 1};
    struct WarningRecStruct {
        quint8 status;
    };

    struct ErrorRecStruct {
        quint8 errorCode;
        quint32 errorInfo;
    };

    struct MeasStartStruct{
        quint16 vol;
        quint8 thro_1;
        quint8 thro_2;
        quint32 distance;
    };

    enum class MeasStartRecStatusEnum : quint8 { DISTANCE = 0, VOL = 1, THRO = 2, MULTI = 3, AGING = 4};
    enum class MotorTypeEnum  : quint8 { OILE = 0, ELECE = 1} ;

    enum class LimitStatusEnum : quint8 {
        INVALID,
        ZEROPOS,
        REACHED,
        RUNNING,
        UPLIMIT,
        DOWNLIMIT,
        NOTARR = 0xFF };

    enum class LimitStatusHEXEnum : quint8 {
        HEXINVALID = 0,
        HEXZEROPOS = 1 << 7,
        HEXREACHED = 1 << 3,
        HEXRUNNING = 1 << 2,
        HEXUPLIMIT = 1 << 1,
        HEXDOWNLIMIT = 1,
        HEXNOTARR = 0xFF
        };

    struct ElecMotorStruct
    {
        quint32 current;
        quint32 lift;
        quint32 torque;
        quint32 speed;
        quint16 temp_1;
        quint16 temp_2;
    };

    struct ElecMotorCompStruct
    {
        quint8   limitStatus;
        quint16  voltage;
        ElecMotorStruct elecMotorStruct[2];  // currently, we have 2 motors with maximum size.
    };

    struct OilMotorCompStruct
    {
        quint32 lift;
        quint32 torque;
        quint32 speed;
        quint16 temp_1;
        quint16 temp_2;
    };

    union MotorCompStruct
    {
        ElecMotorCompStruct elec;
        OilMotorCompStruct oil;
    };

    struct MeasStartRecStruct{
        quint16 humidity;
        quint32 envtemp;
        quint32 pressure;
        //quint16 altitude;
        quint8  thro_1;
        quint8  thro_2;
        quint8  backup_1;
        quint8  backup_2;
        quint8  motorType;
        quint8  numOfMotor;
        MotorCompStruct  motorInfo;
    };

    union MeasDataSendUnion {
        HandShakeStruct p;
        FreqAdjustStruct q;
        MeasStartStruct s;
    };

    union MeasDataReceivedUnion {
       ResetRecStruct p;
       HandShakeRecStruct q;
       FreqAdjustRecStruct r;
       MeasStartRecStruct s;
       WarningRecStruct t;
       ErrorRecStruct u;
    };

    union MeasDataUnion {
        MeasDataSendUnion s;
        MeasDataReceivedUnion r;
    };

    QModbus2DataUnit() = default;

    QModbus2DataUnit(RegisterType type)
        : m_type(type)
    {}

    QModbus2DataUnit(RegisterType type, const HandShakeStruct& v)
        : m_type(type)
    { m_uvalues.s.p = v; }

    QModbus2DataUnit(RegisterType type, const FreqAdjustStruct& v)
        : m_type(type)
    { m_uvalues.s.q = v; }

    QModbus2DataUnit(RegisterType type, const MeasStartStruct& v)
        : m_type(type)
    { m_uvalues.s.s = v; }

    ~QModbus2DataUnit(){}

    RegisterType registerType() const { return m_type; }
    void setRegisterType(RegisterType type) { m_type = type; }

//    inline QVector<quint16> values() const { return m_values; }
//    inline void setValues(const QVector<quint16> &newValues)
//    {
//        m_values = newValues;
//        m_valueCount = newValues.size();
//    }

//    inline uint valueCount() const { return m_valueCount; }
//    inline void setValueCount(uint newCount) { m_valueCount = newCount; }

//    inline void setValue(int index, quint16 newValue)
//    {
//        if (m_values.isEmpty() || index >= m_values.size())
//            return;
//        m_values[index] = newValue;
//    }
//    inline quint16 value(int index) const { return m_values.value(index); }

    bool isValid() const { return m_type != Invalid ;}

    //MeasDataUnion uvalues() const;
    const MeasDataUnion& uvalues() const { return m_uvalues; }

public:
    MeasDataUnion m_uvalues;
private:
    RegisterType m_type = Invalid;
//    QVector<quint16> m_values;
//    uint m_valueCount = 0;
};

typedef QMap<QModbus2DataUnit::RegisterType, QModbus2DataUnit> QModbusDataUnitMap;

Q_DECLARE_TYPEINFO(QModbus2DataUnit, Q_MOVABLE_TYPE);
Q_DECLARE_TYPEINFO(QModbus2DataUnit::RegisterType, Q_PRIMITIVE_TYPE);

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QModbus2DataUnit::RegisterType)

#endif // QMODBUSDATAUNIT_H
