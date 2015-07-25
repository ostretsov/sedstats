#include "setupview.h"
#include "ui_setupview.h"
#include "ocvroutines.h"
#include "controllers/sedstatssettings.h"

#include <QTimer>
#include <QImage>
#include <QCloseEvent>
#include <QDebug>
#include <QtConcurrent/QtConcurrent>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

SetupView::SetupView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetupView)
{
    ui->setupUi(this);


    connect(this, SIGNAL(imageChanged(QPixmap)), this, SLOT(setVideoFrmPicture(QPixmap)), Qt::QueuedConnection);
}

SetupView::~SetupView()
{
    delete ui;
    if(cap.isOpened())
        cap.release();
}


void
SetupView::on_spnMaxWorkingPeriod_valueChanged(int arg1)
{
    SedstatsSettings::Instance().maxWorkingPeriod = arg1;
    emit changeMaxWorkingTime(arg1 * 60); // т.к. мы работаем с секундами
}

void
SetupView::on_checkBox_stateChanged(int arg1)
{
    switch (arg1) {
    case Qt::Unchecked:
        SedstatsSettings::Instance().camUsing = false;
        ui->lblCamStream->setEnabled(false);
        ui->lblFaceObjSize->setEnabled(false);
        ui->sldrMinSize->setEnabled(false);
        ui->sldrMaxSize->setEnabled(false);
        ui->lblTo->setEnabled(false);
        ui->lblPicture->setEnabled(false);
        ui->lblPicture->setText(tr("Video output"));
        m_videoTimer.stop();
        emit changeCheckCamera(Qt::Unchecked);
        break;
    case Qt::Checked:
        SedstatsSettings::Instance().camUsing = true;
        ui->lblCamStream->setEnabled(true);
        ui->lblFaceObjSize->setEnabled(true);
        ui->sldrMinSize->setEnabled(true);
        ui->sldrMaxSize->setEnabled(true);
        ui->lblTo->setEnabled(true);
        ui->lblPicture->setEnabled(true);
        if(initCamera()){
            ui->lblPicture->setText(tr("We have a camera."));
            emit setupViewOpened(true);
            emit changeCheckCamera(Qt::Checked);
            m_res = QtConcurrent::run(this,
                                      &SetupView::showCameraOutput);
        }else{
            ui->lblPicture->setText(tr("We have no camera"));
        }
        break;
    default:
        break;
    }
}

void
SetupView::on_sldrMinSize_valueChanged(int value)
{
    SedstatsSettings::Instance().minWidth = value;
    ui->sldrMinSize->setToolTip(QString("%1").arg(value));
    emit changeMinMaxFaceSize(ui->sldrMinSize->value(),
                              ui->sldrMaxSize->value());
}

void
SetupView::on_sldrMinSize_sliderMoved(int position)
{
    ui->sldrMinSize->setToolTip(QString("%1").arg(position));
}
void SetupView::on_sldrMaxSize_valueChanged(int value)
{
    SedstatsSettings::Instance().maxWidth = value;
    ui->sldrMaxSize->setToolTip(QString("%1").arg(value));
    emit changeMinMaxFaceSize(ui->sldrMinSize->value(),
                              ui->sldrMaxSize->value());

}

void SetupView::on_sldrMaxSize_sliderMoved(int position)
{
    ui->sldrMaxSize->setToolTip(QString("%1").arg(position));
}

bool
SetupView::initCamera(){
    cap.open(0);
    if (cap.isOpened())
    {
        cap.release();
        return true;
    }
    else
        return false;
}

