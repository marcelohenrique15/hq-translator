#include "capture/webcam_capture.h"
#include "detection/text_detector.h"
#include "overlayer/overlay_renderer.h"
#include <translation/translate.h>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <chrono>
#include <thread>

using namespace std;
using namespace cv;
using namespace std::chrono;

string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r\f\v");
    if (string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(first, (last - first + 1));
}

string extract_value(const string& line, const string& prefix) {
    size_t pos = line.find(prefix);
    if (pos != string::npos) {
        return trim(line.substr(pos + prefix.length()));
    }
    return "";
}

int main() {
    WebcamCapture webcam;
    Overlay overlayer;

    if (!webcam.isOpened())
    {
        cerr << "Camera nao foi iniciada corretamente.\n";
        return -1;
    }

    Mat frame;

    long long last_detection_time = 0;
    long long detection_interval_ms = 1000; // Detectar a cada 1 segundo

    string text_to_translate_fixed = "SHINZO WO SASAGEYO";
    string search_query = "Attack on Titan";
    static Gemini ai;

    vector<detection::DetectedText> current_detected_texts;

    while (true)
    {
        if (!webcam.readFrame(frame))
        {
            cerr << "Falha ao capturar frame.\n";
            break;
        }

        long long current_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        if (current_time - last_detection_time >= detection_interval_ms) {
            last_detection_time = current_time;
            current_detected_texts = detection::detect_text_and_boxes(frame);
        }

        detection::draw_text_boxes(frame, current_detected_texts);

        imshow("Webcam", frame);

        if (waitKey(1) == 27)
        {
            break;
        }
    }

    destroyAllWindows();
    return 0;
}