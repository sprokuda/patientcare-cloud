#pragma once
#include <QObject>
#include <QDateTime>
#include <QSettings>

#include <QAbstractButton>
#include <QPushButton>
#include <QMessageBox>
#include <QDir>
#include <QFont>

#include <fstream>

extern QString absoluteApplicationPath;

class patientCareLog
{
public:
    patientCareLog() { workingFont = QFont("Calibri", 10); };
    ~patientCareLog()
    {
        log.close();
    };

    void showErrorDialog(QString text);

    bool openLogGUI(const QSettings& settings, 
                    const char* subfolder, 
                    const char* postfix = "", 
                    const char* message = "PatienCare log file can not be open!");
    bool openLogCMD(const QSettings& settings, 
                    const char* subfolder, 
                    const char* postfix = "", 
                    const char* message = "PatienCare log file can not be open!");


public:
    QFont workingFont;
    std::ofstream log;
};