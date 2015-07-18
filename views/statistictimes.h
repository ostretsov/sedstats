#ifndef STATISTICTIMES_H
#define STATISTICTIMES_H

#include "views/trayicon.h"

#include <QWidget>


namespace Ui {
class StatisticTimes;
}

class QCloseEvent;

class StatisticTimes : public QWidget
{
    Q_OBJECT

public:
    explicit StatisticTimes(QWidget *parent = 0);
    ~StatisticTimes();
public slots:
    void chageIndication(QString value, Color color);

private slots:
    void on_btnSettings_clicked();

signals:
    void showSettings();

private:
    void closeEvent(QCloseEvent *ev);
    Ui::StatisticTimes *ui;
    void changeEvent(QEvent *ev);
};

#endif // STATISTICTIMES_H
