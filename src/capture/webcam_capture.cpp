#include "capture/webcam_capture.h"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

WebcamCapture::WebcamCapture(int cam) : cap(cam)
{
    if(!cap.isOpened())
    {
        cout << "Erro ao ligar a câmera.";
    }
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