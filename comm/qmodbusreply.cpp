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

#include "qmodbusreply.h"

#include <QtCore/qobject.h>
#include <private/qobject_p.h>

QT_BEGIN_NAMESPACE

class QModbus2ReplyPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QModbus2Reply)

public:
    QModbus2DataUnit m_unit;
    int m_serverAddress = 1;
    bool m_finished = false;
    QModbus2Device::Error m_error = QModbus2Device::NoError;
    QString m_errorText;
    QModbus2Response m_response;
    QModbus2Reply::ReplyType m_type;
};

/*!
    \class QModbus2Reply
    \inmodule QtSerialBus
    \since 5.6

    \brief The QModbus2Reply class contains the data for a request sent with
    a \l QModbus2Client derived class.
*/

/*!
    \enum QModbus2Reply::ReplyType

    This enum describes the possible reply type.

    \value Raw      The reply originates from a raw Modbus request. See
                    \l QModbus2Client::sendRawRequest
    \value Common   The reply originates from a common read, write or read/write
                    request. See \l QModbus2Client::sendReadRequest,
                    \l QModbus2Client::sendWriteRequest and \l QModbus2Client::sendReadWriteRequest
*/

/*!
    Constructs a QModbus2Reply object with a given \a type and the specified \a parent.

    The reply will be send to the Modbus client represented by
    \a serverAddress.
*/
QModbus2Reply::QModbus2Reply(ReplyType type, int serverAddress, QObject *parent)
    : QObject(*new QModbus2ReplyPrivate, parent)
{
    Q_D(QModbus2Reply);
    d->m_type = type;
    d->m_serverAddress = serverAddress;
}

/*!
    Returns \c true when the reply has finished or was aborted.
*/
bool QModbus2Reply::isFinished() const
{
    Q_D(const QModbus2Reply);
    return d->m_finished;
}

/*!
   \internal
    Sets whether or not this reply has finished to \a isFinished.

    If \a isFinished is \c true, this will cause the \l finished() signal to be emitted.

    If the operation completed successfully, \l setResult() should be called before
    this function. If an error occurred, \l setError() should be used instead.
*/
void QModbus2Reply::setFinished(bool isFinished)
{
    Q_D(QModbus2Reply);
    d->m_finished = isFinished;
    if (isFinished)
        emit finished();
}

/*!
    \fn void QModbus2Reply::finished()

    This signal is emitted when the reply has finished processing. The reply may still have
    returned with an error.

    After this signal is emitted, there will be no more updates to the reply's data.

    \note Do not delete the object in the slot connected to this signal. Use deleteLater().

    You can also use \l isFinished() to check if a QNetworkReply has finished even before
    you receive the \l finished() signal.

    \sa isFinished(), error()
*/

/*!
    Returns the preprocessed result of a Modbus request.

    For read requests as well as combined read/write requests send via
    \l QModbus2Client::sendReadWriteRequest() it contains the values read
    from the server instance.

    If the request has not finished, has failed with an error or was a write
    request then the returned \l QModbus2DataUnit instance is invalid.

    \note If the \l type() of the reply is \l QModbus2Reply::Raw, the return
    value will always be invalid.

    \sa type(), rawResult()
*/
QModbus2DataUnit QModbus2Reply::result() const
{
    Q_D(const QModbus2Reply);
    if (type() == QModbus2Reply::Common)
        return d->m_unit;
    return QModbus2DataUnit();
}

/*!
    \internal
    Sets the results of a read/write request to a Modbus register data \a unit.
*/
void QModbus2Reply::setResult(const QModbus2DataUnit &unit)
{
    Q_D(QModbus2Reply);
    d->m_unit = unit;
}

/*!
    Returns the server address that this reply object targets.
*/
int QModbus2Reply::serverAddress() const
{
    Q_D(const QModbus2Reply);
    return d->m_serverAddress;
}

/*!
    \fn void QModbus2Reply::errorOccurred(QModbus2Device::Error error)

    This signal is emitted when an error has been detected in the processing of
    this reply. The \l finished() signal will probably follow.

    The error will be described by the error code \a error. If errorString is
    not empty it will contain a textual description of the error. In case of a
    \l QModbus2Device::ProtocolError the \l rawResult() function can be used to
    obtain the original Modbus exception response to get the exception code.

    Note: Do not delete this reply object in the slot connected to this signal.
    Use \l deleteLater() instead.
*/

/*!
    Returns the error state of this reply.
*/
QModbus2Device::Error QModbus2Reply::error() const
{
    Q_D(const QModbus2Reply);
    return d->m_error;
}

/*!
   \internal
    Sets the error state of this reply to \a error and the textual representation of
    the error to \a errorText.

    This will also cause the \l errorOccurred() and \l finished() signals to be emitted,
    in that order.
*/
void QModbus2Reply::setError(QModbus2Device::Error error, const QString &errorText)
{
    Q_D(QModbus2Reply);
    d->m_error = error;
    d->m_errorText = errorText;
    emit errorOccurred(error);
    setFinished(true);
}

/*!
    Returns the textual representation of the error state of this reply.

    If no error has occurred this will return an empty string. It is possible
    that an error occurred which has no associated textual representation,
    in which case this will also return an empty string.

    \sa error(), errorOccurred()
*/
QString QModbus2Reply::errorString() const
{
    Q_D(const QModbus2Reply);
    return d->m_errorText;
}


/*!
    Returns the type of the reply.

    \note If the type of the reply is \l QModbus2Reply::Raw, the return value
    of \l result() will always be invalid.

    \sa result(), rawResult()
*/
QModbus2Reply::ReplyType QModbus2Reply::type() const
{
    Q_D(const QModbus2Reply);
    return d->m_type;
}

/*!
    Returns the raw response of a Modbus request.

    If the request has not finished then the returned \l QModbus2Response
    instance is invalid.

    \sa type(), result()
*/
QModbus2Response QModbus2Reply::rawResult() const
{
    Q_D(const QModbus2Reply);
    return d->m_response;
}

/*!
    \internal
    Sets the result of a Modbus request to a Modbus \a response.
*/
void QModbus2Reply::setRawResult(const QModbus2Response &response)
{
    Q_D(QModbus2Reply);
    d->m_response = response;
}

#include "moc_qmodbusreply.cpp"

QT_END_NAMESPACE
