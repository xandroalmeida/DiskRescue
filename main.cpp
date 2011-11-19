#include <QtGui/QApplication>
#include "mainwindow.h"
#include "ddrescuestatus.h"
#include "ddrescuelog.h"

#include <QSettings>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("SonicIT");
    QCoreApplication::setOrganizationDomain("sonicit.com.br");
    QCoreApplication::setApplicationName("DiskRescue");
    QCoreApplication::setApplicationVersion("0.0.1");

    QSettings::setDefaultFormat(QSettings::IniFormat);

    QApplication a(argc, argv);

    qRegisterMetaType<DdrescueStatus>();
    qRegisterMetaType<DdrescueLog>();

    MainWindow w;

    w.show();

    return a.exec();
}
