#include "commandrunner.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("mx");
    QCoreApplication::setOrganizationDomain("mx");
    QCoreApplication::setApplicationName("CommandRunner");

    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);
    CommandRunner w;


    return a.exec();
}
