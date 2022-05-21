#include "PatientCareCloudWidget.h"

#include <json.hpp>

extern QString absoluteApplicationPath;
extern QString registryDsnFolderPath;

PatientCareCloudWidget::PatientCareCloudWidget(QSettings& _settings,const QStringList& _dsns, QWidget* parent)
    : m_settings(_settings), m_dsns(_dsns), m_lastConnectedDsn(""), QWidget(parent)
{
    db = new dbClient(_settings, this);
    thread = new QThread();

    asw = new AdvancedSettingsWidget(_settings, this);

    msw = new ManualSyncWidget(_settings, this);

    setWindowTitle(tr("PatientCare Sync Configuration"));

    workingFont = QFont("Calibri", 10);
    this->setFont(workingFont);

    dsnLabel = new QLabel("DSN", this);
    dsnSelect = new QtSingleSelect(this);

    dsnSelect->getPopup().setTable(m_dsns);

    locationsLabel = new QLabel("Location", this);
    locationsSelect = new QtSingleSelect(this);
    //       QStringList locations = { "location1","location2","location3" };
    //       locationSelect->getPopup().setTable(locations);
    //       locationSelect->selectFirstBook();

    booksLabel = new QLabel("Appointment Books", this);
    booksSelect = new QtMultiSelect(this);
    /*       QStringList books = { "book1","book2","book3"};
           booksSelect->getPopup().setTable(books);
           booksSelect->selectAllBooks();*/

    headerLayout = new QVBoxLayout();

    headerLayout->addStretch();
    headerLayout->addWidget(dsnLabel);
    headerLayout->addWidget(dsnSelect);
    headerLayout->addStretch();
    headerLayout->addWidget(locationsLabel);
    headerLayout->addWidget(locationsSelect);
    headerLayout->addStretch();
    headerLayout->addWidget(booksLabel);
    headerLayout->addWidget(booksSelect);
    headerLayout->addStretch();


    resetButton = new QPushButton("Reset", this);
    advButton = new QPushButton("Adv Settings", this);
    manButton = new QPushButton("Man Sync", this);
    syncProvButton = new QPushButton("Sync Prov", this);
    saveButton = new QPushButton("Save", this);
    exitButton = new QPushButton("Exit", this);

    buttonLayout1 = new QHBoxLayout();
    buttonLayout1->addWidget(resetButton);
    buttonLayout1->addStretch();
    buttonLayout1->addWidget(advButton);
    buttonLayout1->addStretch();
    buttonLayout1->addWidget(manButton);
    buttonLayout1->addStretch();
    buttonLayout1->addWidget(syncProvButton);

    buttonLayout2 = new QHBoxLayout();
    buttonLayout2->addStretch();
    buttonLayout2->addWidget(saveButton);
    buttonLayout2->addStretch();
    buttonLayout2->addWidget(exitButton);
    buttonLayout2->addStretch();

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    int l, t, r, b;
    mainLayout->getContentsMargins(&l, &t, &r, &b);
    mainLayout->addSpacing(t / 2);
    mainLayout->addLayout(headerLayout);
    mainLayout->addSpacing(t / 2);
    mainLayout->addLayout(buttonLayout1);
    mainLayout->addSpacing(t / 2);
    mainLayout->addLayout(buttonLayout2);
    mainLayout->addSpacing(t / 2);

    setLayout(mainLayout);

    db->moveToThread(thread);
    thread->start();

    connect(resetButton, &QPushButton::clicked, this, &PatientCareCloudWidget::onResetButtonClicked);
    connect(advButton, &QPushButton::clicked, this, &PatientCareCloudWidget::onAdvButtonClicked);
    connect(manButton, &QPushButton::clicked, this, &PatientCareCloudWidget::onManButtonClicked);
    connect(syncProvButton, &QPushButton::clicked, this, &PatientCareCloudWidget::onSyncProvButtonClicked);
    connect(saveButton, &QPushButton::clicked, this, &PatientCareCloudWidget::onSaveButtonClicked);
    connect(exitButton, &QPushButton::clicked, this, &PatientCareCloudWidget::onExitButtonClicked);

    connect(dsnSelect, SIGNAL(emitItemSelected(QString)), this, SLOT(onEmitDsnSelected(QString)));

    connect(locationsSelect, SIGNAL(emitItemSelected(QString)), this, SLOT(onEmitLocationSelected(QString)));

    connect(asw, &AdvancedSettingsWidget::advancedSettingsSaved, [&]() {doSeviceRestart(); });
    connect(asw, &AdvancedSettingsWidget::advancedSettingsCanceled, [&]() {asw->hide(); setEnabled(true); });

    connect(msw, &ManualSyncWidget::manSyncDone, [&]() {msw->hide(); setEnabled(true); });
    connect(msw, &ManualSyncWidget::manSyncCanceled, [&]() {msw->hide(); setEnabled(true); });

    void serviceRestartSuccess();
    void serviceRestartFailed();
    connect(db, SIGNAL(dbConnectError(QString)), this, SLOT(onDbconnectError(QString)));
    connect(db, SIGNAL(dbConnectSuccessful(QString)), this, SLOT(onDbSuccessful(QString)));
    connect(db, SIGNAL(emitLocations(vector<pair<QString, QString>>)), this, SLOT(onEmitLocations(vector<pair<QString, QString>>)));
    connect(db, SIGNAL(emitBooks(vector<pair<QString, QString>>)), this, SLOT(onEmitBooks(vector<pair<QString, QString>>)));
    connect(db, SIGNAL(syncProvDone()), this, SLOT(onEmitSyncProvDone()));
    connect(db, SIGNAL(syncProvError(QString)), this, SLOT(onEmitSyncProvError(QString)));

 

}

