#include "datajsoncfgreader.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QSharedPointer>
DataJsonCfgReader::DataJsonCfgReader()
{

}

DataJsonCfgReader::~DataJsonCfgReader()
{
}

bool DataJsonCfgReader::loadData(const QString &jsonFileName)
{
    bool isExisted = true;

    QFileInfo fi(jsonFileName);
    QString path = fi.absolutePath();
    QString csvFileName = fi.baseName().append(".csv");

    QString csvFullFileName = path +  QDir::separator() + csvFileName;

    QFileInfo fullCsvName(csvFullFileName);

    if (!fullCsvName.exists())
    {
        qCWarning(TEXT_LOGGING) << "Corresponding csv file was not existed!";
        return false;
    }

    if (cfgParser.loadCfg(jsonFileName))
    {
        if (cfgParser.motorType() == QModbus2DataUnit::MotorTypeEnum::ELECE)
        {
            if (cfgParser.numOfMotor() == 2)
            {
                switch (static_cast<quint32>(cfgParser.plan()))
                {
                case (quint32)TestPlanEnum::Throttle:
                {
                    DataJsonRecElementE2::DataJsonRecElementE2FileReaderHandler handler;
                    handler.loadData(csvFullFileName);
                    //m_csvDataHandler = new CfgThrottleWashingDataE2Clz();
                    m_csvDataHandler = QSharedPointer<CfgThrottleWashingDataE2Clz>::create();
                    QSharedPointer<CfgThrottleWashingDataE2Clz> dynCast =
                            qSharedPointerDynamicCast<CfgThrottleWashingDataE2Clz>(m_csvDataHandler);
                    if (dynCast != nullptr)
                    {
                         dynCast->wash(handler.data());
                    }
                }
                    break;
                default:
                    break;
                }
            }
            else
            {
                isExisted = false;
                qCWarning(TEXT_LOGGING) << "This type of file was not supported now" << jsonFileName;
            }
        }
        else
        {
            isExisted = false;
            qCWarning(TEXT_LOGGING) << "This type of file was not supported now" << jsonFileName;
        }
    }
    else
    {
        isExisted = false;
        qCWarning(TEXT_LOGGING) << "Json file failed to open with filename" << jsonFileName;
    }

    return isExisted;
}

bool DataJsonCfgReader::washData()
{
    return false;
}

bool DataJsonCfgReader::close()
{
    return true;
}

CfgJsonRecElement DataJsonCfgReader::getCfgParser() const
{
    return cfgParser;
}
