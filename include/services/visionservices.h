#pragma once

#include <QObject>
#include <QImage>

#include <opencv2/opencv.hpp>
#include "capture/webcam_capture.h"
#include "detection/text_detector.h"
#include "overlayer/overlay_renderer.h"
#include "translation/translate.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <chrono>

using namespace std;
using namespace cv;

class VisionServices : public QObject
{
    Q_OBJECT

    public:
        explicit VisionServices(QObject *parent = nullptr);
        void stop();

    public slots:
        void process();

    signals:
        void frameReady(const QImage &image);
        void finished();

    private:
        bool m_running = true;

        VideoCapture m_cap;
        Detector m_tesseract;
        Gemini m_ia;

        long long m_last_detection_time = 0;
        long long m_detection_interval_ms = 1000;
        unordered_map<string, string> m_translation_cache;
        vector<Detector::TextDetection> m_detection_text;
};