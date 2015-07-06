#include "controllers/cameracontroller.h"
#include "ocvroutines.h"

#include <QDebug>
#include <QCoreApplication>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <vector>


CameraController::CameraController(QObject *parent) :
    QObject(parent)
{
    m_checkCamera = Qt::Unchecked;
    m_minFaceSize = 20;
    m_maxFaceSize = 50; //в процента от ширины изображения
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
    cv::VideoCapture cap(0);
    if (!cap.isOpened()){
        emit imageProcessed(false);
        return;
    }
    cv::Mat image;
    cap >> image;
    cap.release();

    emit imageProcessed(m_hasFaceOnPicture(&image));
}

void
CameraController::setCheckCamera(Qt::CheckState ch){
    m_checkCamera = ch;
}

bool
CameraController::m_hasFaceOnPicture(cv::Mat *picture){
    if(m_checkCamera != Qt::Checked)
        return false;
    std::vector<cv::Rect> objects = findFaces(picture,
                                              m_minFaceSize,
                                              m_maxFaceSize
                                              );

    for(auto const o: objects){
        qDebug() << "Face";
    }
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
