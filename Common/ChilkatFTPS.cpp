#include "chilkatFTPS.h"

extern unique_ptr<patientCareLog> logging;

ChilkatFTPS::ChilkatFTPS(
    const string& _host_name, 
    const size_t& _port, 
    const string& _user, 
    const string& _password)
    : 
    m_host_name(_host_name),
    m_port(_port), 
    m_user(_user), 
    m_password(_password)
{
    ftp = make_shared<CkFtp2>();

    ftp->put_Hostname(m_host_name.c_str());
    ftp->put_Port(m_port);
    ftp->put_Username(m_user.c_str());
    ftp->put_Password(m_password.c_str());

    ftp->put_AuthTls(true);
}

bool ChilkatFTPS::connect()
{
    bool success = ftp->Connect();
    if (success != true)
    {
        logging->log << ftp->lastErrorText() << "\n";
        return success;
    }
    else
    {
        logging->log << "Chlikat FTPS connection is successfully connected" << endl;
        return success;
    }
}

bool ChilkatFTPS::disconnect()
{
    bool success = ftp->Disconnect();
    if (success != true)
    {
        logging->log << ftp->lastErrorText() << "\n";
        return success;
    }
    else
    {
        logging->log << "Chlikat FTPS connection is correctly disconnected" << endl;
        return success;
    }
}

ChilkatFTPS::~ChilkatFTPS()
{
    bool success = ftp->Disconnect();
    if (success != true)
    {
        logging->log << ftp->lastErrorText() << "\n";
        return;
    }
    else
    {
        logging->log << "Chlikat FTPS connection is correctly disconnected" << endl;
        return;
    }
}

bool ChilkatFTPS::sendFile(const string& _local_path, const string& _remote_file_name)
{
    bool success = ftp->PutFile(_local_path.c_str(), _remote_file_name.c_str());//PutFile also works ok
    if (success != true)
    {
        logging->log << ftp->lastErrorText() << "\n";
        return success;
    }
    else
    {
        logging->log << "Chlikat FTPS file upload is successfully done" << endl;
        return success;
    }
}


bool ChilkatFTPS::changeRemoteDir(const string& _remote_dir_name)
{
    bool success = ftp->ChangeRemoteDir(_remote_dir_name.c_str());//PutFile also works ok
    if (success != true)
    {
        logging->log << ftp->lastErrorText() << "\n";
        logging->log << "Chlikat FTPS remote dir to " << _remote_dir_name << " can't be done" << endl;
        return success;
    }
    else
    {
        logging->log << "Chlikat FTPS remote dir to " << _remote_dir_name << " is done" << endl;
        return success;
    }
}