#ifndef DATAJSONCFGREADER_H
#define DATAJSONCFGREADER_H

/*
 *  This class is mainly foucus on
 *
 * - parse the json file located in the specified folder.
 * - validate the json file.
 * - factory class generate a new class for parsing the .csv
 *   file with the same prefix filename with .json file.
 *
*/
#include <QString>
#include <QtCore/qloggingcategory.h>

#include "cfg/cfgwashingdatainf.h"

Q_DECLARE_LOGGING_CATEGORY(TEXT_LOGGING)

class DataJsonCfgReader
{
public:
    DataJsonCfgReader();
    ~DataJsonCfgReader();

    bool loadData(const QString& jsonFileName);

    bool washData();
    bool close();
    CfgWashingDataInf *csvDataHandler() const;

private:
    CfgWashingDataInf* m_csvDataHandler = nullptr;
};

#endif // DATAJSONCFGREADER_H
