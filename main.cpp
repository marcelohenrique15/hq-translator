#include "capture/webcam_capture.h"
#include "detection/text_detector.h"
#include "overlayer/overlay_translator.h"
#include "translation/translator.h"

using namespace std;
using namespace cv;

int main() {
    WebcamCapture webcam;
    Overlay overlayer;
    

    if (!webcam.isOpened())
    {
        cerr << "Câmera não foi iniciada corretamente.\n";
        return -1;
    }

    Mat frame;
    string text = "Erwin Smith, the commander of the Survey Corps in Attack on Titan. ";
    string translated_text = ask_ia(text);
    cout << translated_text;
    
    while (true)
    {
        if (!webcam.readFrame(frame))
        {
            cerr << "Falha ao capturar frame.\n";
            break;
        }
        
        Rect textBox(100, 100, 300, 50);
        overlayer.drawTextOverlay(frame, translated_text, textBox);
        imshow("Webcam", frame);
        
        if (waitKey(1) == 27) // Tecla ESC para fechar programa
        {
            break;
        }
    }

    destroyAllWindows();
    return 0;
}

//Use ./run.sh no terminal.