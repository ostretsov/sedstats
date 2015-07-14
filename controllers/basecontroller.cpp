#include "controllers/basecontroller.h"
#include "controllers/sysidlecontroller.h"
#include "controllers/cameracontroller.h"
#include "controllers/timecounter.h"
#include "views/setupview.h"
#include "views/statistictimes.h"
#include "views/trayicon.h"

#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QSystemTrayIcon>



BaseController::BaseController() :
    QObject(nullptr),
    m_sysNoteTime(SYSNOTETIMERINIT), //1 min
    m_exceededTime(0),
    m_camBusy(false),
    m_sysidleBusy(false),
    m_setupViewOpened(false)
{
    /* views */
    m_statisticTimes = new StatisticTimes(nullptr);
    m_setupView = new SetupView(nullptr);

    /* system tray icon */
    m_trayIcon = new TrayIcon(this);

    /* timers */
    m_sysIdleTimer = new QTimer(this);
    m_cameraTimer = new QTimer(this);
    m_sysNoteTimer = new QTimer(this);
    m_sysNoteTimer->setSingleShot(true);

    /* controllers */
    m_sysIdleController = new SysIdleController(this);
    m_cameraController = new CameraController(this);
    m_timeCounter = new TimeCounter(this);
}

void
BaseController::start(){
    m_trayIcon->show();
    m_timeCounter->startTimers();
}

BaseController::~BaseController(){
    delete m_statisticTimes;
    delete m_setupView;
}

void
BaseController::createConnections(){

    //по таймеру сигналим контроллеру запрос и простое системы
    connect(m_sysIdleTimer, SIGNAL(timeout()),
            m_sysIdleController, SLOT(sysIdleDuration()));
    //получаем сигнал от контроллера о простое системы
    connect(m_sysIdleController, SIGNAL(sysIdleProcessed(ulong)),
            this, SLOT(sysIdleAnswer(ulong)));
    //по таймеру сигналим контроллеру запрос по лицу
    connect(m_cameraTimer, SIGNAL(timeout()),
            m_cameraController, SLOT(procCamInput()));
    //получаем сигнал от контроллера камеры ответ по лицу :)
    connect(m_cameraController, SIGNAL(imageProcessed(bool)),
            this, SLOT(cameraFaceAnswer(bool)));
    //получаем сигнал от трей-икон меню
    connect(m_trayIcon, SIGNAL(menuTriggered(Action)),
            this, SLOT(trayIconTriggered(Action)));
    //передаем сигнал на смену иконки в систрей
    connect(this, SIGNAL(changeSysTrayIcon(Color)),
            m_trayIcon, SLOT(changeIcon(Color)));
    //передаем сигнал от окна с циферками на окно с установками
    connect(m_statisticTimes, &StatisticTimes::showSettings,
            [this](){m_setupView->show();});
    //передаем сигнал на окно с циферками -- поменять циферки
    connect(this, SIGNAL(changeCurrentTime(QString,Color)),
            m_statisticTimes, SLOT(chageIndication(QString,Color)));
    //передаем сигнал на timecounter о состоянии системы busy - true, free - false
    connect(this, SIGNAL(busySignal(bool)),
            m_timeCounter, SLOT(userStateBusy(bool)));
    //принимаем сигнал о смене состояния счетчика (red, green, gray)
    connect(m_timeCounter, SIGNAL(stateChanged(Color)),
            this, SLOT(changeCurrentIcon(Color)));
    //принмаем от счетчика показания времени
    connect(m_timeCounter, SIGNAL(setCurrentWorkingTime(int)),
            this, SLOT(setCurrentWorkingTime(int)));
    //покажем время над иконкой если работает tooltip в системе то будет видно
    connect(this, SIGNAL(setSystemIconToolTip(QString)),
            m_trayIcon, SLOT(changeToolTip(QString)));
    //из окна установок сразу отправим новое значение в счетчик
    //а оттуда оно само увидится в окне с циферками
    connect(m_setupView, SIGNAL(changeMaxWorkingTime(uint)),
            m_timeCounter, SLOT(setMaxWorkingTime(uint)));
    //установить флажок проверки лица в камере в соотв с setupview
    connect(m_setupView, SIGNAL(changeCheckCamera(Qt::CheckState)),
            m_cameraController, SLOT(setCheckCamera(Qt::CheckState)));
    //отправить в камера-контроллер новые значения макс и мин размеров лица
    connect(m_setupView, SIGNAL(changeMinMaxFaceSize(uint,uint)),
            m_cameraController, SLOT(setMinMaxFaceSizes(uint,uint)));
    connect(m_setupView, SIGNAL(setupViewOpened(bool)),
            this, SLOT(setupViewOpened(bool)));
    //обработать сигнал таймера системных оповещений
    connect(m_sysNoteTimer, SIGNAL(timeout()),
            this, SLOT(sysNoteTimerHandle()));
    //соединить для выдачи системных оповещений
    connect(this, SIGNAL(putSysNoteMsg(QString,QString)),
            m_trayIcon, SLOT(showMessage(QString,QString)));


}

