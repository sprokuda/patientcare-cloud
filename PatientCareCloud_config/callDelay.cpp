#include<windows.h>

#include "callDelay.h"
//#include "dbScripts.h"

using namespace std;

callDelay::callDelay(QObject* parent) : QObject(parent)
{

}

callDelay::~callDelay()
{

}

void callDelay::doDelay()
{
    Sleep(250);
	emit delayFinished();
}

