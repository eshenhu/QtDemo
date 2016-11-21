#include "utildatarecordingclz.h"
#include <QFile>
#include <QApplication>
#include <QDir>
#include <QDateTime>

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
    const QString timestamp = now.toString(QLatin1String("yyyyMMdd-hhmmsszzz"));
    // create a subfolder named with timestamp;
    QDir dir(path);
    dir.mkdir(timestamp);

    m_cfgFile = path + QStringLiteral("/%1/data.json").arg(timestamp);
    m_recFile = path + QStringLiteral("/%1/data.csv").arg(timestamp);
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
