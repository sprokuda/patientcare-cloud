
#include <iomanip>
#include "dbClient.h"
#include "De_Clinician_Details.h"
#include "De_FollowApp0403.h"
#include <csv_file_headers.h>

#include<QMetaType>


using namespace std;

extern unique_ptr<patientCareLog> logging;
extern QString absoluteApplicationPath;


void log_query_result(const QString& header, const QString& msg)
{
    if (msg.isEmpty()) logging->log << header.toStdString() << ": " << "no error" << endl;
    else logging->log << header.toStdString() << ": " << "last error: " << msg.toStdString() << endl;
}


dbClient::dbClient(const QSettings& _settings, QObject* parent) : QObject(parent), m_settings(_settings)
{
    crypt.put_CryptAlgorithm(L"aes");
    crypt.put_CipherMode(L"cbc");
    crypt.put_KeyLength(128);
    crypt.put_PaddingScheme(0);

    crypt.put_EncodingMode(L"hex");
    const wchar_t* ivHex = L"000102030405060708090A0B0C0D0E0F";
    crypt.SetEncodedIV(ivHex, L"hex");
    const wchar_t* keyHex = L"000102030405060708090A0B0C0D0E0F101112131415161718191A1B1C1D1E1F";
    crypt.SetEncodedKey(keyHex, L"hex");

    auto dec_address = QString::fromWCharArray(crypt.decryptStringENC(m_settings.value("P1").toString().toStdWString().c_str()));
    auto dec_port = QString::fromWCharArray(crypt.decryptStringENC(m_settings.value("P2").toString().toStdWString().c_str()));
    auto dec_username = QString::fromWCharArray(crypt.decryptStringENC(m_settings.value("P3").toString().toStdWString().c_str()));
    auto dec_password = QString::fromWCharArray(crypt.decryptStringENC(m_settings.value("P4").toString().toStdWString().c_str()));



    chilkat = make_unique<ChilkatFTPS>(
        dec_address.toStdString().c_str(),
        dec_port.toInt(),
        dec_username.toStdString().c_str(),
        dec_password.toStdString().c_str()
        );
}

dbClient::~dbClient()
{

}

void dbClient::connectDatabase(QString dsn)
{
    db = QSqlDatabase::addDatabase("QODBC", "mydb");
    db.setDatabaseName(dsn);
    db.setUserName("dba");
    db.setPassword("sql");

#if 1
    if (!db.open())
    {
        log_query_result("SQL Database is open with", db.lastError().text());
        emit dbConnectError(db.lastError().text());
        return;
    }
#endif

    emit dbConnectSuccessful();
}

void dbClient::reOpen()
{
    if (db.isOpen())
    {
        db.close();
    }
    db = QSqlDatabase::addDatabase("QODBC", "mydb");
    db.setDatabaseName("d4w");
    db.setUserName("dba");
    db.setPassword("sql");


    if (!db.open())
    {
        log_query_result("SQL Database is reopen with", db.lastError().text());
        return;
    }

    log_query_result("SQL Database is reopen with", db.lastError().text());


}

void dbClient::updateSettings() 
{
    auto dec_address = QString::fromWCharArray(crypt.decryptStringENC(m_settings.value("P1").toString().toStdWString().c_str()));
    auto dec_port = QString::fromWCharArray(crypt.decryptStringENC(m_settings.value("P2").toString().toStdWString().c_str()));
    auto dec_username = QString::fromWCharArray(crypt.decryptStringENC(m_settings.value("P3").toString().toStdWString().c_str()));
    auto dec_password = QString::fromWCharArray(crypt.decryptStringENC(m_settings.value("P4").toString().toStdWString().c_str()));



    chilkat = make_unique<ChilkatFTPS>(
        dec_address.toStdString().c_str(),
        dec_port.toInt(),
        dec_username.toStdString().c_str(),
        dec_password.toStdString().c_str()
        );

}


