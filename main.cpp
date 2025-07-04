#include "capture/webcam_capture.h"
#include "detection/text_detector.h"
#include "overlayer/overlay_renderer.h"
#include "translation/LibreTranslate.h"

#include "threads/webcam_thread.h"
#include "threads/translate_thread.h"
#include "threads/detection_thread.h"

#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>

using namespace std;
using namespace cv;
using namespace std::chrono;

int main() {
    WebcamCapture webcam;
    Overlay overlayer;
    Detector tesseract;
    LibreTranslateAPI ai("http://libretranslate_api:5000/");

    if (!webcam.isOpened()) {
        cerr << "Camera nao foi iniciada corretamente.\n";
        return 0;
    }

    // ========== FILAS E SINCRONIZAÇÃO ==========

    queue<Mat> frameQueue;
    mutex frameMutex;
    condition_variable frameCond;

    queue<vector<textData>> toTranslateQueue;
    mutex toTranslateMutex;
    condition_variable translationCond;

    vector<textData> translatedText;
    mutex translatedMutex;

    // ========== THREADS ==========

    WebcamThread webcamThread(webcam, frameQueue, frameMutex, frameCond);
    webcamThread.start();

    DetectionThread detectionThread(tesseract, frameQueue, toTranslateQueue, frameMutex, toTranslateMutex, frameCond, translationCond);
    detectionThread.start();

    TranslateThread translateThread(ai, toTranslateQueue, translatedText, toTranslateMutex, translatedMutex, translationCond);
    translateThread.start();

    // ========== LOOP PRINCIPAL ==========

    bool running = true;
    long long last_detection_time = 0;
    const long long detection_interval_ms = 500;

    while (running) {
        Mat frame;

        {
            unique_lock<mutex> lock(frameMutex);
            frameCond.wait(lock, [&]() { return !frameQueue.empty(); });
            frame = frameQueue.front();
            frameQueue.pop();
        }

        // A cada 500ms (usado anteriormente para controle de OCR)
        long long current_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        if (current_time - last_detection_time >= detection_interval_ms) {
            last_detection_time = current_time;
        }

        // Atualiza textos traduzidos
        vector<textData> detected_text;
        {
            lock_guard<mutex> lock(translatedMutex);
            detected_text = translatedText;
        }

        for (const auto& dt : detected_text) {
            cout << dt.translated << endl;
            Overlay::drawTextOverlay(frame, dt.translated, dt.box);
        }

        imshow("Webcam", frame);

        if (waitKey(1) == 27) {  // ESC
            running = false;
        }
    }

    // ========== FINALIZAÇÃO ==========

    webcamThread.stop();
    frameCond.notify_all();
    webcamThread.join();

    detectionThread.stop();
    frameCond.notify_all();
    detectionThread.join();

    translateThread.stop();
    translationCond.notify_all();
    translateThread.join();

    destroyAllWindows();
    return 0;
}
