#ifndef BASECONTROLLER_H
#define BASECONTROLLER_H

#include "views/trayicon.h"

#include <QObject>

class SetupView;
class StatisticTimes;
class QTimer;
class TrayIcon;
class SysIdleController;
class CameraController;
class TimeCounter;

const unsigned int SYSNOTETIMERINIT = 60; // sec
const unsigned int IDLETIMERINIT = 1000; // milli sec
const unsigned int CAMERATIMERINIT = 2000; // milli sec
const unsigned int IDLEDURATION = 1000; // m sec

class BaseController final : public QObject
{
    Q_OBJECT
public:
    explicit BaseController();
    ~BaseController();
    void start();
    void createConnections();
    void startTimers();

signals:
    void quit();
    void sysIdleRequest();
    void changeCurrentTime(QString value, Color color);
    void busySignal(bool busy);
    void changeSysTrayIcon(Color color);
    void setSystemIconToolTip(QString toolTip);
    void putSysNoteMsg(const QString &title, const QString &msg);

public slots:
    void aboutQuit();
    void sysIdleAnswer(unsigned long duration);
    void cameraFaceAnswer(bool facePresent);
    void trayIconTriggered(Action action);
    void changeCurrentIcon(Color color); //запрос на смену иконки
    void setCurrentWorkingTime(int t);
    void setupViewOpened(bool opened);
    void sysNoteTimerHandle();

private:
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
//    unsigned int m_maxWorkingTime; //sec
//    unsigned int m_activeTime; //sec
    unsigned int m_sysNoteTime; //sec
    int m_exceededTime; //sec
    bool m_online; //connected to internet server
    bool m_camBusy, m_sysidleBusy;
    bool m_setupViewOpened;
    unsigned long int m_beginTimeStamp;
};

#endif // BASECONTROLLER_H
