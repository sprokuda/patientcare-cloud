#include "PatientCareCloudWidget.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PatientCareCloudWidget w;
    w.show();
    return a.exec();
}
