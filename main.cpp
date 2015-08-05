#include "views/setupview.h"
#include "controllers/basecontroller.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QSettings>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setOrganizationName("orgname");
    app.setOrganizationDomain("facetracker.com");
    app.setApplicationName("FaceTracker");
    app.setApplicationVersion("0.0.1");
    app.setQuitOnLastWindowClosed(false);


    BaseController ctrl(&app);
    QObject::connect(&ctrl, &BaseController::quit, &app, &QApplication::quit);
    ctrl.createConnections();
    ctrl.readSettings();
    ctrl.initLanguagesMap();
    ctrl.createDB();
    ctrl.startTimers();

    ctrl.start();
    return app.exec();
}
