#pragma once
#include <opencv2/opencv.hpp>

using namespace cv;

class WebcamCapture
{
    private:
        VideoCapture cap;

    public:
        WebcamCapture(int cam = 0);
        ~WebcamCapture();

        bool isOpened() const;
        bool readFrame(Mat& frame);
};