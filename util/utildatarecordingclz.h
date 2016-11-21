#ifndef UTILDATARECORDINGCLZ_H
#define UTILDATARECORDINGCLZ_H

#include <QString>

class UtilDataRecordingClz
{
public:
    UtilDataRecordingClz();

public:
    // Singleton
    UtilDataRecordingClz getInstance(){
        static UtilDataRecordingClz v;
        return v;
    }

    bool newRec();
    //bool closeRec();

    const QString& getCfgFileName() const;
    const QString& getRecFileName() const;

private:
    QString m_cfgFile;
    QString m_recFile;
};

#endif // UTILDATARECORDINGCLZ_H
