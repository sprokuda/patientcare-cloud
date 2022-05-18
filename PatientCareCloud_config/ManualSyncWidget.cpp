#include "PatientCareCloudWidget.h"
#include "ManualSyncWidget.h"

ManualSyncWidget::ManualSyncWidget(const QSettings& _settings, QWidget *parent)
	: QWidget(parent), 
	m_settings(_settings),
	db(dynamic_cast<PatientCareCloudWidget*>(this->parentWidget())->getClient())

{
	setWindowTitle(tr("PatientCare"));

	this->setFont(QFont("Calibri", 10));
	workingFont = QFont(QFont("Calibri", 10));

	QDate endDate = QDate::currentDate();
	QDate startDate = QDate::fromJulianDay(endDate.toJulianDay() - 2);

	QLabel* startLabel = new QLabel("Start Date", this);
	startDatePicker = new QtDatePicker(this);
	startDatePicker->getCalendar().setMinimumDate(startDate);
	startDatePicker->getCalendar().setMaximumDate(endDate);
	startDatePicker->setDate(startDate);
//	startDatePicker->getCalendar().setSelectedDate(startDate);
	startLayout = new QHBoxLayout();
	startLayout->addStretch();
	startLayout->addWidget(startLabel);
	startLayout->addWidget(startDatePicker);


	QLabel* endLabel = new QLabel("End Date", this);
	endDatePicker = new QtDatePicker(this);
	endDatePicker->getCalendar().setMinimumDate(startDate);
	endDatePicker->getCalendar().setMaximumDate(endDate);
	endDatePicker->setDate(endDate);
//	endDatePicker->getCalendar().setSelectedDate(endDate);
	endLayout = new QHBoxLayout();
	endLayout->addStretch();
	endLayout->addWidget(endLabel);
	endLayout->addWidget(endDatePicker);

	syncButton = new QPushButton("Sync",this);
	cancelButtun = new QPushButton("Cancel",this);
	buttonLayout = new QHBoxLayout();
	buttonLayout->addWidget(syncButton);
	buttonLayout->addStretch();
	buttonLayout->addWidget(cancelButtun);


	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	int l, t, r, b;
	mainLayout->getContentsMargins(&l, &t, &r, &b);
	mainLayout->addSpacing(t / 2);
	mainLayout->addLayout(startLayout);
	mainLayout->addSpacing(t / 2);
	mainLayout->addLayout(endLayout);
	mainLayout->addSpacing(t / 2);
	mainLayout->addLayout(buttonLayout);
	mainLayout->addSpacing(t / 2);

	setLayout(mainLayout);

	this->setWindowFlags(Qt::Window);

	connect(syncButton, SIGNAL(clicked()), this, SLOT(onSyncClicked()));
	connect(cancelButtun, SIGNAL(clicked()), this, SLOT(onCancelClicked()));
	connect(db, SIGNAL(manSynchQueryDone()), this, SLOT(onEmitManSyncDone()));
	connect(db, SIGNAL(manSynchQueryError(QString)), this, SLOT(onEmitManSyncError(QString)));
}

ManualSyncWidget::~ManualSyncWidget()
{
	delete startLayout;
	delete endLayout;
	delete buttonLayout;
}

void ManualSyncWidget::showAdjusted()
{
	show();
	adjustPosition();
}


void ManualSyncWidget::adjustPosition()
{
	QRect rect = this->rect();
	QPoint top_left = this->parentWidget()->mapToGlobal(rect.topLeft());
	this->move(top_left.x() + this->parentWidget()->width() / 2 - this->width()/2, top_left.y());
}


void ManualSyncWidget::onSyncClicked()
{
	QString settings_str = m_settings.value("bind_json").toString();
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
	for (nlohmann::json::iterator it = j.begin(); it != j.end(); ++it)
	{
		if (it == j.begin())
			str.append(string(it.value()));
		else
			str.append(";").append(string(it.value()));
	}

	QStringList list = QString::fromStdString(str).split(";");

	bool ok;
	QStringList list1 = startDatePicker->getDate().split("/");
	QDate start = QDate(list1.at(2).toInt(&ok), list1.at(1).toInt(&ok), list1.at(0).toInt(&ok));

	QStringList list2 = endDatePicker->getDate().split("/");
	QDate end = QDate(list2.at(2).toInt(&ok), list2.at(1).toInt(&ok), list2.at(0).toInt(&ok));

	if (start > end)
	{
		QMessageBox msgBox(this);
		msgBox.setText("The Start Date cannot\nbe later than the End Date");
		msgBox.setIcon(QMessageBox::Warning);
		auto* ok = msgBox.addButton("Back to selection", QMessageBox::ActionRole);

		msgBox.setFont(workingFont);

		msgBox.exec();
		return;
	}

	QMetaObject::invokeMethod(db, "doManSync", Qt::QueuedConnection,
		Q_ARG(QString, startDatePicker->getDate()),
		Q_ARG(QString, endDatePicker->getDate()),
		Q_ARG(QStringList,list)
		);
	this->setEnabled(false);
}

void ManualSyncWidget::onEmitManSyncDone()
{
	QMessageBox msgBox(this);
	msgBox.setText("Manual sync\nis done");
	msgBox.setIcon(QMessageBox::Information);
	msgBox.setFont(workingFont);
	msgBox.exec();

	this->setEnabled(true);
	emit manSyncDone();
}

void ManualSyncWidget::onEmitManSyncError(QString text)
{
	QMessageBox msgBox(this);
	msgBox.setText(text);
	msgBox.setIcon(QMessageBox::Information);
	msgBox.setFont(workingFont);
	msgBox.exec();

	this->setEnabled(true);
	emit manSyncDone();
}

void ManualSyncWidget::onCancelClicked()
{
	emit manSyncCanceled();
}

void ManualSyncWidget::closeEvent(QCloseEvent* event)
{
	emit manSyncCanceled();
}
