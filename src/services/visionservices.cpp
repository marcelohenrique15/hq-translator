#include "services/visionservices.h"
#include <QDebug>

using namespace cv;
using namespace std;
using namespace::chrono;

VisionServices::VisionServices(QObject *parent) : QObject(parent) {}

void VisionServices::stop()
{
    m_running = false;
}

void VisionServices::process()
{
    m_cap.open(0);

    if (!m_cap.isOpened()) {
        cerr << "Erro: Câmera não foi iniciada corretamente.\n";
        emit finished(); 
        return;
    }

    while (m_running){
        Mat frame;
        m_cap >> frame;

        if (frame.empty()){
            continue;
        }

        long long current_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        if (current_time - m_last_detection_time >= m_detection_interval_ms){
            m_last_detection_time = current_time;
            m_detection_text = m_tesseract.detect_text_box(frame);
        }

        for (const auto& dt : m_detection_text){
            string translated;

            auto it = m_translation_cache.find(dt.text);
            if (it != m_translation_cache.end()){
                translated = it->second;
            }

            else{
                translated = m_ia.translate(dt.text);
                m_translation_cache[dt.text] = translated;
            }

            Overlay::drawTextOverlay(frame, translated, dt.box);
        }

        cvtColor(frame, frame, COLOR_BGR2RGB);
        QImage qimg((uchar*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
        emit frameReady(qimg.copy());
    }

    m_cap.release();
    emit finished();
    qDebug() << "Trabalho em paralelo do VisionServices finalizado.";
}