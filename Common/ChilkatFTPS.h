#pragma once

#include <QObject>

#include <iostream>
#include <fstream>
#include <string>

#include <CkFtp2.h>

#include "patientCareLog.h"

using namespace std;

class ChilkatFTPS : public QObject
{

public:
    ChilkatFTPS(
        const string& _host_name = "127.0.0.1", 
        const size_t& port = 991,
        const string& _user = "",
        const string& _password = "");

    ~ChilkatFTPS();

    bool connect();
    bool disconnect();

    bool sendFile(const string& _local_path, const string& _remote_file_name);
    bool ChilkatFTPS::changeRemoteDir(const string& _remote_dir_name);
    shared_ptr<CkFtp2> ftp;
private:

    string m_host_name;
    size_t m_port;
    string m_user;
    string m_password;
};
