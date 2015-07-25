#include "sedstatssettings.h"
#include <QDebug>

unsigned int SedstatsSettings::minWidth;
unsigned int SedstatsSettings::maxWidth;
unsigned int SedstatsSettings::maxWorkingPeriod;
QString SedstatsSettings::passwd;
QString SedstatsSettings::login;
QString SedstatsSettings::lang;
bool SedstatsSettings::camUsing;

void
SedstatsSettings::writeSettings()const{
    qDebug() << "Write values";
    qDebug() << "minWidth" << minWidth;
    qDebug() << "maxWidth" << maxWidth;
    QSettings settings;
    settings.setValue("minWidth", minWidth);
    settings.setValue("maxWidth", maxWidth);
    settings.setValue("maxWorkingPeriod", maxWorkingPeriod);
    settings.setValue("Login", login);
    settings.setValue("Password", passwd);
    settings.setValue("CameraOn", camUsing);
    settings.setValue("Language", lang);
}

void SedstatsSettings::readSettings()const{
    QSettings settings;

    minWidth = settings.value("minWidth", 20).toInt();
    maxWidth = settings.value("maxWidth", 50).toInt();
    maxWorkingPeriod = settings.value("maxWorkingPeriod", 1).toInt();
    login = settings.value("Login", "").toString();
    passwd = settings.value("Password", "").toString();
    lang = settings.value("Language", "").toString();
    camUsing = settings.value("CameraOn", false).toBool();
    qDebug() << "Read values";
    qDebug() << "minWidth" << minWidth;
    qDebug() << "maxWidth" << maxWidth;

}