void dbClient::getLocations()
{
    QSqlQuery query(db);

    query.exec("SELECT \"practice_id\",\"description\" FROM \"dba\".\"practice_locations\";");
    while (query.next())
    {
        m_locations.push_back(make_pair(query.value(0).toString(), query.value(1).toString()));
    }
//@@    log_query_result("Last error of location query: ", db.lastError().text());

    qRegisterMetaType<std::vector<pair<QString, QString>>>("std::vector<pair<QString, QString>>");
    qRegisterMetaType<vector<pair<QString, QString>>>("vector<pair<QString, QString>>");

    emit emitLocations(m_locations);
}

void dbClient::getBooks(QString book)
{
    QSqlQuery query(db);
    m_books.clear();
//    if (book == "1")
//    {
        const char* script = "SELECT app_book_number,app_book_description FROM \"dba\".\"app_books\" \
    where is_active = 1 and practice_id =%1 and app_book_number > 0;";

        query.exec(QString(script).arg(book));
        while (query.next())
        {
            m_books.push_back(make_pair(query.value(0).toString(), query.value(1).toString()));

        }
//@@        log_query_result("Last error of location query: ", db.lastError().text());
//    }
//    else
//    {
//        m_books = m_test_books;
//    }

    qRegisterMetaType<std::vector<pair<QString, QString>>>("std::vector<pair<QString, QString>>");
    qRegisterMetaType<vector<pair<QString, QString>>>("vector<pair<QString, QString>>");

    emit emitBooks(m_books);
}

void dbClient::getClinicID()
{
    const char* script =
        "Select practice_id as \"D4W Location ID\", \"trim\"(\"dba\".\"cd1_get_login\"(\"dba\".\"sf_get_param_value\"('SMS_SURGERY_ID', practice_id)))"
        " as \"PatientCare Clinic ID\",   \"practice_locations\".\"description\"  as \"PatientCare Clinic Name\" from practice_locations where is_active =1";
    QSqlQuery query(db);
    query.exec(script);

    vector<vector<QString>> clinic_ids;

    while (query.next())
    {
        clinic_ids.push_back({
            query.value(0).toString(),
            query.value(1).toString(),
            query.value(2).toString()
            });
    }

    qRegisterMetaType<std::vector<vector<QString>>>("std::vector<vector<QString>>");
    qRegisterMetaType<vector<vector<QString>>>("vector<vector<QString>>");

    emitClinicIDs(clinic_ids);
}

void dbClient::createDeFollowApp()
{
    QSqlQuery query(db);
    query.exec((string(De_FollowApp0403_1) + string(De_FollowApp0403_2)).c_str());
    log_query_result("De_FollowApp procedure is created with", db.lastError().text());
}


void dbClient::doManSync(QString start, QString end, QStringList books)
{
    if (!logging->log.is_open())
    {
        emit manSynchQueryError("Log file of ManSync operation\ncannot is not open.\nOperation cancelled.");
        return;
    }
    logging->log << "ManSync operation is started" << endl;
    reOpen();
    createDeFollowApp();
    doExport(start, end, books);
    bool ok = doUpload();
    doFileDelete("Appointments.csv");


    if (ok)
    {
        emit manSynchQueryDone();
        logging->log << "ManSync operation is done"<< endl;
    }
    else
        emit manSynchQueryError("Problem on upload happened,\nplease check log file of operation.");

}

void dbClient::doExport(QString start, QString end, QStringList books)
{
    unique_ptr<csvWriter> apps = make_unique<csvWriter>(this);
    if (!apps->openCSV("Appointments.csv", apps_header)) return;

    QStringList startL = start.split("/");
    qint64 startJ = QDate(startL.at(2).toInt(), startL.at(1).toInt(), startL.at(0).toInt()).toJulianDay();
    QStringList endL = end.split("/");
    qint64 endJ = QDate(endL.at(2).toInt(), endL.at(1).toInt(), endL.at(0).toInt()).toJulianDay();

    QSqlQuery query(db);
    for (int i = 0; i < books.size(); i++)
    {
        for (qint64 j = startJ; j <= endJ; j++)
        {
            QString date = QDate::fromJulianDay(j).toString("yyyy-MM-dd");

            QString script = QString("call \"dba\".\"de_FollowApp\"(%1,'%2');").arg(books.at(i)).arg(date);
            query.exec(script.toStdString().c_str());
            while (query.next())
            {
                vector<string> line;
                for (int k = 0; k < 20; k++)
                {
                    line.push_back(query.value(k).toString().toStdString());
                }
                apps->writeArray(line);
            }
        }
    }

}


