#pragma once

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QFont>
#include <QSettings>
#include <QPaintEvent>
#include <json.hpp>
#include "QtDatePicker.h"
#include "dbClient.h"
using namespace std;

class ManualSyncWidget : public QWidget
{
	Q_OBJECT

public:
	ManualSyncWidget(const QSettings& _settings, QWidget *parent = Q_NULLPTR);
	~ManualSyncWidget();

public slots:
	void showAdjusted();
	void onEmitManSyncDone();
	void onEmitManSyncError(QString text);
signals:
	void manSyncDone();
	void manSyncCanceled();

private:
	void adjustPosition();
	void closeEvent(QCloseEvent* event);
private slots:

	void onSyncClicked();
	void onCancelClicked();

private:
	QFont workingFont;

	QtDatePicker* startDatePicker;
	QtDatePicker* endDatePicker;

	QHBoxLayout* startLayout;
	QHBoxLayout* endLayout;

	QPushButton* syncButton;
	QPushButton* cancelButtun;
	QHBoxLayout* buttonLayout;

	const QSettings& m_settings;
	dbClient*& db;
};
