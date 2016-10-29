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

#include "qmodbusdataunit.h"

QT_BEGIN_NAMESPACE

/*!
    \class QModbus2DataUnit
    \inmodule QtSerialBus
    \since 5.6

    \brief QModbus2DataUnit is a container class representing single bit and
    \c 16 bit word entries in the Modbus register.

    \l QModbus2DataUnit can be used for read and write operations. The entries
    are addressed via \l startAddress() and the \l valueCount() number of
    contiguous entries. \l registerType() determines which register is used for
    the operations. Note that some registers are read-only registers.

    The actual \l values() are either single bit or \c 16 bit based.
    \l QModbus2DataUnit::DiscreteInputs and \l QModbus2DataUnit::Coils
    only accept single bits. Therefore \c 0 is interpreted as \c 0 and anything
    else \c 1.
*/

/*!
    \enum QModbus2DataUnit::RegisterType

    This enum describes all supported register types.

    \value Invalid              Set by the default constructor, do not use.
    \value DiscreteInputs       This type of data can be provided by an I/O
                                system.
    \value Coils                This type of data can be alterable by an
                                application program.
    \value InputRegisters       This type of data can be provided by an I/O
                                system.
    \value HoldingRegisters     This type of data can be alterable by an
                                application program.
*/

/*!
    \fn QModbus2DataUnit::QModbus2DataUnit()

    Constructs an empty, invalid QModbus2DataUnit. Start address is set to \c -1
    and the \l registerType is set to \l QModbus2DataUnit::Invalid.
*/

/*!
    \fn QModbus2DataUnit::QModbus2DataUnit(RegisterType type)

    Constructs a unit of data for register \a type. Start address is set to
    \c 0, data range and data values are empty.
*/

/*!
    \fn QModbus2DataUnit::QModbus2DataUnit(RegisterType type, int address,
                                         quint16 size)

    Constructs a unit of data for register\a type. Start address of the data is
    set to \a address and the size of the unit to \a size.
    The entries of \l values() are initialized with \c 0.
*/

/*!
    \fn QModbus2DataUnit::QModbus2DataUnit(RegisterType type, int address,
                                         const QVector<quint16> &data)

    Constructs a unit of data for register\a type. Start address of the data is
    set to \a address and the unit's values to \a data.
    The value count is implied by the \a data size.
*/

/*!
    \fn void QModbus2DataUnit::setRegisterType(QModbus2DataUnit::RegisterType type)

    Sets the register \a type.

    \sa registerType(), QModbus2DataUnit::RegisterType
*/

/*!
    \fn QModbus2DataUnit::RegisterType QModbus2DataUnit::registerType() const

    Returns the type of the register.

    \sa setRegisterType(), QModbus2DataUnit::RegisterType
*/

/*!
    \fn void QModbus2DataUnit::setStartAddress(int address)

    Sets the start \a address of the data unit.

    \sa startAddress()
*/

/*!
    \fn int QModbus2DataUnit::startAddress() const

    Returns the start address of data unit in the register.

    \sa setStartAddress()
*/

/*!
    \fn void QModbus2DataUnit::setValues(const QVector<quint16> &values)

    Sets the \a values of the data unit. \l QModbus2DataUnit::DiscreteInputs
    and \l QModbus2DataUnit::Coils tables only accept single bit value, so \c 0
    is interpreted as \c 0 and anything else as \c 1.

    \sa values()
*/

/*!
    \fn QVector<quint16> QModbus2DataUnit::values() const

    Returns the data in the data unit. \l QModbus2DataUnit::DiscreteInputs
    and \l QModbus2DataUnit::Coils tables only accept single bit value, so \c 0
    is interpreted as \c 0 and anything else as \c 1.

    \sa setValues()
*/

/*!
    \fn uint QModbus2DataUnit::valueCount() const

    Returns the size of the requested register's data block or the size of data
    read from the device.

    This function may not always return a count that equals \l values() size.
    Since this class is used to request data from the remote data register, the
    \l valueCount() can be used to indicate the size of the requested register's
    data block. Once the request has been processed, the \l valueCount() is
    equal to the size of \l values().

    \sa setValueCount()
*/

/*!
    \fn void QModbus2DataUnit::setValueCount(uint newCount)

    Sets the size of the requested register's data block to \a newCount.

    This may be different from \l values() size as this function is used
    to indicated the size of a data request. Only once the data request
    has been processed \l valueCount() is equal to the size of \l values().
*/

/*!
    \fn void QModbus2DataUnit::setValue(int index, quint16 value)

    Sets the register at position \a index to \a value.
*/

/*!
    \fn quint16 QModbus2DataUnit::value(int index) const

    Return the value at position \a index.
*/

/*!
    \fn bool QModbus2DataUnit::isValid() const

    Returns \c true if the \c QModbus2DataUnit is valid; otherwise \c false.
    A \c QModbus2DataUnit is considered valid if the \l registerType() is not
    \l QModbus2DataUnit::Invalid and the \l startAddress() is greater than or
    equal to \c 0.
*/

/*!
    \typedef QModbusDataUnitMap
    \relates QModbus2DataUnit
    \since 5.6

    Synonym for QMap<QModbus2DataUnit::RegisterType, QModbus2DataUnit>.
*/

QT_END_NAMESPACE

//QModbus2DataUnit::MeasDataUnion QModbus2DataUnit::uvalues() const
//{
//    return m_uvalues;
//}
