#include "capture/webcam_capture.h"
#include "detection/text_detector.h"
#include "overlayer/overlay_renderer.h"
#include "translation/LibreTranslate.h"
#include "threads/webcam_thread.h"

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
    LibreTranslateAPI ai("http://localhost:5001/");

    if (!webcam.isOpened()) {
        cerr << "Camera nao foi iniciada corretamente.\n";
        return 1;
    }

    // ========== FILAS E MUTEXES ==========

    queue<Mat> frameQueue;
    mutex frameMutex;
    condition_variable frameCond;

    queue<Mat> ocrQueue;
    mutex ocrMutex;
    condition_variable ocrCond;

    queue<vector<Detector::TextDetection>> translationQueue;
    mutex translationMutex;
    condition_variable translationCond;

    vector<Detector::TextDetection> translatedDetections;
    mutex translatedMutex;

    unordered_map<string, string> translation_cache;

    // ========== THREAD DE CAPTURA ==========
    WebcamThread webcamThread(webcam, frameQueue, frameMutex, frameCond);
    webcamThread.start();

    // ========== THREAD DE OCR ==========
    atomic<bool> ocrRunning(true);
    thread ocrThread([&]() {
        while (ocrRunning) {
            Mat frameForOCR;

            {
                unique_lock<mutex> lock(ocrMutex);
                ocrCond.wait(lock, [&]() {
                    return !ocrQueue.empty() || !ocrRunning;
                });

                if (!ocrRunning) break;

                frameForOCR = ocrQueue.front();
                ocrQueue.pop();
            }

            auto detections = tesseract.detect_text_box(frameForOCR);

            {
                lock_guard<mutex> lock(translationMutex);
                translationQueue.push(detections);
            }
            translationCond.notify_one();
        }
    });

    // ========== THREAD DE TRADUÇÃO ==========
    atomic<bool> translationRunning(true);
    thread translateThread([&]() {
        while (translationRunning) {
            vector<Detector::TextDetection> toTranslate;

            {
                unique_lock<mutex> lock(translationMutex);
                translationCond.wait(lock, [&]() {
                    return !translationQueue.empty() || !translationRunning;
                });

                if (!translationRunning) break;

                toTranslate = translationQueue.front();
                translationQueue.pop();
            }

            for (auto& dt : toTranslate) {
                if (dt.text.empty()) continue; // skip texto vazio

                auto it = translation_cache.find(dt.text);
                if (it != translation_cache.end()) {
                    dt.translated = it->second;
                } else {
                    try {
                        string result = ai.translateText(dt.text, "en", "pt");
                        dt.translated = result;
                        translation_cache[dt.text] = result;
                    } catch (const std::exception& e) {
                        dt.translated = "[ERRO]";
                        cerr << "[EXCEPTION] " << e.what() << endl;
                    } catch (const char* msg) {
                        dt.translated = "[ERRO]";
                        cerr << "[ERRO C] " << msg << endl;
                    } catch (...) {
                        dt.translated = "[ERRO]";
                        cerr << "[ERRO] Tradução falhou (exceção desconhecida)." << endl;
                    }
                }
            }

            {
                lock_guard<mutex> lock(translatedMutex);
                translatedDetections = toTranslate;
            }
        }
    });

    // ========== LOOP PRINCIPAL ==========
    bool running = true;
    long long last_detection_time = 0;
    const long long detection_interval_ms = 150;

    while (running) {
        Mat frame;

        {
            unique_lock<mutex> lock(frameMutex);
            frameCond.wait(lock, [&]() { return !frameQueue.empty(); });
            frame = frameQueue.front();
            frameQueue.pop();
        }

        // A cada 150ms, envia um novo frame para o OCR
        long long current_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        if (current_time - last_detection_time >= detection_interval_ms) {
            last_detection_time = current_time;

            {
                lock_guard<mutex> lock(ocrMutex);
                ocrQueue.push(frame.clone());
            }
            ocrCond.notify_one();
        }

        // Pega o último resultado traduzido
        vector<Detector::TextDetection> detected_text;
        {
            lock_guard<mutex> lock(translatedMutex);
            detected_text = translatedDetections;
        }

        for (const auto& dt : detected_text) {
            Overlay::drawTextOverlay(frame, dt.translated, dt.box);
        }

        imshow("Webcam", frame);

        if (waitKey(1) == 27) {
            running = false;  // ESC
        }
    }

    // ========== FINALIZAÇÃO ==========
    webcamThread.stop();
    frameCond.notify_all();
    webcamThread.join();

    ocrRunning = false;
    ocrCond.notify_all();
    ocrThread.join();

    translationRunning = false;
    translationCond.notify_all();
    translateThread.join();

    destroyAllWindows();
    return 0;
}
