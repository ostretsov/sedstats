#include "sedstatssettings.h"
unsigned int minWidth, maxWidth, maxWorkingPeriod;
QString login, passwd;
bool camUsing;


void
SedstatsSettings::writeSettings(){


}

void
SedstatsSettings::readSettings(){
    minWidth = settings.value("minWidth", 20).toInt();
    maxWidth = settings.value("maxWidth", 50).toInt();
    maxWorkingPeriod = settings.value("maxWorkingTime", 1).toInt();
    login = settings.value("Login", "").toString();
    passwd = settings.value("Password", "").toString();
    camUsing = settings.value("Camera", false).toBool();
}


