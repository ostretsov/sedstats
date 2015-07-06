#include "ocvroutines.h"

#include <opencv2/imgproc/imgproc.hpp>

#include <QString>
#include <QDebug>
#include <QCoreApplication>

std::vector<cv::Rect> findFaces(cv::Mat *m,
                                unsigned int percent_min,
                                unsigned int percent_max){
    std::vector<cv::Rect> objects;
    cv::CascadeClassifier face_cascade;
    QString cascade_path = QCoreApplication::applicationDirPath() + "/haarcascades/haarcascade_frontalface_alt.xml";
    if (!face_cascade.load(cascade_path.toStdString())){
        qDebug() << "No cascade!!!";
        return objects;
    }
    cv::Mat frame = m->clone();
    cv::cvtColor(frame, frame, CV_BGR2GRAY );
    cv::equalizeHist(frame, frame);
    cv::Size min(frame.cols*percent_min/100, frame.cols*percent_min/100);
    cv::Size max(frame.cols*percent_max/100, frame.cols*percent_max/100);
    face_cascade.detectMultiScale(frame,
                     objects,
                     1.1,
                     2,
                     0,
                     min,
                     max);
    return objects;
}
