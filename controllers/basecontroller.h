#ifndef BASECONTROLLER_H
#define BASECONTROLLER_H

#include "views/trayicon.h"
#include "controllers/sedstatssettings.h"

#include <QObject>
#include <QMap>


class SetupView;
class StatisticTimes;
class QTimer;
class TrayIcon;
class SysIdleController;
class CameraController;
class TimeCounter;
class QApplication;
class QTranslator;

const unsigned int SYSNOTETIMERINIT = 60; // sec
const unsigned int IDLETIMERINIT = 1000; // milli sec
const unsigned int CAMERATIMERINIT = 20000; // 20 sec
const unsigned int IDLEDURATION = 1000; // m sec

class BaseController final : public QObject
{
    Q_OBJECT
public:
    explicit BaseController(QApplication *parent = 0);
    ~BaseController();
    void start();
    void createConnections();
    void startTimers();
    void initLanguagesMap();

signals:
    void quit();
    void sysIdleRequest();
    void changeCurrentTime(QString value, Color color);
    void busySignal(bool busy);
    void changeSysTrayIcon(Color color);
    void setSystemIconToolTip(QString toolTip);
    void putSysNoteMsg(const QString &title, const QString &msg);
    void changeLanguage();

public slots:
    void aboutQuit();
    void sysIdleAnswer(unsigned long duration);
    void cameraFaceAnswer(bool facePresent);
    void trayIconTriggered(Action action);
    void changeCurrentIcon(Color color); //запрос на смену иконки
    void setCurrentWorkingTime(int t);
    void setupViewOpened(bool opened);
    void sysNoteTimerHandle();
    void changeLanguage(QString lang);

private:
    //our parent -- QApplication
    QApplication *m_app;
    //translations
    QMap<QString, QString> m_translations;
    QMap<QString, QTranslator*> m_translators;
    //views
    StatisticTimes* m_statisticTimes;
    SetupView* m_setupView;
    TrayIcon *m_trayIcon;

    //Controllers
    SysIdleController *m_sysIdleController;
    CameraController *m_cameraController;
    TimeCounter *m_timeCounter;

    //Timers
    QTimer *m_sysIdleTimer;
    QTimer *m_cameraTimer;
    QTimer *m_sysNoteTimer;

    //Values
    unsigned int m_sysNoteTime; //sec
    int m_exceededTime; //sec
    bool m_online; //connected to internet server
    bool m_camBusy, m_sysidleBusy;
    bool m_setupViewOpened;
    unsigned long int m_beginTimeStamp;
    QString m_currentLanguage;

};

#endif // BASECONTROLLER_H
