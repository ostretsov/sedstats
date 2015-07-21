#ifndef SEDSTATSSETTINGS_H
#define SEDSTATSSETTINGS_H

#include <QString>

/*
 * Синглетон по Майерсу
 */

class SedstatsSettings
{
public:
    static const SedstatsSettings& Instance()
    {
        static SedstatsSettings theSingleInstance;
        return theSingleInstance;
    }
    QString getLogin();
    QString getPasswd();
    unsigned int getMinWidth();
    unsigned int getMaxWidth();
    unsigned int getMaxWorkingPeriod();
    bool getCamUsing();
    void setLogin(QString login){this->login = login;};
    void setPasswd(QString passwd){this->passwd = passwd;};
    void setMaxWorkingPeriod(unsigned int period){this->maxWorkingPeriod = period;};
    void setCamUsing(bool cu){this->camUsing = cu;};
    void setMinWidth(unsigned int width){this->minWidth = width;};
    void setMaxWidth(unsigned int width){this->maxWidth = width;};

    void writeSettings();
    void readSettings();
private:
    SedstatsSettings(){};
    SedstatsSettings(const SedstatsSettings& root);
    SedstatsSettings& operator=(const SedstatsSettings&);

    unsigned int minWidth, maxWidth, maxWorkingPeriod;
    QString login, passwd;
    bool camUsing;
};


#endif // SEDSTATSSETTINGS_H
