#include "statistictimes.h"
#include "ui_statistictimes.h"

#include <QCloseEvent>

StatisticTimes::StatisticTimes(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatisticTimes)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint
                   |Qt::WindowStaysOnTopHint
                   //|Qt::WindowMinimizeButtonHint
                   |Qt::WindowCloseButtonHint);
}

void
StatisticTimes::chageIndication(QString value, Color color){
    QString styleSheet = color == Color::green ? "color:green" : "color:red";
    ui->lcdCurrentTime->setStyleSheet(styleSheet);
    ui->lcdCurrentTime->display(value);
}


StatisticTimes::~StatisticTimes()
{
    delete ui;
}

void StatisticTimes::on_btnSettings_clicked()
{
    emit showSettings();
}

void
StatisticTimes::closeEvent(QCloseEvent *ev){
    hide();
    ev->ignore();
}

void
StatisticTimes::changeEvent(QEvent *ev){

    if (ev->type() == QEvent::LanguageChange){
        ui->retranslateUi(this);
    }
    QWidget::changeEvent(ev);
}
