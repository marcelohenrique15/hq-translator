#include "gui/cameraworker.h"
#include <QDebug>

CameraWorker::CameraWorker(QObject *parent) : QObject(parent){}

void CameraWorker::stop()
{
    m_running = false;
}

void CameraWorker::process()
{
    m_cap.open(0);
    if(!m_cap.isOpened()){
        qWarning() << "Erro: Câmera não foi iniciada corretamente.";
        emit finished();
        return;
    }

    while (m_running) {
        cv::Mat frame;
        m_cap >> frame;

        if(!frame.empty()) {
            cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
            QImage qimg((uchar*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
            emit frameReady(qimg.copy());

        }
    }

    m_cap.release();
    emit finished();
    qDebug() << "Trabalho em paralelo da câmera finalizado.";
}
