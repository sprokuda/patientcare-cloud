#include "csvWriter.h"



using namespace std;

extern QString absoluteApplicationPath;
extern unique_ptr<patientCareLog> logging;

csvWriter::csvWriter(QObject* parent) : QObject(parent)
{

}

bool csvWriter::openCSV(const QString& fileName, const vector<string>& header)
{
    QDir dir(absoluteApplicationPath);

    auto create_and_report = [&dir](QString dir_name)
    {
        auto result = dir.mkdir(dir_name);
        if (result) return QString(dir_name) + QString(" is created");
        else return QString(dir_name) + QString(" directory can t be created or already exists");
    };

    auto check_directory = [&dir, create_and_report](QString dir_name)
    {
        bool fld_report = QDir(dir_name).exists();
        if (!fld_report)
        {
            logging->log << create_and_report(dir_name).toStdString() + "\n";
        }
        auto cd_report = dir.cd(dir_name);
        if (!cd_report)
        {
            logging->log << (string("Can t enter directory ") + string("\"dir_name\"\n"));
        }
    };


    check_directory(absoluteApplicationPath + "/" + "Export");

    const QFileInfo outputDir(dir.absolutePath());
    if ((!outputDir.exists()) || (!outputDir.isDir()) || (!outputDir.isWritable()))
    {
        logging->log << "output directory does not exist, is not a directory, or is not writeable" << endl;
    }

    bool if_existed = false;

    QFile file(dir.absolutePath() + "\\" + fileName);
    try 
    {
        ofs.open((dir.absolutePath() + "\\" + fileName).toStdString());
    }
    catch (...)
    {
        logging->log << "Try to open file is failed, csv file is NOT open:" << (dir.absolutePath() + "\\" + fileName).toStdString() << endl;
        return false;
    }

    if (!ofs.is_open())
    {
        logging->log << "file is NOT open:" << (dir.absolutePath() + "\\" + fileName).toStdString() << endl;
        return false;
    }

    logging->log << "The file is open: " << (dir.absolutePath() + "\\" + fileName).toStdString() << endl;

    if (!if_existed) writeArray(header);
    return true;
}

csvWriter::~csvWriter()
{
    ofs.close();
}