PatientCareCloudWidget::~PatientCareCloudWidget()
{
    thread->quit();
    thread->wait();
    delete db;

    delete headerLayout;

    delete buttonLayout1;
    delete buttonLayout2;
}

void PatientCareCloudWidget::initialLoad(QString dsn)
{
    QMetaObject::invokeMethod(db, "connectDatabase", Qt::QueuedConnection, Q_ARG(QString, dsn));
}


void PatientCareCloudWidget::onDbconnectError(QString message)
{
    QString returned_dsn = message.split(":").at(0);
    QMessageBox msgBox(this);
    msgBox.setText(message);
    msgBox.setIcon(QMessageBox::Warning);
    auto* reconnect = msgBox.addButton("Reconnect", QMessageBox::ActionRole);
    QPushButton* connect_prev = nullptr;

    QString dsn_to_reconnect;
    if (!m_lastConnectedDsn.isEmpty())
    {
        connect_prev = msgBox.addButton("Connect to last successfull", QMessageBox::ActionRole);
        dsn_to_reconnect = m_lastConnectedDsn;
    }
    else
    {
        connect_prev = msgBox.addButton("Connect some other...", QMessageBox::ActionRole);
        int i = rand() % m_dsns.size();
        dsn_to_reconnect = m_dsns.at(i);
    }
    auto* exit = msgBox.addButton("Exit Program", QMessageBox::ActionRole);

    msgBox.exec();

    if (msgBox.clickedButton() == reconnect)
    {
        initialLoad(returned_dsn);
    }

    if (msgBox.clickedButton() == connect_prev)
    {
        initialLoad(dsn_to_reconnect);
    }

    else if (msgBox.clickedButton() == exit)
    {
        qApp->quit();
    }
}

void PatientCareCloudWidget::onDbSuccessful(QString message)
{
    m_lastConnectedDsn = message;
    dsnSelect->selectItem(message);
    QMetaObject::invokeMethod(db, "setWorkingDir", Qt::DirectConnection, Q_ARG(QString, "Config"));
    QMetaObject::invokeMethod(db, "getLocations", Qt::QueuedConnection);
}

void PatientCareCloudWidget::onEmitLocations(vector<pair<QString, QString>> v)
{
    m_locations = v;
    QStringList list;
    for (auto it = v.begin(); it != v.end(); it++)
    {
        list << it->second;
    }

    locationsSelect->getPopup().setTable(list);
    locationsSelect->selectFirstItem();
    selectedLocation = v.at(0).first;
    QMetaObject::invokeMethod(db, "getBooks", Qt::QueuedConnection, Q_ARG(QString, m_locations.front().first));
}

