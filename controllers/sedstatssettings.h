#ifndef SEDSTATSSETTINGS_H
#define SEDSTATSSETTINGS_H

#include <QString>
#include <QSettings>

class SedstatsSettings
{
public:
    static const SedstatsSettings& Instance()
    {
        static SedstatsSettings theSingleInstance;
        return theSingleInstance;
    }
    void writeSettings() const;
    void readSettings() const;
    static unsigned int maxWidth, maxWorkingPeriod;
    static unsigned int minWidth;
    static QString login, passwd, lang;
    static bool camUsing;
private:
    SedstatsSettings(){}
    SedstatsSettings(const SedstatsSettings& root);
    SedstatsSettings& operator=(const SedstatsSettings&);

};


#endif // SEDSTATSSETTINGS_H
