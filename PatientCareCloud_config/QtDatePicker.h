#pragma once

#include <QWidget>
#include <QLabel>
#include <QlineEdit>
#include <QPushButton>
#include <QDate>

#include "QtDatePickerPopup.h"


class QtDatePicker : public QWidget
{
    Q_OBJECT

public:
    QtDatePicker(QWidget* parent = Q_NULLPTR);

    void adjustPopupPosition();
    QString getDate();
    QCalendarWidget& getCalendar() { return popup->getCalendar(); };
public slots:
    void setDate(QDate date);
private slots:
    void onShowPopupButtonClicked();

signals:

    void editingFinished();

private:

    QtDatePickerPopup* popup;
    QLabel* dateLbl;
    QLineEdit* edit;
    QToolButton* button;

};

