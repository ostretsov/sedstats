#include "views/setupview.h"
#include "controllers/basecontroller.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("orgname");
    app.setOrganizationDomain("facetracker.com");
    app.setApplicationName("FaceTracker");
    app.setApplicationVersion("0.0.1");

    QString locale = QLocale::system().name();
    QTranslator translator;
    translator.load(QString(":/tr/translations/sedstats_"+locale));
    app.installTranslator(&translator);

    BaseController ctrl;
    QObject::connect(&ctrl, &BaseController::quit, &app, &QApplication::quit);
    ctrl.createConnections();
    ctrl.startTimers();

    ctrl.start();
    QApplication::setQuitOnLastWindowClosed(false);
    return app.exec();
}
