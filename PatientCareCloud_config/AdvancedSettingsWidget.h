#pragma once

#include <QWidget>

#include <QtWidgets/QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QFont>
#include <QSettings>
#include <QTableWidget>
#include <QStringList>
#include <QDialog>

#include <vector>

#include <CkCrypt2W.h>

#include "dbClient.h"

//#include "ui_AdvancedSettings.h"

class AdvancedSettingsWidget : public QWidget
{
	Q_OBJECT

public:
	AdvancedSettingsWidget(QSettings& _settings, QWidget *parent = Q_NULLPTR);
	~AdvancedSettingsWidget();

public slots:
	void showAdjusted();

signals:
	void advancedSettingsSaved();
	void advancedSettingsCanceled();

private slots:

	void onClinicIDsClicked();
	void onEmitClinicIDs(vector<vector<QString>> v);
	void onSaveClicked();
	void onCancelClicked();
	void closeEvent(QCloseEvent* event);
private:
	void showErrorDialog(const QString& capture);
	void adjustPosition();

private:
//	Ui::AdvancedSettings ui;
	CkCrypt2W crypt;
	QSettings& m_settings;
	QFont workingFont;
//	dbClient*& db ;

	QLabel* headerLabel;
	QHBoxLayout* headerLayout;

	QLabel* hostLabel;
	QLineEdit* hostEdit;
	QHBoxLayout* hostLayout;

	QLabel* portLabel;
	QLineEdit* portEdit;
	QHBoxLayout* portLayout;

	QLabel* usernameLabel;
	QLineEdit* usernameEdit;
	QHBoxLayout* usernameLayout;

	QLabel* passwordLabel;
	QLineEdit* passwordEdit;
	QHBoxLayout* passwordLayout;

	QPushButton* clinicIDsButton;
	QPushButton* saveButton;
	QPushButton* cancelButton;
	QHBoxLayout* buttonLayout;

	QDialog* clinicIDDialog;
	QTableWidget* clinicIDTable;
	QStringList qsl;
	QPushButton* clinicIDDialogButton;
	QHBoxLayout* clinicIDDialogButtonlayout;
	vector<QTableWidgetItem*> tableItems;
};