void
BaseController::aboutQuit(){
    qDebug() << "Quit from application";
    emit quit();
}


void
BaseController::sysIdleAnswer(unsigned long duration){
    QTime time;
    QString time_s = time.currentTime().toString();
    m_sysidleBusy = duration < IDLEDURATION ? true : false;
    emit busySignal(m_sysidleBusy or m_camBusy);
}

void
BaseController::cameraFaceAnswer(bool facePresent){
    QString time_s = QTime().currentTime().toString();
    m_camBusy = facePresent;
}
 
void
BaseController::trayIconTriggered(Action action){
    switch (action) {
    case Action::quit:
        aboutQuit();
        break;
    case Action::active:
        m_statisticTimes->show();
        break;
    case Action::settings:
        m_setupView->show();
        break;
    default:
        break;
    }
}

void
BaseController::startTimers(){
    //1000 проверять system idle каждую секунду
    m_sysIdleTimer->start(IDLETIMERINIT);
    //теребить камеру каждые 2 секунды
    m_cameraTimer->start(CAMERATIMERINIT);
}

void
BaseController::changeCurrentIcon(Color color){
    emit changeSysTrayIcon(color);
    switch (color) {
    case Color::red:

        break;
    case Color::green:
        emit putSysNoteMsg(tr("New working period"),
                           tr("New period started"));
        /*break;*/
    case Color::gray:
        if(m_sysNoteTimer->isActive()){
            m_sysNoteTimer->stop();
        }
        break;
    default:
        break;
    }
}

void
BaseController::setCurrentWorkingTime(int t){
    m_exceededTime = t;
    QString sign = t < 0 ? "-" : "";
    QString time = QString("%1:%2")
            .arg(abs(t/60), 2, 10, QLatin1Char('0'))
            .arg(abs(t % 60), 2, 10, QLatin1Char('0'));
    Color color = t < 0 ? Color::red : Color::green;
    emit changeCurrentTime(sign + time, color);
    emit setSystemIconToolTip(sign+time);
}

void
BaseController::setupViewOpened(bool opened){

    if (opened){
        m_cameraTimer->stop();
    }
    else{
        m_cameraTimer->start(CAMERATIMERINIT);
    }
}

void
BaseController::sysNoteTimerHandle(){
    qDebug() << "Exceeded time = " << m_exceededTime;
    unsigned int _extratime = abs(m_exceededTime) / SYSNOTETIMERINIT; //переведем в минуты
    if(_extratime < 1 /* < 1 min */){
        qDebug() << "exceededTime" << m_exceededTime;
        emit putSysNoteMsg(tr("Time exceeded!"),
                           tr("You reach max working time!"));
        m_sysNoteTimer->singleShot(m_sysNoteTime * 1000, this, SLOT(sysNoteTimerHandle()));
        return;
    }
    else{
        qDebug() << "extraTime = " << _extratime;
        emit putSysNoteMsg(tr("Time exceeded!"),
                           QString("Your time is greater than the maximum %1 minute").arg(_extratime));
        m_sysNoteTime *= 2;
        m_sysNoteTimer->singleShot(m_sysNoteTime * 1000, this, SLOT(sysNoteTimerHandle()));
    }
}
