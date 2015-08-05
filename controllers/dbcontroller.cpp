#include "dbcontroller.h"
#include "controllers/sedstatssettings.h"

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDir>
#include <QFileDialog>

DBController::DBController(QObject *parent) :
    QObject(parent)
{
    qDebug() << "DB CONTROLLER CONSTRUCTING";

    p_tableName = "periods";
    p_createTableQuery = QString("CREATE TABLE %1 "
                                 "(TIMESTAMP INT PRIMARY KEY NOT NULL, "
                                 "MAXDURATION INT NOT NULL, "
                                 "DURATION INT NOT NULL)").arg(p_tableName);
    p_checkTableQuery = QString("SELECT count(*) FROM sqlite_master "
                                "WHERE type='table' AND name='%1'").arg(p_tableName);
    p_storeQuery = QString("INSERT INTO %1 VALUES(?, ?, ?)").arg(p_tableName);
}

void
DBController::storeData(uint start, uint maxduration, uint duration){
    qDebug() << "Storing data";
    QSqlQuery _query;
    _query.prepare(p_storeQuery);
    _query.bindValue(0, start);
    _query.bindValue(1, maxduration);
    _query.bindValue(2, duration);
    if(!_query.exec()){
        qDebug() << "Can't write data to DB";
        qDebug() << _query.lastQuery();
        qDebug() << _query.lastError();
    }
}

bool
DBController::checkDB(){
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    if(!db.isValid()){
        qDebug() << "Can't load driver";
        qDebug() << db.lastError();
    }
    else{
        qDebug() << "Driver for sqlite loaded";
    }
    QString _fileName = SedstatsSettings::Instance().dbFileName;
    if (_fileName == ""){
        qDebug() << "No file name in settings";
        return false;
    }
    /* файл существует но не является БД */
    db.setDatabaseName(_fileName);
    if(!db.open()){
        qDebug() << "DBfile_Error";
        qDebug() << db.lastError();
        return false;
    }
    /* файл существует, явл. БД, но не имеет нужной таблицы */
    QSqlQuery _query;
    _query.prepare(p_checkTableQuery);
    if(!_query.exec()){
        qDebug() << _query.lastQuery();
        qDebug() << _query.lastError();
        return false;
    }
    _query.first();
    if(_query.value(0).toInt() != 1){
        qDebug() << "No table " << p_tableName;
        return false;
    }
    return true;
}

void
DBController::createDB(){
    /* взять директорию пользователя */
    QString _workDir = QDir::homePath();
    /* вызвать диалог для выбора места и имени БД */
    QFileDialog fd;
    QString _fileName =
            fd.getSaveFileName(0,
                               tr("Save Your Data File"),
                               _workDir,
                               tr("Data files (*.sqlite)"));
    if (!_fileName.endsWith(".sqlite")){
        _fileName += ".sqlite";
    }
    qDebug() << _fileName << " selected";
    /* установить имя БД, открыть, создать таблицу */
    QSqlDatabase db = QSqlDatabase::database();
    db.setDatabaseName(_fileName);
    if(!db.open()){
        qDebug() << "Can't create DB " << _fileName;
        qDebug() << db.lastError();
    }
    SedstatsSettings::Instance().dbFileName = _fileName;
    QSqlQuery _query;
    _query.prepare(p_createTableQuery);
    if(!_query.exec()){
        qDebug() << "Can't create table";
        qDebug() << _query.lastQuery();
        qDebug() << _query.lastError();
    }
}
