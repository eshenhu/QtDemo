#include "datajsoncfgreader.h"
#include "cfg/cfgjsonrecelement.h"
#include <QFile>
#include <QFileInfo>
#include <QDir>
DataJsonCfgReader::DataJsonCfgReader()
{

}

DataJsonCfgReader::~DataJsonCfgReader()
{
    if (m_csvDataHandler)
        delete m_csvDataHandler;
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

    CfgJsonRecElement cfgParser;

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
                    handler.loadData(jsonFileName);
                    m_csvDataHandler = new CfgThrottleWashingDataE2Clz();
                    m_csvDataHandler->wash(handler.data());
                }
                    break;
                default:
                    break;
                }
            }
            else
            {
                qCWarning(TEXT_LOGGING) << "This type of file was not supported now" << jsonFileName;
            }
        }
    }
    else
    {
        qCWarning(TEXT_LOGGING) << "Json file failed to open with filename" << jsonFileName;
    }
}

bool DataJsonCfgReader::washData()
{

    //if (m_csvDataHandler)
}

bool DataJsonCfgReader::close()
{
    if (m_csvDataHandler)
        delete m_csvDataHandler;

    return true;
}

CfgWashingDataInf *DataJsonCfgReader::csvDataHandler() const
{
    return m_csvDataHandler;
}
