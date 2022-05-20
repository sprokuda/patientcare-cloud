#pragma once

#include <QWidget>

#include <QtWidgets/QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QLayout>
#include <QCombobox>
#include <QCheckbox>
#include <QPushButton>
#include <QTimeEdit>
#include <QMessageBox>
#include <QFont>
#include <QSettings>
#include <QProcess>

#include "AdvancedSettingsWidget.h"
#include "ManualSyncWidget.h"
#include "QtMultiSelect.h"
#include "QtSingleSelect.h"
#include "QtSingleSelectPopup.h"
#include "dbClient.h"


class PatientCareCloudWidget : public QWidget
{
    Q_OBJECT
        friend class QtSingleSelect;
    friend class QtSingleSelectPopup;
    friend class AdvancedSettingsWidget;
    friend class ManualSyncWidget;
public:
    PatientCareCloudWidget(QSettings& _settings, const QStringList& _dsns, QWidget* parent = Q_NULLPTR);
    ~PatientCareCloudWidget();

    void initialLoad(QString dsn);
    dbClient& getClientDb() { return *db; };
    dbClient*& getClient() { return db; };
private:

    QString getLocationNumber(const QString& location_name);
    QString getBooksNumbers(const QStringList& books_names);

private slots:
    void onDbconnectError(QString message);
    void onDbSuccessful(QString message);

    void onEmitLocations(vector<pair<QString, QString>> v);
    void onEmitBooks(vector<pair<QString, QString>> v);

    void onEmitDsnSelected(QString item_text);
    void onEmitLocationSelected(QString item_text);

    void onResetButtonClicked();
    void onAdvButtonClicked();
    void onManButtonClicked();
    void onSyncProvButtonClicked();
    void onEmitSyncProvDone();
    void onEmitSyncProvError(QString text);
    void onSaveButtonClicked();
    void onExitButtonClicked();
    void doSeviceRestart();

    void closeEvent(QCloseEvent* event);


private:

    QFont workingFont;
    QSettings& m_settings;

    AdvancedSettingsWidget* asw;
    ManualSyncWidget* msw;
    dbClient* db;
    QThread* thread;

    const QStringList m_dsns;
    QString m_lastConnectedDsn;
    vector<pair<QString, QString>> m_books;
    vector<pair<QString, QString>> m_locations;
    map<QCheckBox*, QString> m_checks;
    map<string, string> m_bind;// = { {"1",{"1","2","3"}},{"4",{"5","6"}},{"7",{"8","9","0"}} };
    QString selectedLocation;

    QLabel* dsnLabel;
    QtSingleSelect* dsnSelect;

    QLabel* locationsLabel;
    QtSingleSelect* locationsSelect;

    QLabel* booksLabel;
    QtMultiSelect* booksSelect;

    QVBoxLayout* headerLayout;

    QHBoxLayout* boxesLayout;
    QHBoxLayout* synchLayout;

    QPushButton* resetButton;
    QPushButton* advButton;
    QPushButton* manButton;
    QPushButton* syncProvButton;
    QPushButton* saveButton;
    QPushButton* exitButton;
    QHBoxLayout* buttonLayout1;
    QHBoxLayout* buttonLayout2;
};
