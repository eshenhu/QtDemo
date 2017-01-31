#include "utildatarecordingclz.h"
#include <QFile>
#include <QApplication>
#include <QDir>
#include <QDateTime>
#include <QDebug>

UtilDataRecordingClz::UtilDataRecordingClz()
{
}

/*
 *  create data.json & data.csv file where the executable file located.
 */
bool UtilDataRecordingClz::newRec()
{
    QString path = QCoreApplication::applicationDirPath();

    QDateTime now = QDateTime::currentDateTime();
    m_timeStamp = now.toString(QLatin1String("yyyyMMdd-hhmmsszzz"));
    // create a subfolder named with timestamp;
    QDir dir(path);
    if (!dir.mkdir(m_timeStamp))
        qWarning() << "UtilDataRecordingClz failed to create folder "
                   << path << m_timeStamp;

    m_cfgFile = path + QString("/%1/data.json").arg(m_timeStamp);
    m_recFile = path + QString("/%1/data.csv").arg(m_timeStamp);
    return true;
}

const QString& UtilDataRecordingClz::getCfgFileName() const
{
    return m_cfgFile;
}

const QString& UtilDataRecordingClz::getRecFileName() const
{
    return m_recFile;
}

QString UtilDataRecordingClz::getTimeStamp() const
{
    return m_timeStamp;
}
