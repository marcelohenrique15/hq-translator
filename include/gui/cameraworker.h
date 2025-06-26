#ifndef CAMERAWORKER_H
#define CAMERAWORKER_H

#include <QObject>
#include <QImage>
#include <opencv2/opencv.hpp>

class CameraWorker :public QObject
{
    Q_OBJECT

public:
    explicit CameraWorker(QObject *parent = nullptr);
    void stop();

public slots:
    void process();

signals:
    void frameReady(const QImage &image);
    void finished();

private:
    bool m_running = true;
    cv::VideoCapture m_cap;

};

#endif