void PatientCareCloudWidget::onEmitBooks(vector<pair<QString, QString>> v)
{
    m_books = v;
    QStringList list;
    QStringList loaded_books;
    QStringList books_to_be_checked;
    map<string, string> stored_books;

    QSettings settings_for_dsn(registryDsnFolderPath + m_lastConnectedDsn, QSettings::Registry32Format);
    auto read_from_registry = settings_for_dsn.value("BindJson").toString().toStdString();
#if 0
    QMessageBox msgBox(this);
    msgBox.setText(QString("json from registry: ") + QString::fromStdString(read_from_registry));
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setFont(workingFont);
    msgBox.exec();
#endif

    nlohmann::json j;
    if (!read_from_registry.empty())
    {
        try
        {
            j = nlohmann::json::parse(read_from_registry.c_str());
        }
        catch (...)
        {
            QMessageBox msgBox(this);
            msgBox.setText("Error in parsing location/books setting");
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setFont(workingFont);
            msgBox.exec();
            return;
        }
    }
    else
    {
#if 0
        QMessageBox msgBox(this);
        msgBox.setText(QString("json from registry is empty"));
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setFont(workingFont);
        msgBox.exec();
#endif
    }

    for (nlohmann::json::iterator it = j.begin(); it != j.end(); ++it)
    {
        if (QString(it.key().c_str()) == selectedLocation)
            loaded_books = QString::fromStdString(it.value()).split(";");
    }

    for (auto it = m_books.begin(); it != m_books.end(); it++)
    {
        list << it->second;
        if (loaded_books.contains(it->first)) books_to_be_checked << it->second;
    }

    booksSelect->getPopup().setTable(list);
    booksSelect->setText("");
    booksSelect->selectBooks(books_to_be_checked);
}

void PatientCareCloudWidget::onEmitDsnSelected(QString item_text)
{
    initialLoad(item_text);
}

void PatientCareCloudWidget::onEmitLocationSelected(QString item_text)
{

    QStringList list;


    for (auto& item : m_locations)
    {
        if (item.second == item_text)
        {
            selectedLocation = item.first;
            QMetaObject::invokeMethod(db, "getBooks", Qt::QueuedConnection, Q_ARG(QString, item.first));

        }
    }
}


QString PatientCareCloudWidget::getLocationNumber(const QString& location_name)
{
    for (auto it = m_locations.begin(); it != m_locations.end(); it++)
    {
        if (it->second == location_name) return it->first;
    }
    return QString("");

}

QString PatientCareCloudWidget::getBooksNumbers(const QStringList& books_names)
{
    QStringList result;
    int j = 0;
    for (auto it = m_books.begin(); it != m_books.end(); it++)
    {
        for (int i = j; i < books_names.size(); i++)
        {
            if (it->second == books_names.at(i))
            {
                result << it->first;
                j = i;
                break;
            }
        }
    }
    return result.join(";");
}


void PatientCareCloudWidget::onResetButtonClicked()
{
    QMessageBox msgBox(this);
    msgBox.setText("This will clear all system settings\nincluding the ftp credentials.\nAre you sure you want to proceed?");
    msgBox.setIcon(QMessageBox::Information);
    auto* ok = msgBox.addButton("Confirm", QMessageBox::ActionRole);
    auto* cancel = msgBox.addButton(QMessageBox::Cancel);

    msgBox.setFont(workingFont);

    msgBox.exec();

    if (msgBox.clickedButton() == ok)
    {
        m_bind.clear();
        QSettings settings_for_dsn(registryDsnFolderPath + m_lastConnectedDsn, QSettings::Registry32Format);

        settings_for_dsn.setValue("BindJson", "");
        settings_for_dsn.setValue("P1", "");
        settings_for_dsn.setValue("P2", "");
        settings_for_dsn.setValue("P3", "");
        settings_for_dsn.setValue("P4", "");

        QMetaObject::invokeMethod(db, "getLocations", Qt::QueuedConnection);

    }
    else if (msgBox.clickedButton() == cancel)
    {
        return;
    }

    QMessageBox msgBox1(this);
    msgBox1.setText("Settings are cleared");
    msgBox1.setIcon(QMessageBox::Information);
    msgBox1.setFont(workingFont);
    msgBox1.exec();

}

void PatientCareCloudWidget::onAdvButtonClicked()
{
    this->setEnabled(false);
    asw->loadFromRegistry(m_lastConnectedDsn);
    asw->setEnabled(true);
    asw->showAdjusted();

}

