#include "AdvancedSettingsWidget.h"
#include "PatientCareCloudWidget.h"

AdvancedSettingsWidget::AdvancedSettingsWidget(QSettings& _settings, QWidget *parent)
	: m_settings(_settings), QWidget(parent)
{
//	ui.setupUi(this);
    setWindowTitle(tr("PatientCare"));

    this->setFont(QFont("Calibri", 10));
    workingFont = QFont("Calibri", 10);

    headerLabel = new QLabel("Advanced Settings", this);
    headerLayout = new QHBoxLayout();

    headerLayout->addStretch();
    headerLayout->addWidget(headerLabel);
    headerLayout->addStretch();

    hostLabel = new QLabel("Host Address", this);
    hostEdit = new QLineEdit(this);
    hostEdit->setInputMask("000.000.000.000;_");

    hostLayout = new QHBoxLayout();
    hostLayout->addStretch();
    hostLayout->addWidget(hostLabel);
    hostLayout->addWidget(hostEdit);

    portLabel = new QLabel("Host port", this);
    portEdit = new QLineEdit(this);

    portLayout = new QHBoxLayout();
    portLayout->addStretch();
    portLayout->addWidget(portLabel);
    portLayout->addWidget(portEdit);

    usernameLabel = new QLabel("Username", this);
    usernameEdit = new QLineEdit(this);

    usernameLayout = new QHBoxLayout();
    usernameLayout->addStretch();
    usernameLayout->addWidget(usernameLabel);
    usernameLayout->addWidget(usernameEdit);

    passwordLabel = new QLabel("Password", this);
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);

    passwordLayout = new QHBoxLayout();
    passwordLayout->addStretch();
    passwordLayout->addWidget(passwordLabel);
    passwordLayout->addWidget(passwordEdit);

    clinicIDsButton = new QPushButton("Clinic ID", this);
    saveButton = new QPushButton("Save", this);
    cancelButton = new QPushButton("Cancel", this);

    buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(clinicIDsButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(saveButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(cancelButton);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    int l, t, r, b;
    mainLayout->getContentsMargins(&l, &t, &r, &b);
    mainLayout->addSpacing(t / 2);
    mainLayout->addLayout(headerLayout);
    mainLayout->addSpacing(t / 2);
    mainLayout->addLayout(hostLayout);
    mainLayout->addSpacing(t / 2);
    mainLayout->addLayout(portLayout);
    mainLayout->addSpacing(t / 2);
    mainLayout->addLayout(usernameLayout);
    mainLayout->addSpacing(t / 2);
    mainLayout->addLayout(passwordLayout);
    mainLayout->addSpacing(t / 2);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addSpacing(t / 2);

    setLayout(mainLayout);

    installEventFilter(this);

    this->setWindowFlags(Qt::Window);

    clinicIDDialog = new QDialog(this);
    clinicIDTable = new QTableWidget(0,3,clinicIDDialog);
    clinicIDDialogButton = new QPushButton("Ok",clinicIDDialog);
    clinicIDDialogButtonlayout = new QHBoxLayout();
    QVBoxLayout* clinicLayout = new QVBoxLayout(clinicIDDialog);

    qsl << QString("D4W Location ID") << QString("PatientCare Clinic ID") << QString("PatientCare Clinic Name");
    clinicIDTable->setHorizontalHeaderLabels(qsl);

    clinicIDDialogButtonlayout->addStretch();
    clinicIDDialogButtonlayout->addWidget(clinicIDDialogButton);
    clinicIDDialogButtonlayout->addStretch();
    clinicIDDialog->setLayout(clinicLayout);
    clinicLayout->addWidget(clinicIDTable);
    clinicLayout->addLayout(clinicIDDialogButtonlayout);

    connect(clinicIDsButton, SIGNAL(clicked()), this, SLOT(onClinicIDsClicked()));
    connect(clinicIDDialogButton, &QPushButton::clicked, [&](){clinicIDDialog->hide(); });
    connect(saveButton, SIGNAL(clicked()), this, SLOT(onSaveClicked()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(onCancelClicked()));

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
    hostEdit->setText(dec_address);
    auto dec_port = QString::fromWCharArray(crypt.decryptStringENC(m_settings.value("P2").toString().toStdWString().c_str()));
    portEdit->setText(dec_port);
    auto dec_username = QString::fromWCharArray(crypt.decryptStringENC(m_settings.value("P3").toString().toStdWString().c_str()));
    usernameEdit->setText(dec_username);
    auto dec_password = QString::fromWCharArray(crypt.decryptStringENC(m_settings.value("P4").toString().toStdWString().c_str()));
    passwordEdit->setText(dec_password);

    auto& db = dynamic_cast<PatientCareCloudWidget*>(this->parentWidget())->getClientDb();
    connect(&db, SIGNAL(emitClinicIDs(vector<vector<QString>>)), this, SLOT(onEmitClinicIDs(vector<vector<QString>>)));
}

AdvancedSettingsWidget::~AdvancedSettingsWidget()
{
    delete headerLayout;
    delete hostLayout;
    delete portLayout;
    delete usernameLayout;
    delete passwordLayout;
    delete buttonLayout;
    delete clinicIDDialogButtonlayout;

    for (auto it = tableItems.begin(); it != tableItems.end(); it++)
    {
        if (*it) delete* it;
    }

    tableItems.clear();
}

void AdvancedSettingsWidget::showErrorDialog(const QString& capture)
{
    QMessageBox msgBox(this);
    msgBox.setText(capture);
    msgBox.setIcon(QMessageBox::Warning);
    auto* ok = msgBox.addButton("Back", QMessageBox::ActionRole);

    msgBox.setFont(workingFont);

    msgBox.exec();
}

void AdvancedSettingsWidget::onClinicIDsClicked()
{
    auto& db = dynamic_cast<PatientCareCloudWidget*>(this->parentWidget())->getClientDb();
    QMetaObject::invokeMethod(&db, "getClinicID", Qt::QueuedConnection);
}


void AdvancedSettingsWidget::onEmitClinicIDs(vector<vector<QString>> v)
{
    QString text;  
    for (auto it = tableItems.begin(); it != tableItems.end(); it++)
    {
        if (*it) delete *it;
    }
    tableItems.clear();

    clinicIDTable->setRowCount(0);
    int i(0), j (0);

    QTableWidgetItem* table_item;
    for (auto it = v.begin(); it != v.end(); it++)
    {
        auto& item = *it;
        clinicIDTable->insertRow(i);
        for (auto it1 = item.begin(); it1 != item.end(); it1++)
        {
            table_item = new QTableWidgetItem(*it1);
            tableItems.push_back(table_item);
            clinicIDTable->setItem(i, j, table_item);
            j++;
         }
        j = 0;
        i++;
    }
    clinicIDDialog->show();
}

void AdvancedSettingsWidget::onSaveClicked()
{
    QStringList raw_address = hostEdit->text().split(".");// .remove("_");
    for (auto it = raw_address.begin(); it != raw_address.end(); it++)
    {
        if (it->isEmpty())
        {
            showErrorDialog("The host IP address may not\ncontain an empty field");
            return;
        }
        else
        {
            if (it->toInt() > 255)
            {
                showErrorDialog("The host IP address may not\ncontain a value more that 255");
                return;
            }
        }
    }
    bool ok;
    int port = portEdit->text().toInt(&ok);
    if (port < 1 || port >65535)
    {
        showErrorDialog("The host port should be in range\nfrom 1 to 65535");
        return;
    }

    if (usernameEdit->text().isEmpty())
    {
        showErrorDialog("The Username can't be empty");
        return;
    }

    const wchar_t* enc_address = crypt.encryptStringENC(raw_address.join(".").toStdWString().c_str());
    const wchar_t* enc_port = crypt.encryptStringENC(portEdit->text().toStdWString().c_str());
    const wchar_t* enc_username = crypt.encryptStringENC(usernameEdit->text().toStdWString().c_str());
    const wchar_t* enc_password = crypt.encryptStringENC(passwordEdit->text().toStdWString().c_str());

    m_settings.setValue("P1", QString::fromStdWString(enc_address)); //portEdit->text().toStdString().c_str()
    m_settings.setValue("P2", QString::fromStdWString(enc_port));
    m_settings.setValue("P3", QString::fromStdWString(enc_username));
    m_settings.setValue("P4", QString::fromStdWString(enc_password));

    emit advancedSettingsSaved();
}

void AdvancedSettingsWidget::onCancelClicked()
{
    hide();
    emit advancedSettingsCanceled();
}


void AdvancedSettingsWidget::closeEvent(QCloseEvent* event)
{
    emit advancedSettingsCanceled();
}

void AdvancedSettingsWidget::showAdjusted()
{
    show();
    adjustPosition();
}


void AdvancedSettingsWidget::adjustPosition()
{
    QRect rect = this->rect();
    QPoint top_left = this->parentWidget()->mapToGlobal(rect.topLeft());
    this->move(top_left.x() + this->parentWidget()->width()/2 - this->width()/2, top_left.y());
}


