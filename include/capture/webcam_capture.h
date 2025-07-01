#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

class WebcamCapture
{
    private:
        VideoCapture cap;

    public:
        WebcamCapture(int cam = CAP_V4L2, int width = 1280, int height = 720, double fps = 30.0);
        ~WebcamCapture();

        bool isOpened() const;
        bool readFrame(Mat& frame);
};

