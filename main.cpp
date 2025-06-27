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
#include <unordered_map>

using namespace std;
using namespace cv;
using namespace std::chrono;

int main() {
    WebcamCapture webcam;
    Overlay overlayer;
    Gemini ai;

    if (!webcam.isOpened()) {
        cerr << "Camera nao foi iniciada corretamente.\n";
        return -1;
    }

    Mat frame;
    long long last_detection_time = 0;
    long long detection_interval_ms = 1000;

    vector<detection::DetectedText> current_detected_texts;
    unordered_map<string, string> translation_cache;

    while (true) {
        if (!webcam.readFrame(frame)) {
            cerr << "Falha ao capturar frame.\n";
            break;
        }

        long long current_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        if (current_time - last_detection_time >= detection_interval_ms) {
            last_detection_time = current_time;
            current_detected_texts = detection::detect_text_and_boxes(frame);
        }

        for (const auto& dt : current_detected_texts) {
            string translated;

            // Verifica se o texto já foi traduzido anteriormente
            auto it = translation_cache.find(dt.text);
            if (it != translation_cache.end()) {
                translated = it->second;
            } else {
                translated = ai.translate(dt.text);
                translation_cache[dt.text] = translated;
            }

            Overlay::drawTextOverlay(frame, translated, dt.bbox);
        }

        imshow("Webcam", frame);

        if (waitKey(1) == 27) {
            break;  // Tecla ESC para sair
        }
    }

    destroyAllWindows();
    return 0;
}
