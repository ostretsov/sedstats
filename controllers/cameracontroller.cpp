#include "controllers/cameracontroller.h"
#include "controllers/sedstatssettings.h"
#include "ocvroutines.h"

#include <QDebug>
#include <QCoreApplication>
#include <QTime>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <vector>


CameraController::CameraController(QObject *parent) :
    QObject(parent)
{
    if(SedstatsSettings::Instance().getCamUsing()){
        m_checkCamera = Qt::Checked;
    }else{
        m_checkCamera = Qt::Unchecked;
    }
    m_minFaceSize = SedstatsSettings::Instance().getMinWidth();
    m_maxFaceSize = SedstatsSettings::Instance().getMaxWidth();
    connect(this, &CameraController::m_tenSecondsFinished,
            this, &CameraController::m_tenSecondsResult,
            Qt::QueuedConnection);
}

bool
CameraController::m_cameraInit(){
    cv::VideoCapture capture(0);//взять дефолтную камеру
    //может вставить в форму setup выбор камеры?
    //вдруг там не одна?
    bool cam = capture.isOpened();
    capture.release();
    return cam;
}

void
CameraController::procCamInput(){
    qDebug() << "\t Proc cam input";
    if(m_checkCamera != Qt::Checked){
        qDebug() << "\t No need to proc camera";
        emit imageProcessed(false);
        return;
    }
    cv::VideoCapture cap(0);
    if (!cap.isOpened()){
        qDebug() << "\t No capture opened";
        emit imageProcessed(false);
        return;
    }
    //Здесь получать изображение до 10 сек
    // или до обнаружения физиономии
    QtConcurrent::run(this, &CameraController::m_tenSecondsFunc, cap);
}

bool
CameraController::m_tenSecondsFunc(cv::VideoCapture cap){
    qDebug() << "\t\t\t\t Start 10 sec cicle";
    cv::Mat image;
    QTime _t;
    _t.start();
    int i = 0;
    bool _facePresent = false;
    while(_t.elapsed() < 10000 /*10 sec in mills*/
          and !_facePresent
          and m_checkCamera == Qt::Checked){
        cap >> image;
        _facePresent = m_hasFaceOnPicture(&image);
        i++;
    }
    qDebug() << "\t\t\t\t"<< i << " frames processed. Time ellapsed " << _t.elapsed() << "msec";
    qDebug() << "\t\t\t\t Face present = " << _facePresent;
    emit m_tenSecondsFinished(_facePresent);
    return _facePresent;
}

void
CameraController::m_tenSecondsResult(bool hf){
    emit imageProcessed(hf);
}

void
CameraController::setCheckCamera(Qt::CheckState ch){
    m_checkCamera = ch;
}

bool
CameraController::m_hasFaceOnPicture(cv::Mat *picture){
    std::vector<cv::Rect>
            objects = findFaces(picture, m_minFaceSize, m_maxFaceSize);
    if (objects.size() > 0){
        return true;
    }else{
        return false;
    }
}

void
CameraController::setMinMaxFaceSizes(unsigned int min, unsigned int max){
    m_minFaceSize = min;
    m_maxFaceSize = max;
}
