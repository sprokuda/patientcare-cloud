#include <QtWidgets/QApplication>
#include <QSettings>
#include <CkGlobal.h>
#include "PatientCareCloudWidget.h"
#include "patientCareLog.h"


QString absoluteApplicationPath;
unique_ptr<patientCareLog> logging;
QString registryDsnFolderPath("HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\ODBC\\ODBC.INI\\");

int main(int argc, char *argv[])
{
    CkGlobal glob;
    glob.UnlockBundle("CENTAU.CB1032023_BXXvnwRxjC1N");

    logging = make_unique<patientCareLog>();

    QApplication app(argc, argv);

    app.setWindowIcon(QIcon(":/PatientCareCloudWidget/resources/PatCare2.ico"));

    QSettings settings(QSettings::SystemScope, "Centaur Software", "PatientCare");

    if (!logging->openLogGUI(settings, "Config\\Log", "config", "PatientCare_Config log file can not be open!"))
        return -111;

    absoluteApplicationPath = settings.value("Path").toString();

    QSettings dsnRegistryKeys(
        "HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\ODBC\\ODBC.INI\\ODBC Data Sources",
        QSettings::Registry32Format);
    QStringList dsns = dsnRegistryKeys.allKeys();

//    dsns.removeAt(0);
//    dsns.removeAt(0);

    PatientCareCloudWidget w(settings, dsns);
    w.show();
    w.initialLoad(dsns.at(0));
    return app.exec();
}
