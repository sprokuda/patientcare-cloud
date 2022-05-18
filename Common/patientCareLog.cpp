#include "patientCareLog.h"


bool patientCareLog::openLogGUI(const QSettings& settings, const char* postfix, const char* message)
{
    absoluteApplicationPath = settings.value("Path").toString();
    auto filename = absoluteApplicationPath.toStdString() + "/" + "Log" + "/" +
        QDate::currentDate().toString("yyyy-MM-dd").toStdString() + "_" +
        QTime::currentTime().toString("hh-mm-ss").toStdString() + "."+postfix+".txt";
    try 
    {
        log.open(filename);
    }
    catch (...)
    {
        QMessageBox msgBox(nullptr);
        msgBox.setText("Exception on Log file open");
        msgBox.setIcon(QMessageBox::Critical);
        auto* exit = msgBox.addButton("Exit Program", QMessageBox::ActionRole);

        msgBox.exec();
        return false;
    }

    tryOpenLog:
        if (!log.is_open())
        {
            QMessageBox msgBox(nullptr);
            msgBox.setText(message);
            msgBox.setIcon(QMessageBox::Critical);
            auto* open = msgBox.addButton("Try to open again", QMessageBox::ActionRole);
            auto* exit = msgBox.addButton("Exit Program", QMessageBox::ActionRole);

            msgBox.exec();

            if (msgBox.clickedButton() == open)
            {
                try
                {
                    log.open(filename);
                }
                catch (...)
                {
                    goto tryOpenLog;
                }
                goto tryOpenLog;
            }
            else if (msgBox.clickedButton() == exit)
            {
                return false;
            }
        }
     log << filename << std::endl;
     return true;
    

};


bool patientCareLog::openLogCMD(const QSettings& settings, const char* postfix, const char* message)
{
    absoluteApplicationPath = settings.value("Path").toString();
    auto filename = absoluteApplicationPath.toStdString() + "/" + "Log" + "/" +
        QDate::currentDate().toString("yyyy-MM-dd").toStdString() + "_" +
        QTime::currentTime().toString("hh-mm-ss").toStdString() + "." + postfix + ".txt";
    try
    {
        log.open(filename);
    }
    catch (...)
    {
        return false;
    }
    if (!log.is_open())
    {
        return false;
    }
    log << filename << std::endl;
    return true;
};