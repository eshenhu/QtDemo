#ifndef UTILDATARECORDINGCLZ_H
#define UTILDATARECORDINGCLZ_H

#include <QString>
#include "ui/uiheader.h"

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

    bool newRec(TestPlanEnum plan);
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
