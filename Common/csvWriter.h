#pragma once

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDate>
#include <QTime>
#include <QTextStream>
#include <iostream>
#include <vector>
#include <fstream>

#include "patientCareLog.h"

using namespace std;

class csvWriter : public QObject
{
    Q_OBJECT
public:

    csvWriter(QObject* parent = nullptr);


    csvWriter(csvWriter&& in) noexcept
    {
        this->ofs = move(in.ofs);
    };

    ~csvWriter();

    bool openCSV(const QString& fileName, QString workingDir, const vector<string>& header);

    template<typename T>
    void writeArray(vector<T> data)
    {
        for (auto it = data.begin(); it != data.end(); it++)
        {
            if (it == data.begin()) ofs << "'" << *it << "'";
            else ofs << "," << "'" << *it << "'";
        }
        ofs << endl;// "\n";
    };

private:
    ofstream ofs;
};

