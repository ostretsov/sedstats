#ifndef SEDSTATSSETTINGS_H
#define SEDSTATSSETTINGS_H

#include <QString>
#include <QSettings>

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
    QString getLogin()const {return login;}
    QString getPasswd()const {return passwd;}
    QString getLang() const {return lang;}
    unsigned int getMinWidth() const {return minWidth;}
    unsigned int getMaxWidth()const {return maxWidth;}
    unsigned int getMaxWorkingPeriod() const {return maxWorkingPeriod;}
    bool getCamUsing() const {return camUsing;}
    void setLogin(QString login) const {this->login = login;}
    void setPasswd(QString passwd)const {this->passwd = passwd;}
    void setLang(QString lang) const {this->lang = lang;}
    void setMaxWorkingPeriod(unsigned int period)const{this->maxWorkingPeriod = period;}
    void setCamUsing(bool cu) const {this->camUsing = cu;}
    void setMinWidth(unsigned int width) const {this->minWidth = width;}
    void setMaxWidth(unsigned int width) const {this->maxWidth = width;}

    void writeSettings() const;
    void readSettings() const;
private:
    SedstatsSettings(){}
    SedstatsSettings(const SedstatsSettings& root);
    SedstatsSettings& operator=(const SedstatsSettings&);

    static unsigned int minWidth;
    static unsigned int maxWidth, maxWorkingPeriod;
    static QString login, passwd, lang;
    static bool camUsing;
};


#endif // SEDSTATSSETTINGS_H
