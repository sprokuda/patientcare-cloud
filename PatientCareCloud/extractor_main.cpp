#include <QtWidgets/QApplication>
#include <QSettings>
#include <CkGlobal.h>

#include "patientCareLog.h"

using namespace std;

QString absoluteApplicationPath;
unique_ptr<patientCareLog> logging;
QString registryDsnFolderPath("HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\ODBC\\ODBC.INI\\");

int main(int argc, char *argv[])
{
    CkGlobal glob;
    glob.UnlockBundle("CENTAU.CB1032023_BXXvnwRxjC1N");

    QApplication a(argc, argv);

    logging = make_unique<patientCareLog>();

    //a.setWindowIcon(QIcon(":/PatientCareCloudWidget/resources/PatCare2.ico"));

    QSettings settings(QSettings::SystemScope, "Centaur Software", "PatientCare");

    if (!logging->openLogGUI(settings, "AU12345\\Log", "", "PatientCare log file can not be open!"))
        return -111;

    return a.exec();
}
