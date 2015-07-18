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


    BaseController ctrl(&app);
    QObject::connect(&ctrl, &BaseController::quit, &app, &QApplication::quit);
    ctrl.createConnections();
    ctrl.initLanguagesMap();
    ctrl.startTimers();

    ctrl.start();
    QApplication::setQuitOnLastWindowClosed(false);
    return app.exec();
}
