#ifndef SETUPVIEW_H
#define SETUPVIEW_H

#include <QWidget>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QTimer>
#include <QPixmap>
#include <QFuture>

namespace Ui {
class SetupView;
}

class SetupView : public QWidget
{
    Q_OBJECT
public:
    explicit SetupView(QWidget *parent = 0);
    ~SetupView();
signals:
    void changeMaxWorkingTime(unsigned int t);
    void changeMinMaxFaceSize(unsigned int min, unsigned int max);
    void changeCheckCamera(Qt::CheckState ch);
    void imageChanged(QPixmap pxmp);
    void setupViewOpened(bool opened);
    void languageChanged(QString lang);

private slots:
    void on_spnMaxWorkingPeriod_valueChanged(int arg1);
    void on_checkBox_stateChanged(int arg1);
    void on_sldrMinSize_valueChanged(int value);
    void on_sldrMinSize_sliderMoved(int position);
    void on_sldrMaxSize_valueChanged(int value);
    void on_sldrMaxSize_sliderMoved(int position);
    void setVideoFrmPicture(QPixmap pctr);

    void on_cmbLanguage_activated(const QString &arg1);

private:
    Ui::SetupView *ui;
    bool initCamera();
    void showCameraOutput();
    cv::VideoCapture cap;
    QTimer m_videoTimer;
    QFuture<void> m_res;
    QString m_currentLanguage;
    void closeEvent(QCloseEvent *ev);
    void showEvent(QShowEvent *ev);
    void hideEvent(QHideEvent *ev);
    void changeEvent(QEvent *ev);
};

#endif // SETUPVIEW_H
