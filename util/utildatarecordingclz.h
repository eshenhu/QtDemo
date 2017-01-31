#ifndef UTILDATARECORDINGCLZ_H
#define UTILDATARECORDINGCLZ_H

#include <QString>

class UtilDataRecordingClz
{
public:
    UtilDataRecordingClz();

public:
    // Singleton
    static UtilDataRecordingClz& getInstance(){
        static UtilDataRecordingClz v;
        return v;
    }

    bool newRec();
    //bool closeRec();

    const QString& getCfgFileName() const;
    const QString& getRecFileName() const;

    QString getTimeStamp() const;

private:
    QString m_cfgFile;
    QString m_recFile;

    QString m_timeStamp;
};

#endif // UTILDATARECORDINGCLZ_H
