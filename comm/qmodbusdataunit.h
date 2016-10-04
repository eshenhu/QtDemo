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

//        DiscreteInputs,
//        Coils,
//        InputRegisters,
//        HoldingRegisters,
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
        HandShakeStruct() = default;
        quint16 randomNumLow;
        quint16 randomNumHigh;
    };

    struct FreqAdjustStruct{
        FreqAdjustStruct() = default;
        quint8 freqValue;
    };

    struct MeasConfigStruct{
        MeasConfigStruct() = default;
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
        ManualMeasStruct() = default;
        quint16 vol;
        quint16 thro;
        quint16 distance;
    };

    struct ThroCalibrateStruct{
        ThroCalibrateStruct() = default;
        quint16 timeInHigh;
        quint16 timeInLow;
    };

    union MeasDataUnion {
        HandShakeStruct p;
        FreqAdjustStruct q;
        MeasConfigStruct r;
        ManualMeasStruct s;
        ThroCalibrateStruct t;
    };

    QModbus2DataUnit() = default;

    QModbus2DataUnit(RegisterType type)
        : m_type(type)
    {}

//    QModbus2DataUnit(RegisterType type, quint16 newValueCount)
//        : QModbus2DataUnit(type, QVector<quint16>(newValueCount))
//    {}

//    QModbus2DataUnit(RegisterType type, const QVector<quint16> &newValues)
//        : m_type(type)
//        , m_values(newValues)
//        , m_valueCount(newValues.size())
//    {}

    QModbus2DataUnit(RegisterType type, const HandShakeStruct& v)
        : m_type(type)
    { m_uvalues.p = v; }

    QModbus2DataUnit(RegisterType type, const FreqAdjustStruct& v)
        : m_type(type)
    { m_uvalues.q = v; }

    QModbus2DataUnit(RegisterType type, const MeasConfigStruct& v)
        : m_type(type)
    { m_uvalues.r = v; }

    QModbus2DataUnit(RegisterType type, const ManualMeasStruct& v)
        : m_type(type)
    { m_uvalues.s = v; }

    QModbus2DataUnit(RegisterType type, const ThroCalibrateStruct& v)
        : m_type(type)
    { m_uvalues.t = v; }


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

private:
    RegisterType m_type = Invalid;
//    QVector<quint16> m_values;
    MeasDataUnion m_uvalues;
//    uint m_valueCount = 0;
};
typedef QMap<QModbus2DataUnit::RegisterType, QModbus2DataUnit> QModbusDataUnitMap;

Q_DECLARE_TYPEINFO(QModbus2DataUnit, Q_MOVABLE_TYPE);
Q_DECLARE_TYPEINFO(QModbus2DataUnit::RegisterType, Q_PRIMITIVE_TYPE);

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QModbus2DataUnit::RegisterType)

#endif // QMODBUSDATAUNIT_H
