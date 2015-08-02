#ifndef DBCONTROLLER_H
#define DBCONTROLLER_H

#include <QObject>

class DBController : public QObject
{
    Q_OBJECT
public:
    explicit DBController(QObject *parent = 0);
    bool checkDB();
    void createDB();
signals:
public slots:
    void storeData(uint start, uint maxduration, uint duration);
private:
};

#endif // DBCONTROLLER_H
