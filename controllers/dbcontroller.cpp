#include "dbcontroller.h"
#include "controllers/sedstatssettings.h"

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

DBController::DBController(QObject *parent) :
    QObject(parent)
{
}

void
DBController::storeData(uint start, uint maxduration, uint duration){

}

bool
DBController::checkDB(){
    QString _fileName = SedstatsSettings::Instance().dbFileName;
    if (_fileName == ""){
        qDebug() << "No file name in settings";
        return false;
    }
    /* файл существует но не является БД */
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(_fileName);
    if(!db.open()){
        qDebug() << "DBfile_Error";
        qDebug() << db.lastError();
        return false;
    }
    /* файл существует, явл. БД, но не имеет нужной структуры */
    QSqlQuery query("SELECT count(*) FROM sqlite_master WHERE type='table' AND name='table_name'");
}

void
DBController::createDB(){

}
