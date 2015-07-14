#ifndef TRAYICON_H
#define TRAYICON_H

#include "systemtrayicon/asystemtrayicon.h"
#include <QMap>
#include <QPointer>


enum Color {green, gray, red};
enum Action {active, quit, settings};

class QMenu;
class QAction;

class TrayIcon : public QObject
{
    Q_OBJECT
public:
    explicit TrayIcon(QObject *parent = 0);
    ~TrayIcon();

    void setToolTip(QString tooltip);
    void setIcon(QIcon icon);
    void setIcon(QString icon);
    void setContextMenu(QMenu *menu);
    void show();
signals:
    void menuTriggered(Action);

public slots:
    void changeIcon(Color color);
    void changeToolTip(QString tooltip);
    void showMessage(const QString &title, const QString &msg);

private:
    QMap<Color, QString> m_icons;
    QMap<Action, QString> m_actions;
    QAction *act, *qt, *sett;
    QMenu *m_menu;
    QPointer<ASystemTrayIcon> m_trayIcon;
};

#endif //TRAYICON_H
