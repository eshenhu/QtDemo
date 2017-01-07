#ifndef DATAJSONRECELEMENT_H
#define DATAJSONRECELEMENT_H

#include <QVector>
#include <QFile>
#include <QtCore/qloggingcategory.h>



Q_DECLARE_LOGGING_CATEGORY(TEXT_LOGGING)

/*
 *  It was perferred use C++ template. --eshenhu
*/
/*
 * Drived class dedictated for 2 motor with elec type.
 */
class DataJsonRecElementE2
{
public:
    enum class ELEMCURSOR : quint8
    {
        CURSOR_POS      = 0,

        VOL_POS         = 1,
        THRO1_POS       = 2,
        THRO2_POS       = 3,
        DISTANCE_POS    = 4,

        REC_POSSTATUS_POS,

        REC_VOL_POS,
        REC_THRO_POS,
        REC_ECHODISTANCE_POS,

        REC_CUR1_POS,
        REC_THU1_POS,
        REC_TORQUE1_POS,
        REC_SPEED1_POS,
        REC_MOTOR1TMP1_POS,
        REC_MOTOR1TMP2_POS,
        REC_M1EFFICI1_POS,
        REC_M1EFFICI2_POS,
        REC_M1POWER,
        REC_M1MECHAPWR,
        REC_M1MECHAEFFI,

        REC_CUR2_POS,
        REC_THU2_POS,
        REC_TORQUE2_POS,
        REC_SPEED2_POS,
        REC_MOTOR2TMP1_POS,
        REC_MOTOR2TMP2_POS,
        REC_M2EFFICI1_POS,
        REC_M2EFFICI2_POS,
        REC_M2POWER,
        REC_M2MECHAPWR,
        REC_M2MECHAEFFI,

        ELEMCURSOR_END
    };

public:
    DataJsonRecElementE2();

    const QString toString() const;

    double getData(quint32 idx) const {
        return (idx < static_cast<quint32>(m_data.size())) ? m_data[idx] : 0;
    }

    bool hardReset();
    bool reset();
    bool incCursor();
    bool setMetaData(quint32 vol, quint32 thro1, quint32 thro2, quint32 dis);
    bool setPosStatus(quint32);
    bool setData(const quint32 idx, const double v){
        if (idx < static_cast<quint32>(m_data.size())){
            m_data[idx] = v;
            return true;
        }
        else{
            return false;
        }
    }

private:
    QVector<double> m_data;

public:
    class DataJsonRecElementE2GetHelper;
    class DataJsonRecElementE2FileHelper;
    class DataJsonRecElementE2FileReaderHandler;
};


class DataJsonRecElementE2::DataJsonRecElementE2GetHelper
{
public:
    DataJsonRecElementE2GetHelper() = default;

public:
     DataJsonRecElementE2& getElem(bool isNew = false);
};

class DataJsonRecElementE2::DataJsonRecElementE2FileHelper
{
public:
    DataJsonRecElementE2FileHelper() = default;

private:
    QFile& getFile(){
        static QFile m_fileHandler;
        return m_fileHandler;
    }

public:
    bool newFile(const QString& path);
    bool closeFile();
    bool writeData(const DataJsonRecElementE2&);
    //DataJsonRecElementE2::DataJsonRecElementE2FileReaderHandler loadData(const QString& filename);

    const QString getTitle();
};


//It should be improved here with iterator methods. -- eshenhu why another "SHOULD"? f*ck.
class DataJsonRecElementE2::DataJsonRecElementE2FileReaderHandler
{
public:
    DataJsonRecElementE2FileReaderHandler();

    const QVector<DataJsonRecElementE2>& data();
    void loadData(const QString filename);

private:
    QString m_filename;
    QVector<DataJsonRecElementE2> m_data;
};

#endif // DATAJSONRECELEMENT_H