void PatientCareCloudWidget::onManButtonClicked()
{
    this->setEnabled(false);
    msw->setEnabled(true);
    msw->showAdjusted();
}

void PatientCareCloudWidget::onSyncProvButtonClicked()
{
    QSettings settings_for_dsn(registryDsnFolderPath + m_lastConnectedDsn, QSettings::Registry32Format);
    QString settings_str = settings_for_dsn.value("BindJson").toString();
    if (settings_str.isEmpty())
    {
        QMessageBox msgBox(this);
        msgBox.setText("Empty value in location/books setting");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setFont(workingFont);
        msgBox.exec();
        return;
    }

    nlohmann::json j;
    try
    {
        j = nlohmann::json::parse(m_settings.value("bind_json").toString().toStdString().c_str());
    }
    catch (...)
    {
        QMessageBox msgBox(this);
        msgBox.setText("Error in parsing location/books setting");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setFont(workingFont);
        msgBox.exec();
        return;
    }
    string str;
    for (json::iterator it = j.begin(); it != j.end(); ++it)
    {
        if (it == j.begin())
            str.append(string(it.key()));
        else
            str.append(";").append(string(it.key()));
    }
    QStringList list = QString::fromStdString(str).split(";");

    QMetaObject::invokeMethod(db, "updateSettings", Qt::DirectConnection,
        Q_ARG(QString, m_lastConnectedDsn));

    QMetaObject::invokeMethod(db, "doSyncProv", Qt::QueuedConnection,
        Q_ARG(QStringList, list)
    );
}


void PatientCareCloudWidget::onEmitSyncProvDone()
{
    QMessageBox msgBox(this);
    msgBox.setText("Sync prov\nis done");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setFont(workingFont);
    msgBox.exec();

    this->setEnabled(true);
}

void PatientCareCloudWidget::onEmitSyncProvError(QString text)
{
    QMessageBox msgBox(this);
    msgBox.setText(text);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setFont(workingFont);
    msgBox.exec();

    this->setEnabled(true);
}

void PatientCareCloudWidget::onSaveButtonClicked()
{
    setEnabled(false);

    auto location_number = getLocationNumber(locationsSelect->getCheckedName()).toStdString();
    auto books_numbers = getBooksNumbers(booksSelect->getCheckedNames()).toStdString();

    if (m_bind.find(location_number) == m_bind.end())
        m_bind.emplace(location_number, books_numbers);
    else
        m_bind.at(location_number) = books_numbers;

    auto read_from_registry = m_settings.value("bind_json").toString().toStdString();

    nlohmann::json j;
    if (!read_from_registry.empty())
    {
        try
        {
            j = nlohmann::json::parse(read_from_registry.c_str());
        }
        catch (...)
        {
            QMessageBox msgBox(this);
            msgBox.setText("Error in parsing location/books setting");
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setFont(workingFont);
            msgBox.exec();
            return;
        }
    }


    for (auto it = m_bind.begin(); it != m_bind.end(); it++)
    {
        j[it->first.c_str()] = it->second.c_str();
    }

    QSettings settings_for_dsn(registryDsnFolderPath + m_lastConnectedDsn, QSettings::Registry32Format);
    settings_for_dsn.setValue("BindJson", QString(j.dump().c_str()).remove("\\"));

    QMessageBox msgBox(this);
    msgBox.setText("Settings are saved");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setFont(workingFont);
    msgBox.exec();

    setEnabled(true);
}

void PatientCareCloudWidget::doSeviceRestart()
{
    asw->hide();
    QMetaObject::invokeMethod(db, "updateSettings", Qt::DirectConnection);
    setEnabled(true);
}



void PatientCareCloudWidget::onExitButtonClicked()
{
    QMessageBox msgBox(this);
    msgBox.setText("Confirm you want to exit program");
    msgBox.setIcon(QMessageBox::Warning);
    auto* ok = msgBox.addButton("Exit Program", QMessageBox::ActionRole);
    auto* cancel = msgBox.addButton(QMessageBox::Cancel);

    msgBox.setFont(workingFont);

    msgBox.exec();

    if (msgBox.clickedButton() == ok)
    {
        qApp->quit();
    }
    else if (msgBox.clickedButton() == cancel)
    {
        return;
    }

}

void PatientCareCloudWidget::closeEvent(QCloseEvent* event)
{
    onExitButtonClicked();
}

