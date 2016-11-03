#include "filefactory.h"
#include <QtCore>

//FileFactory::FileFactory()
//{

//}

QFile *FileFactory::makeNewFile()
{
    //QString str = QCoreApplication::applicationDirPath() + '/data';
    return new QFile("in.csv");
}
