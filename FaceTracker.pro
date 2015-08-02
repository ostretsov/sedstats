#-------------------------------------------------
#
# Project created by QtCreator 2015-06-21T17:55:15
#
#-------------------------------------------------


CONFIG += c++11


QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sedstats
TEMPLATE = app


SOURCES += main.cpp\
    controllers/basecontroller.cpp \
    controllers/cameracontroller.cpp \
    views/setupview.cpp \
    views/statistictimes.cpp \
    views/trayicon.cpp \
    controllers/sysidlecontroller.cpp \
    controllers/timecounter.cpp \
    systemtrayicon/asystemtrayicon.cpp \
    systemtrayicon/auniformsystemtrayicon.cpp \
    systemtrayicon/aunitysystemtrayicon.cpp \
    ocvroutines.cpp \
    controllers/sedstatssettings.cpp \
    controllers/dbcontroller.cpp

HEADERS  += views/setupview.h \
    views/statistictimes.h \
    views/trayicon.h \
    controllers/basecontroller.h \
    controllers/cameracontroller.h \
    controllers/sysidlecontroller.h \
    controllers/timecounter.h \
    systemtrayicon/asystemtrayicon.h \
    systemtrayicon/auniformsystemtrayicon.h \
    systemtrayicon/aunitysystemtrayicon.h \
    ocvroutines.h \
    controllers/sedstatssettings.h \
    controllers/dbcontroller.h

FORMS    += forms/setupview.ui \
    forms/statistictimes.ui

TRANSLATIONS = translations/sedstats_ru.ts \
                translations/sedstats_de.ts

unix{
    LIBS += -L/usr/lib -lappindicator -lnotify
    LIBS += -lX11 -lXss
    PKGCONFIG += opencv
    CONFIG += link_pkgconfig
    PKGCONFIG += gtk+-2.0
}

#DEFINES += QT_NO_DEBUG_OUTPUT

RESOURCES += \
    resources.qrc