bool dbClient::doUpload()
{
    log_query_result("The time of upload start is ", QTime::currentTime().toString());

    string remote_file_prefix = "";// QTime::currentTime().toString("hh-mm-ss").toStdString() + "_" + to_string(QDate::currentDate().dayOfWeek()) + "_";
    if (chilkat->connect())
    {
        bool ok1 = chilkat->changeRemoteDir("Aftercare");
        bool ok2 = chilkat->sendFile(absoluteApplicationPath.toStdString() + "/Export" + "\\" + "Appointments.csv", remote_file_prefix + "Appointments.csv");
        bool ok3 = chilkat->disconnect();
        return ok1 && ok2 && ok3;
    }
    else
    return false;
}


void dbClient::doFileDelete(const QString& file_to_delete)
{
    if (m_settings.value("DAU").toString().toInt())
    {
        logging->log << "Deleting of " << file_to_delete.toStdString() << " file of exported data is enabled" << endl;

        if (QFile::remove(absoluteApplicationPath + QString("\\Export\\") + file_to_delete))
            logging->log << file_to_delete.toStdString() << " file of exported data is deleted" << endl;
        else
            logging->log << "Appointments.csv file of exported data cannot be deleted for some reason" << endl;
    }
    else
    {
        logging->log << "Deleting of " << file_to_delete.toStdString() << " file of exported data NOT enabled" << endl;
    }
}


void dbClient::doSyncProv(const QStringList& list)
{
    if (!logging->log.is_open())
    {
        emit manSynchQueryError("Log file of SyncProv operation\ncannot be created.\nOperation cancelled.");
        return;
    }

    logging->log << "SyncProv operation is started"<< endl;
    reOpen();
    createDeClinicianDetails();
    doDeClinicianDetails(list);
    bool ok = doUploadClinicianDetails();
    doFileDelete("Members.csv");

    if (ok)
    {
        emit syncProvDone();
        logging->log << "SyncProv operation is done" << endl;
    }
    else
        emit syncProvError("Problem on upload happened,\nplease check log file of operation.");
}

void dbClient::createDeClinicianDetails()
{
    QSqlQuery query(db);
    query.exec(De_Clinician_Details);
    log_query_result("De_ClinicianDetails procedure is created with", db.lastError().text());
}


bool dbClient::doUploadClinicianDetails()
{
    log_query_result("The time of upload start is ", QTime::currentTime().toString());

    string remote_file_prefix = "";// QTime::currentTime().toString("hh-mm-ss").toStdString() + "_" + to_string(QDate::currentDate().dayOfWeek()) + "_";

    if (chilkat->connect())
    {
        bool ok1 = chilkat->changeRemoteDir("Members");
        bool ok2 = chilkat->sendFile(absoluteApplicationPath.toStdString() + "/Export" + "\\" + "Members.csv", remote_file_prefix + "Members.csv");
        bool ok3 = chilkat->disconnect();
        return ok1 && ok2 && ok3;
    }
    else return false;
}


void dbClient::doDeClinicianDetails(const QStringList& list)
{
    unique_ptr<csvWriter> members = make_unique<csvWriter>(this);
    if (!members->openCSV("Members.csv", members_header)) return;
    QSqlQuery query(db);

    for (int i = 0; i < list.size(); i++)
    {
        QString script = QString("call \"dba\".\"de_clinician_details\"(%1);").arg(list.at(i));
        query.exec(script.toStdString().c_str());
        while (query.next())
        {
            vector<string> line;
            for (int i = 0; i < 12; i++)
            {
                line.push_back(query.value(i).toString().toStdString());
            }
            members->writeArray(line);
        }
    }
}


