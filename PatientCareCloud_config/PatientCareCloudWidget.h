#pragma once

#include <QtWidgets/QWidget>
#include "ui_PatientCareCloudWidget.h"

class PatientCareCloudWidget : public QWidget
{
    Q_OBJECT

public:
    PatientCareCloudWidget(QWidget *parent = Q_NULLPTR);

private:
    Ui::PatientCareCloudWidgetClass ui;
};