void
SetupView::showCameraOutput(){
    if(!this->isVisible()) return;
    cv::Mat image;
    cap.open(0);
    if(!cap.isOpened())
        return;
    while(ui->checkBox->checkState() == Qt::Checked
          and !this->isMinimized()
          and this->isVisible()) {
        if(!cap.grab()){
            qDebug()<< "Can't grab video";
        }
        else{
            if(!cap.retrieve(image)){
                qDebug() << "Can't retrieve image";
            }
        }
        QSize frSize = ui->frmVideoStream->frameSize();
        cv::Size sz;
        /* Соотношение сторон кадра 4:3
         * на моей камере
        */
        if(frSize.width()/4 < frSize.height()/3){
            sz.width = frSize.width()/4;
            sz.height = sz.width * 3;
            sz.width *= 4;
        }
        else if(frSize.width()/4 > frSize.height()/3){
            sz.height = frSize.height() / 3;
            sz.width = sz.height * 4;
            sz.height *= 3;
        }
        else{
            sz.width = frSize.width();
            sz.height = frSize.height();
        }
        cv::Mat scaled;
        cv::resize(image, scaled, sz, 0, 0);
        std::vector<cv::Rect> faces = findFaces(&scaled,
                                        ui->sldrMinSize->value(),
                                        ui->sldrMaxSize->value());
        for(const auto &f: faces){
            cv::rectangle(scaled, f, cv::Scalar(0, 255, 0));
        }
        int cx = scaled.cols/2;
        int cy = scaled.rows/2;
        int w_min = scaled.cols*ui->sldrMinSize->value()/100;
        int w_max = scaled.cols*ui->sldrMaxSize->value()/100;
        cv::rectangle(scaled,
                      cv::Point(cx-w_min/2, cy-w_min/2),
                      cv::Point(cx+w_min/2, cy+w_min/2),
                      cv::Scalar(0, 255,255));
        cv::rectangle(scaled,
                      cv::Point(cx-w_max/2, cy-w_max/2),
                      cv::Point(cx+w_max/2, cy+w_max/2),
                      cv::Scalar(0, 0, 255));
        cv::cvtColor(scaled,scaled,CV_BGR2RGB);
        QImage img= QImage((const unsigned char*)(scaled.data),
                           scaled.cols,scaled.rows,QImage::Format_RGB888);
        emit imageChanged(QPixmap::fromImage(img));
    }
    cap.release();
    return;
}

void
SetupView::setVideoFrmPicture(QPixmap pctr){
    ui->lblPicture->setPixmap(pctr);
}

void
SetupView::closeEvent(QCloseEvent *ev){
    emit setupViewOpened(false);
    ev->accept();
}

void
SetupView::showEvent(QShowEvent* ev){
    /*set values according to settings */
    ui->edtLogin->setText(SedstatsSettings::Instance().login);
    ui->edtPassword->setText(SedstatsSettings::Instance().passwd);
    ui->checkBox->setChecked(SedstatsSettings::Instance().camUsing);
    ui->spnMaxWorkingPeriod->setValue(SedstatsSettings::Instance().maxWorkingPeriod);
    ui->sldrMinSize->setValue(SedstatsSettings::Instance().minWidth);
    ui->sldrMaxSize->setValue(SedstatsSettings::Instance().maxWidth);
    ui->cmbLanguage->setCurrentIndex(
                ui->cmbLanguage->findText(SedstatsSettings::Instance().lang));
    if(!ui->checkBox->isChecked()){
        ui->lblCamStream->setEnabled(false);
        ui->lblFaceObjSize->setEnabled(false);
        ui->sldrMinSize->setEnabled(false);
        ui->sldrMaxSize->setEnabled(false);
        ui->lblTo->setEnabled(false);
        ui->lblPicture->setEnabled(false);
    }

    if(initCamera()){
        ui->lblPicture->setText(tr("We have a camera."));
        emit setupViewOpened(true);
        emit changeCheckCamera(Qt::Checked);
        QFuture<void> res =
                QtConcurrent::run(this,
                                  &SetupView::showCameraOutput);
    }else{
        ui->lblPicture->setText(tr("We have no camera"));
    }
    ev->accept();
}

void
SetupView::hideEvent(QHideEvent* ev){
    emit setupViewOpened(false);
    ev->accept();
}

void
SetupView::changeEvent(QEvent *ev){

    if (ev->type() == QEvent::LanguageChange){
        ui->retranslateUi(this);
        int idx = ui->cmbLanguage->findText(m_currentLanguage);
        ui->cmbLanguage->setCurrentIndex(idx);
    }
    QWidget::changeEvent(ev);
}

void SetupView::on_cmbLanguage_activated(const QString &arg1)
{
    SedstatsSettings::Instance().lang = arg1;
    m_currentLanguage = arg1;
    emit languageChanged(arg1);

}
