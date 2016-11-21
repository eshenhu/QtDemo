#ifndef DATAJSONRECELEMENT_H
#define DATAJSONRECELEMENT_H

#include <QVector>
#include <QFile>
/*
 *  It was perferred use C++ template. --eshenhu
*/
/*
 * Drived class dedictated for 2 motor with elec type.
 */
class DataJsonRecElementE2
{
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

        REC_CUR1_POS,
        REC_THU1_POS,
        REC_TORQUE1_POS,
        REC_SPEED1_POS,
        REC_MOTOR1TMP1_POS,
        REC_MOTOR1TMP2_POS,
        REC_M1EFFICI1_POS,
        REC_M1EFFICI2_POS,
        REC_M1POWER,

        REC_CUR2_POS,
        REC_THU2_POS,
        REC_TORQUE2_POS,
        REC_SPEED2_POS,
        REC_MOTOR2TMP1_POS,
        REC_MOTOR2TMP2_POS,
        REC_M2EFFICI1_POS,
        REC_M2EFFICI2_POS,
        REC_M2POWER,

        ELEMCURSOR_END
    };

public:
    DataJsonRecElementE2();

    const QString toString() const;

    bool hardReset();
    bool reset();
    bool incCursor();
    bool setMetaData(quint32 vol, quint32 thro1, quint32 thro2, quint32 dis);
    bool setPosStatus(quint32);

private:
    QVector<quint32> m_data;

public:
    class DataJsonRecElementE2GetHelper;
    class DataJsonRecElementE2FileHelper;

public:
    static DataJsonRecElementE2GetHelper  m_getHelper;
    static DataJsonRecElementE2FileHelper m_fileHelper;
};


class DataJsonRecElementE2::DataJsonRecElementE2GetHelper
{
public:
    DataJsonRecElementE2GetHelper() = default;

public:
    static DataJsonRecElementE2& getElem(bool isNew = false);
};

class DataJsonRecElementE2::DataJsonRecElementE2FileHelper
{
public:
    DataJsonRecElementE2FileHelper() = default;

public:
    bool newFile(const QString& path);
    bool closeFile();
    bool writeData(const DataJsonRecElementE2&);
    //const QVector<DataJsonRecElementE2>& loadData(const QString& filename);

    const QString& getTitle();
private:
    //QString m_filename;
    QFile m_fileHandler;
};


#endif // DATAJSONRECELEMENT_H
