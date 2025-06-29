#include "capture/webcam_capture.h"

using namespace cv;
using namespace std;

WebcamCapture::WebcamCapture(int cam, int width, int height, double fps) : cap(cam)
{
    if(!cap.isOpened())
    {
        cout << "Erro ao ligar a câmera.\n";
        return;
    }

    cap.set(CAP_PROP_FRAME_WIDTH, width);
    cap.set(CAP_PROP_FRAME_HEIGHT, height);
    cap.set(CAP_PROP_FPS, fps);
}

WebcamCapture::~WebcamCapture()
{
    if(cap.isOpened())
    {
        cap.release();
    }
}

bool WebcamCapture::isOpened() const
{
    return cap.isOpened();
}

bool WebcamCapture::readFrame(Mat& frame)
{
    return cap.read(frame);
}