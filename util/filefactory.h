#ifndef FILEFACTORY_H
#define FILEFACTORY_H


#include <QFile>

class FileFactory
{
public:
    FileFactory() = delete;

public:
    QFile *makeNewFile();
};

#endif // FILEFACTORY_H
