#ifndef UTILDATARECORDINGCLZ_H
#define UTILDATARECORDINGCLZ_H


class UtilDataRecordingClz
{
public:
    UtilDataRecordingClz();

public:
    // Singleton
    UtilDataRecordingClz* getInstance(){
        static UtilDataRecordingClz* p = new UtilDataRecordingClz();
        return p;
    }

    void newRec();
    void loadRec();
private:


};

#endif // UTILDATARECORDINGCLZ_H
