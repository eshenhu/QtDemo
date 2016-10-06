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
        ResetCode = 0x01,
        HandShakeCode = 0x02,
        FreqAdjustCode = 0x03,
        StartBtnCode = 0x04,
        AlarmInfoCode = 0x05,
        MeasConfigCode = 0x06,
        MeasStartCode = 0x07,
        MeasEndCode = 0x08,
        ManualMeasStartCode = 0x09,
        ThroCalibrateCode = 0x0a,
        QueryAlarmInfoCode = 0x0b
    };

    struct HandShakeStruct{
        quint16 randomNumLow;
        quint16 randomNumHigh;
    };

    struct FreqAdjustStruct{
        quint8 freqValue;
    };

    enum class MeasConfigRecStatus : quint8 { DISTANCE = 0, VOL = 1, THRO = 2, MULTI = 3, AGING = 4};
    struct MeasConfigStruct{
        quint8 measOptions;
        quint16 throStart;
        quint16 throEnd;
        quint16 throStep;
        quint16 volStart;
        quint16 volEnd;
        quint16 volStep;
        quint16 disStart;
        quint16 disEnd;
        quint16 disStep;
        quint16 timeStep;
        quint16 startDelay;
        quint16 softDelay;
    };

    struct ManualMeasStruct{
        quint16 vol;
        quint16 thro;
        quint16 distance;
    };

    struct ThroCalibrateStruct{
        quint16 timeInHigh;
        quint16 timeInLow;
    };

    enum class ResetRecStatus : quint8 { BUSY = 0, OK = 1 };
    struct ResetRecStruct {
        ResetRecStruct() = default;
        quint8 status;
    };

    struct HandShakeRecStruct {
        quint8 mainboardHWRev;
        quint8 mainboardFirmwareRev;
        quint8 sampleboardHWRev;
        quint8 sampleboardFirmwareRev;
        quint16 volSpan;
        quint16 currentSpan;
        quint8 forceSpanL;
        quint8 forceSpanM;
        quint8 forceSpanH;
        quint8 forceZeroL;
        quint8 forceZeroM;
        quint8 forceZeroH;
        quint8 force2ZeroL;
        quint8 force2ZeroM;
        quint8 force2ZeroH;
        quint8 torqueSpanL;
        quint8 torqueSpanM;
        quint8 torqueSpanH;
        quint8 torqueZeroSpanL;
        quint8 torqueZeroSpanM;
        quint8 torqueZeroSpanH;
        quint8 torque2ZeroSpanL;
        quint8 torque2ZeroSpanM;
        quint8 torque2ZeroSpanH;
        quint16 mainboardID1;
        quint16 mainboardID2;
        quint16 mainboardID3;
        quint16 mainboardID4;
        quint16 sampleboardID1;
        quint16 sampleboardID2;
        quint16 sampleboardID3;
        quint16 sampleboardID4;
        quint8 errorID1;
        quint8 errorID2;
        quint8 errorID3;
    };

    enum class StartBtnRecStatus : quint8 { WAITING = 0, OK = 1 };
    struct StartBtnRecStruct {
        quint8 status;
    };

    enum class WarningRecStatus : quint8 { STATUS = 0};
    struct WarningRecStruct {
        quint8 status;
    };

    struct MeasConfigRecStruct {
        quint8 options;
    };

    struct ManualMeasRecStruct {
        quint16 sequenceNum;
        quint16 vol;
        quint16 cur1;
        quint16 cur2;
        quint8 forceL;
        quint8 forceM;
        quint8 forceH;
        quint8 torqueL;
        quint8 torqueM;
        quint8 torqueH;
        quint16 speedL;
        quint16 speedH;
        quint8 force2L;
        quint8 force2M;
        quint8 force2H;
        quint8 torque2L;
        quint8 torque2M;
        quint8 torque2H;
        quint16 speed2L;
        quint16 speed2H;
        quint16 temp1;
        quint16 temp2;
        quint16 temp3;
        quint16 temp4;
        quint16 shock1X;
        quint16 shock1Y;
        quint16 shock1Z;
        quint16 shock2X;
        quint16 shock2Y;
        quint16 shock2Z;
        quint8  fuelL;
        quint8  fuelM;
        quint8  fuelH;
        quint8  expand1;
        quint8  expand2;
        quint8  expand3;
        quint8  errorL;
        quint8  errorM;
        quint8  errorH;
    };

    union MeasDataSendUnion {
        HandShakeStruct p;
        FreqAdjustStruct q;
        MeasConfigStruct r;
        ManualMeasStruct s;
        ThroCalibrateStruct t;
    };

    union MeasDataReceivedUnion {
       ResetRecStruct p;
       HandShakeRecStruct q;
       FreqAdjustStruct r;
       StartBtnRecStruct s;
       WarningRecStruct t;
       MeasConfigRecStruct u;
       ManualMeasRecStruct v;
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

    QModbus2DataUnit(RegisterType type, const MeasConfigStruct& v)
        : m_type(type)
    { m_uvalues.s.r = v; }

    QModbus2DataUnit(RegisterType type, const ManualMeasStruct& v)
        : m_type(type)
    { m_uvalues.s.s = v; }

    QModbus2DataUnit(RegisterType type, const ThroCalibrateStruct& v)
        : m_type(type)
    { m_uvalues.s.t = v; }

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

    MeasDataUnion uvalues() const;

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
