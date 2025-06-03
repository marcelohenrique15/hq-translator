#include "capture/webcam_capture.h"
#include "detection/text_detector.h"
#include "overlayer/overlay_translator.h"
#include "translation/translator.h"
#include <iostream>

using namespace std;

int main() {
    WebcamCapture webcam;

    if (!webcam.isOpened()) {
        std::cerr << "Câmera não foi iniciada corretamente.\n";
        return -1;
    }

    cv::Mat frame;
    while (true) {
        if (!webcam.readFrame(frame)) {
            std::cerr << "Falha ao capturar frame.\n";
            break;
        }

        cv::imshow("Webcam", frame);

        if (cv::waitKey(1) == 27) { // Tecla ESC
            break;
        }
    }

    cv::destroyAllWindows();
    return 0;
}

//Use ./run.sh no terminal.