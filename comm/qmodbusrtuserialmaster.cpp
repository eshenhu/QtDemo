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

#include "qmodbusrtuserialmaster.h"
#include "qmodbusrtuserialmaster_p.h"

#include <QtCore/qloggingcategory.h>

QT_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(QT_MODBUS2)
Q_DECLARE_LOGGING_CATEGORY(QT_MODBUS2_LOW)

/*!
    \class QModbusRtuSerialMaster
    \inmodule QtSerialBus
    \since 5.6

    \brief The QModbusRtuSerialMaster class represents a Modbus client
    that uses a serial bus for its communication with the Modbus server.

    Communication via Modbus requires the interaction between a single
    Modbus client instance and multiple Modbus servers. This class
    provides the client implementation via a serial port.
*/

/*!
    Constructs a serial Modbus master with the specified \a parent.
*/
QModbus2RtuSerialMaster::QModbus2RtuSerialMaster(QObject *parent)
    : QModbus2Client(*new QModbus2RtuSerialMasterPrivate, parent)
{
    Q_D(QModbus2RtuSerialMaster);
    d->setupSerialPort();
}

/*!
    \internal
*/
QModbus2RtuSerialMaster::~QModbus2RtuSerialMaster()
{
    close();
}

/*!
    \since 5.7

    Returns the amount of microseconds for the silent interval between two
    consecutive Modbus messages.

    \sa setInterFrameDelay()
*/
int QModbus2RtuSerialMaster::interFrameDelay() const
{
    Q_D(const QModbus2RtuSerialMaster);
    return d->m_interFrameDelayMilliseconds * 1000;
}

/*!
    \since 5.7

    Sets the amount of \a microseconds for the silent interval between two
    consecutive Modbus messages. By default, the class implementation will use
    a pre-calculated value according to the Modbus specification. A active or
    running connection is not affected by such delay changes.

    \note If \a microseconds is set to -1 or \a microseconds is less than the
    pre-calculated delay then this pre-calculated value is used as frame delay.
*/
void QModbus2RtuSerialMaster::setInterFrameDelay(int microseconds)
{
    Q_D(QModbus2RtuSerialMaster);
    d->m_interFrameDelayMilliseconds = qCeil(qreal(microseconds) / 1000.);
    d->calculateInterFrameDelay();
}

/*!
    \internal
*/
QModbus2RtuSerialMaster::QModbus2RtuSerialMaster(QModbus2RtuSerialMasterPrivate &dd, QObject *parent)
    : QModbus2Client(dd, parent)
{
    Q_D(QModbus2RtuSerialMaster);
    d->setupSerialPort();
}

/*!
     \reimp

     \note When calling this function, existing buffered data is removed from
     the serial port.
*/
bool QModbus2RtuSerialMaster::open()
{
    if (state() == QModbus2Device::ConnectedState)
        return true;

    Q_D(QModbus2RtuSerialMaster);
    d->setupEnvironment(); // to be done before open
    if (d->m_serialPort->open(QIODevice::ReadWrite)) {
        setState(QModbus2Device::ConnectedState);
        d->m_serialPort->clear(); // only possible after open
    } else {
        setError(d->m_serialPort->errorString(), QModbus2Device::ConnectionError);
    }
    return (state() == QModbus2Device::ConnectedState);
}

/*!
     \reimp
*/
void QModbus2RtuSerialMaster::close()
{
    if (state() == QModbus2Device::UnconnectedState)
        return;

    Q_D(QModbus2RtuSerialMaster);
    if (d->m_serialPort->isOpen())
        d->m_serialPort->close();

    if (d->m_queue.count())
        qCDebug(QT_MODBUS2_LOW) << "(RTU client) Aborted replies:" << d->m_queue.count();

    while (!d->m_queue.isEmpty()) {
        // Finish each open reply and forget them
        QModbus2RtuSerialMasterPrivate::QueueElement elem = d->m_queue.dequeue();
        if (!elem.reply.isNull()) {
            elem.reply->setError(QModbus2Device::ReplyAbortedError,
                                 QModbus2Client::tr("Reply aborted due to connection closure."));
        }
    }

    setState(QModbus2Device::UnconnectedState);
}

QT_END_NAMESPACE
