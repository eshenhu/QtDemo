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

#ifndef QMODBUSCLIENT_H
#define QMODBUSCLIENT_H

#include <QtCore/qobject.h>
#include "qmodbusdataunit.h"
#include "qmodbusdevice.h"
#include "qmodbuspdu.h"
#include "qmodbusreply.h"

QT_BEGIN_NAMESPACE

class QModbus2ClientPrivate;

class Q_SERIALBUS_EXPORT QModbus2Client : public QModbus2Device
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QModbus2Client)
    Q_PROPERTY(int timeout READ timeout WRITE setTimeout NOTIFY timeoutChanged)

public:
    explicit QModbus2Client(QObject *parent = nullptr);
    ~QModbus2Client();

    QModbus2Reply *sendReadRequest(const QModbus2DataUnit &read, int serverAddress);
//    QModbus2Reply *sendWriteRequest(const QModbus2DataUnit &write, int serverAddress);
//    QModbus2Reply *sendReadWriteRequest(const QModbus2DataUnit &read, const QModbus2DataUnit &write,
//                                       int serverAddress);
    QModbus2Reply *sendRawRequest(const QModbus2Request &request, int serverAddress);

    int timeout() const;
    void setTimeout(int newTimeout);

    int numberOfRetries() const;
    void setNumberOfRetries(int number);

Q_SIGNALS:
    void timeoutChanged(int newTimeout);

protected:
    QModbus2Client(QModbus2ClientPrivate &dd, QObject *parent = nullptr);

    virtual bool processResponse(const QModbus2Response &response, QModbus2DataUnit *data);
    virtual bool processPrivateResponse(const QModbus2Response &response, QModbus2DataUnit *data);
};

QT_END_NAMESPACE

#endif // QMODBUSCLIENT_H
