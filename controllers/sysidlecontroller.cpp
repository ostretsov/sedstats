#include "sysidlecontroller.h"

#include <X11/extensions/scrnsaver.h>

SysIdleController::SysIdleController(QObject *parent) :
    QObject(parent)
{
}

unsigned long
SysIdleController::m_get_idle(){
    Display *dpy = XOpenDisplay(NULL);
    //TODO обработать или нет?
    if (!dpy) {
      return(0);
    }
    XScreenSaverInfo *info = XScreenSaverAllocInfo();
    XScreenSaverQueryInfo(dpy, DefaultRootWindow(dpy), info);
    XCloseDisplay(dpy);
    unsigned int res = info->idle;
    free(info);
    return res;
}

void
SysIdleController::sysIdleDuration(){
    emit sysIdleProcessed(m_get_idle());
}
