#include <QCoreApplication>
#include "hellostring.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    HelloString helloString;

    helloString.hellostring();

    return QCoreApplication::exec();
}


