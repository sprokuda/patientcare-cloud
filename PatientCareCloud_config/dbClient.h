#pragma once

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QVariant>
#include <QDateTime>
#include <QSettings>
#include <QProcess>

#include <QMessageBox>
#include <QPushButton>
#include <QAbstractButton>

#include <iostream>
#include <map>
#include <vector>


#include <CkCrypt2W.h>

#include "chilkatFTPS.h"
#include "csvWriter.h"
#include "patientCareLog.h"

#include <json.hpp>
using namespace nlohmann;
using namespace std;

void log_query_result(const QString& header, const QString& msg);

class dbClient : public QObject
{
    Q_OBJECT
public:
    dbClient(const QSettings& _settings, QObject* parent = nullptr);
    ~dbClient();

    Q_INVOKABLE void connectDatabase(QString dsn);
    Q_INVOKABLE void updateSettings(QString dsn);
    Q_INVOKABLE void getLocations();
    Q_INVOKABLE void getBooks(QString book);
    Q_INVOKABLE void getClinicID();

    void createDeFollowApp();
    Q_INVOKABLE void doManSync(QString start, QString end, QStringList books);
    void doExport(QString start, QString end, QStringList books);
    bool doUpload();
    void doFileDelete(const QString& file_to_delete);

    Q_INVOKABLE void doSyncProv(const QStringList& list);
    void createDeClinicianDetails();
    bool doUploadClinicianDetails();
    void doDeClinicianDetails(const QStringList& list);
    void reOpen(QString dsn);

signals:

    void dbConnectError(QString message);
    void dbConnectSuccessful(QString message);
    void emitLocations(const vector<pair<QString, QString>>& m);
    void emitBooks(const vector<pair<QString, QString>>& m);
    void emitClinicIDs(vector<vector<QString>> clinic_ids);
    void manSynchQueryDone();
    void manSynchQueryError(QString text);
    void syncProvDone();
    void syncProvError(QString text);
    void serviceRestartSuccess();
    void serviceRestartFailed();

private:
    QSqlDatabase db;
    QString m_lastDsn;
    const QSettings& m_settings;
    CkCrypt2W crypt;
    vector<pair<QString, QString>> m_locations;
    vector<pair<QString, QString>> m_books;

    vector<pair<QString, QString>> m_test_books;

    unique_ptr<ChilkatFTPS> chilkat;

};


