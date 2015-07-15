#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QDebug>

#include "trayicon.h"


TrayIcon::TrayIcon(QObject *parent) : QObject(parent),
    m_icons()
{
    m_trayIcon = ASystemTrayIcon::create();
    m_menu = new QMenu(nullptr);

    m_actions.insert(active,tr("activate"));
    m_actions.insert(quit, tr("quit"));
    /*
    m_actions.insert(settings, tr("settings"));
    */

    act = m_menu->addAction(m_actions.value(active));
    /*
    sett = m_menu->addAction(m_actions.value(settings));
    */
    qt = m_menu->addAction(m_actions.value(quit));

    connect(act, &QAction::triggered, [this] () {emit menuTriggered(active);});
    /*
    connect(sett, &QAction::triggered, [this] () {emit menuTriggered(settings);});
    */
    connect(qt, &QAction::triggered, [this] () {emit menuTriggered(quit);});

    setContextMenu(m_menu);

    m_icons.insert(gray, ":/images/images/gray.png");
    m_icons.insert(green,":/images/images/green.png");
    m_icons.insert(red, ":/images/images/red.png");
    m_trayIcon->show();
}

void
TrayIcon::changeIcon(Color color){
    setIcon(QIcon(m_icons.value(color)));
    //setIcon(m_icons.value(color));
}

void
TrayIcon::setContextMenu(QMenu *menu){
    m_trayIcon->setContextMenu(menu);
}

void TrayIcon::setIcon(QIcon icon){
    qDebug() << "Set icon ";
    m_trayIcon->setIcon(icon);
}
void
TrayIcon::setIcon(QString icon){
    qDebug() << "Set icon by fname";
    m_trayIcon->setIcon(icon);
}

void
TrayIcon::changeToolTip(QString tooltip){
    m_trayIcon->setToolTip(tooltip);
}

void
TrayIcon::setToolTip(QString tooltip){
    m_trayIcon->setToolTip(tooltip);
}



TrayIcon::~TrayIcon()
{
    delete m_menu;
}

void
TrayIcon::show(){
    m_trayIcon->show();
}

void
TrayIcon::showMessage(const QString &title, const QString &msg){
    m_trayIcon->showMessage(title, msg);
}

