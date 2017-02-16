#include "utildatarecordingclz.h"
#include <QFile>
#include <QApplication>
#include <QDir>
#include <QDateTime>
#include <QDebug>
#include "cfg/unireslocation.h"

UtilDataRecordingClz::UtilDataRecordingClz()
{
}

/*
 *  create data.json & data.csv file where the executable file located.
 */
bool UtilDataRecordingClz::newRec(TestPlanEnum plan)
{
    QString text = TestPlanStringMap[(int)plan];

    QString path = QCoreApplication::applicationDirPath();
    const QString pathToTestDataStore = QStringLiteral("Test_Data");

    QDateTime now = QDateTime::currentDateTime();
    m_timeStamp = now.toString(QLatin1String("yyyyMMdd-hhmmsszzz"));

    const QString relavantPathToTestDataStore =
            pathToTestDataStore + QDir::separator() + m_timeStamp;
    const QString fullPathToTestDataStore = path + QDir::separator()
            + relavantPathToTestDataStore;

    // create a subfolder named with timestamp;
    QDir dir(path);

    if (!dir.mkpath(relavantPathToTestDataStore))
    {
        qWarning() << "UtilDataRecordingClz failed to create folder "
                   << relavantPathToTestDataStore;
        return false;
    }

    //dir.cd(relavantPathToTestDataStore);

    const QStringList saveNameList({UniResLocation::getCfgResHdl()->manufacture(),
                                    UniResLocation::getCfgResHdl()->SerialNumber(),
                                    UniResLocation::getCfgResHdl()->motorType(),
                                    UniResLocation::getCfgResHdl()->ESCType(),
                                    QStringLiteral("data"),
                                    m_timeStamp,
                                    text});
    const QString savename = saveNameList.join('-');

    m_cfgFile = fullPathToTestDataStore + QString("/%1.json").arg(savename);
    m_recFile = fullPathToTestDataStore + QString("/%1.csv").arg(savename);
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
