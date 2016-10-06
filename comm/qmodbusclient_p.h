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

#ifndef QMODBUSCLIENT_P_H
#define QMODBUSCLIENT_P_H

#include <QtCore/qtimer.h>
#include "qmodbusclient.h"
#include "qmodbuspdu.h"

#include "qmodbusdevice_p.h"

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

class Q_AUTOTEST_EXPORT QModbus2ClientPrivate : public QModbus2DevicePrivate
{
    Q_DECLARE_PUBLIC(QModbus2Client)

public:
    QModbus2Reply *sendRequest(const QModbus2Request &request, int serverAddress,
                              const QModbus2DataUnit *const unit);
    QModbus2Request createReadRequest(const QModbus2DataUnit &data) const;
//    QModbus2Request createWriteRequest(const QModbus2DataUnit &data) const;
//    QModbus2Request createRWRequest(const QModbus2DataUnit &read, const QModbus2DataUnit &write) const;

    bool processResponse(const QModbus2Response &response, QModbus2DataUnit *data);

    virtual QModbus2Reply *enqueueRequest(const QModbus2Request &, int, const QModbus2DataUnit &,
                                         QModbus2Reply::ReplyType) {
        return nullptr;
    }
    // TODO: Review once we have a transport layer in place.
    virtual bool isOpen() const { return false; }

    int m_numberOfRetries = 3;
    int m_responseTimeoutDuration = 1000;

    struct QueueElement {
        QueueElement() = default;
        QueueElement(QModbus2Reply *r, const QModbus2Request &req, const QModbus2DataUnit &u, int num,
                int timeout = -1)
            : reply(r), requestPdu(req), unit(u), numberOfRetries(num)
        {
            if (timeout >= 0) {
                // always the case for TCP
                timer.reset(new QTimer);
                timer->setSingleShot(true);
                timer->setInterval(timeout);
            }
        }
        bool operator==(const QueueElement &other) const {
            return reply == other.reply;
        }

        QPointer<QModbus2Reply> reply;
        QModbus2Request requestPdu;
        QModbus2DataUnit unit;
        int numberOfRetries;
        QSharedPointer<QTimer> timer;
        QByteArray adu;
        qint64 bytesWritten = 0;
    };
    void processQueueElement(const QModbus2Response &pdu, const QueueElement &element);
private:
    bool processReadRestCodeResponse(const QModbus2Response &response, QModbus2DataUnit *data);
    bool processReadHandShakeCodeResponse(const QModbus2Response &response, QModbus2DataUnit *data);
    bool processReadStartBtnCodeResponse(const QModbus2Response &response, QModbus2DataUnit *data);
    bool processReadAlarmInfoCodeResponse(const QModbus2Response &response, QModbus2DataUnit *data);
    bool processReadMeasConfigCodeResponse(const QModbus2Response &response, QModbus2DataUnit *data);
    bool processReadMeasStartCodeResponse(const QModbus2Response &response, QModbus2DataUnit *data);
    bool processReadMeasEndCodeResponse(const QModbus2Response &response, QModbus2DataUnit *data);
    bool processReadManualMeasStartCodeResponse(const QModbus2Response &response, QModbus2DataUnit *data);
    bool processReadThroCalibrateCodeResponse(const QModbus2Response &response, QModbus2DataUnit *data);
    bool processReadFreqAdjustCodeResponse(const QModbus2Response &response, QModbus2DataUnit *data);
    bool processReadQueryAlarmInfoCodeResponse(const QModbus2Response &response, QModbus2DataUnit *data);
public:
//    static bool isValid(const QModbus2Response &response, QModbus2Response::FunctionCode fc);
};

QT_END_NAMESPACE

#endif // QMODBUSCLIENT_P_H

