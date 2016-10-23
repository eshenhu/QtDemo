#ifndef FILEASSIST_H
#define FILEASSIST_H


class FileHelper
{
public:
    FileHelper();
    ~FileHelper();

public:
    int init();
    int close();
    void saveData();
};



#endif // FILEASSIST_H
