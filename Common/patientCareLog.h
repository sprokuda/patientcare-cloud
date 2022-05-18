#pragma once
#include <QObject>
#include <QDateTime>
#include <QSettings>

#include <QAbstractButton>
#include <QPushButton>
#include <QMessageBox>

#include <fstream>

extern QString absoluteApplicationPath;

class patientCareLog
{
public:
    patientCareLog() {};
    ~patientCareLog()
    {
        log.close();
    };
    bool openLogGUI(const QSettings& settings, const char* postfix = "", const char* message = "PatienCare log file can not be open!");
    bool openLogCMD(const QSettings& settings, const char* postfix = "", const char* message = "PatienCare log file can not be open!");


public:
    std::ofstream log;
};