#include "patientCareLog.h"

void patientCareLog::showErrorDialog(QString text)
{
    QMessageBox msgBox(nullptr);
    msgBox.setText(text);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setFont(workingFont);
    msgBox.exec();
}

bool patientCareLog::openLogGUI(const QSettings& settings, const char* subfolder, const char* postfix, const char* message)
{
    absoluteApplicationPath = settings.value("Path").toString();

    QDir dir(absoluteApplicationPath);

    auto create_and_report = [&dir](QString dir_name)
    {
        auto result = dir.mkdir(dir_name);
        if (result) return std::make_pair(QString("The folder ")+QString(dir_name) + QString(" is created"),true);
        else return std::make_pair(QString(dir_name) + QString(" directory can t be created or already exists"),false);
    };

    auto check_directory = [this, &dir, create_and_report](QString dir_name)
    {
        bool fld_report = QDir(dir_name).exists();
        if (!fld_report)
        {
            auto result = create_and_report(dir_name);
            if (!result.second)
            {
                showErrorDialog(result.first);
            }
        }
        auto cd_report = dir.cd(dir_name);
        if (!cd_report)
        {
            showErrorDialog(QString("Can't enter directory: ") + dir_name);
        }
    };

    auto subfolders = QString(subfolder).split("\\");
    QString path = absoluteApplicationPath;
    for (int i = 0; i < subfolders.size(); i++)
    {
        path = path + "\\" + subfolders.at(i);
        check_directory(path);
    }

    const QFileInfo outputDir(dir.absolutePath());
    if ((!outputDir.exists()) || (!outputDir.isDir()) || (!outputDir.isWritable()))
    {
        showErrorDialog("Output directory does not exist,\nis not a directory,\nor is not writeable");
    }



    auto filename = absoluteApplicationPath.toStdString() + "/" + subfolder + "/" +
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


bool patientCareLog::openLogCMD(const QSettings& settings, const char* subfolder, const char* postfix, const char* message)
{
    absoluteApplicationPath = settings.value("Path").toString();
    auto filename = absoluteApplicationPath.toStdString() + "/" + subfolder + "/" +
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