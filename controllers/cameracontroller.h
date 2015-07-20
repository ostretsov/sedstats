#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <QObject>
#include <opencv2/highgui/highgui.hpp>
namespace cv {
class Mat;
}

class QPixmap;

class CameraController : public QObject
{
    Q_OBJECT
public:
    explicit CameraController(QObject *parent = 0);
signals:
    void imageProcessed(bool facePresent);
    void m_tenSecondsFinished(bool hf);
public slots:
    void procCamInput();
    void setMinMaxFaceSizes(unsigned int min, unsigned int max);
    void setCheckCamera(Qt::CheckState ch);
    void m_tenSecondsResult(bool hf);
private:
    bool m_cameraInit();
    bool m_hasFaceOnPicture(cv::Mat *picture);
    unsigned int m_minFaceSize, m_maxFaceSize; //в процентах
    Qt::CheckState m_checkCamera;
    bool m_tenSecondsFunc(cv::VideoCapture cap);
};

#endif // CAMERACONTROLLER_H
