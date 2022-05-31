#include <QtWidgets/QApplication>
#include <QSettings>
#include <CkGlobal.h>
#include <iostream>
#include <json.hpp>

#include "patientCareLog.h"
#include "dbClient.h"

using namespace std;

QString absoluteApplicationPath;
unique_ptr<patientCareLog> logging;
QString registryDsnFolderPath("HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Centaur Software\\PatientCare\\");

void MessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        cout << "QtDebugMsg: " << msg.toStdString() << "\n";
        //qDebug() << "QtDebugMsg: " << msg << "\n";
        break;
    case QtInfoMsg:
        cout << "QtInfoMsg: " << msg.toStdString() << "\n";
        break;
    case QtWarningMsg:
        //cout << "QtWarningMsg: " << msg.toStdString() << "\n";
        break;
    case QtCriticalMsg:
        cout << "QtCriticalMsg: " << msg.toStdString() << "\n";
        break;
    case QtFatalMsg:
        cout << "QtFatalMsg: " << msg.toStdString() << "\n";
        fprintf(stdout, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        abort();
    }
}


int main(int argc, char *argv[])
{
    CkGlobal glob;
    glob.UnlockBundle("CENTAU.CB1032023_BXXvnwRxjC1N");
    qInstallMessageHandler(MessageOutput);
    QString enteredDSN;
    QString enteredDate;
    if (argc == 1 || argc > 3)
    {
        cout << "Wrong number of arguments is given,\n\
                 It can be a single argument <DSN_name>\n\
                 or\n\
                 it can be two arguments <DSN_name> <Date> as <yyyy-mm-dd>."
            << endl;
        return -110;
    }

    if (argc == 2)
    {
        enteredDSN.append(argv[1]);
        cout << "The entered DSN is " << enteredDSN.toStdString() << endl;
        enteredDate = QDate::currentDate().toString("yyyy-MM-dd");
        cout << "The Date is " << enteredDate.toStdString() << endl;
    }
    if (argc == 3)
    {
        enteredDSN.append(argv[1]);
        enteredDate.append(argv[2]);
        cout << "The entered DSN is " << enteredDSN.toStdString() << endl;
        cout << "The entered Date is " << enteredDate.toStdString() << endl;
    }

    QApplication a(argc, argv);

    logging = make_unique<patientCareLog>();

    QSettings settings(QSettings::SystemScope, "Centaur Software", "PatientCare");
    QSettings settings_for_dsn(registryDsnFolderPath + enteredDSN, QSettings::Registry32Format);

    if (!logging->openLogGUI(
        settings, 
        (enteredDSN+"\\Log").toStdString().c_str(),
        "", 
        "PatientCare log file can not be open!"))
        return -111;

    dbClient db(settings);
    if (!db.connectDatabase(enteredDSN))
    {
        QMessageBox msgBox(nullptr);
        msgBox.setText("The database can't be connected");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setFont(QFont("Calibri", 10));
        msgBox.exec();
        return -112;
    }

    nlohmann::json bindJson;
    try
    {
        bindJson = nlohmann::json::parse(settings_for_dsn.value("BindJson").toString().toStdString().c_str());
        for (nlohmann::json::iterator it = bindJson.begin(); it != bindJson.end(); ++it)
        {
            logging->log << "The location " << it.key() << " has books : " << string(it.value()) << "\n";
        }
    }
    catch (...)
    {
        QMessageBox msgBox(nullptr);
        msgBox.setText("Error in parsing location/books setting");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setFont(QFont("Calibri", 10));
        msgBox.exec();
        return -113;
    }

    string str_books;
    string str_locations;
    for (json::iterator it = bindJson.begin(); it != bindJson.end(); ++it)
    {
        if (it == bindJson.begin())
        {
            str_books.append(string(it.value()));
            str_locations.append(string(it.key()));
        }
        else
        {
            str_books.append(";").append(string(it.value()));
            str_locations.append(";").append(string(it.key()));
        }
    }

    QStringList books = QString::fromStdString(str_books).split(";");
    QStringList locations = QString::fromStdString(str_locations).split(";");

    db.connectDatabase(enteredDSN);
    {
        auto date_parts = enteredDate.split("-");
        auto reformattedDate = date_parts.at(2) + "/" +
            date_parts.at(1) + "/" +
            date_parts.at(0);

        db.createDeFollowApp();
        db.doExport(reformattedDate, reformattedDate, books);
        bool ok = db.doUpload();
        db.doFileDelete("Appointments.csv");
    }

    {   
        db.createDeClinicianDetails();
        db.doDeClinicianDetails(locations);
        bool ok = db.doUploadClinicianDetails();
        db.doFileDelete("Members.csv");
    }
    
    cout << "The extraction and upload are completed,\nprogram may be closed." << endl;
    return 0;
}
