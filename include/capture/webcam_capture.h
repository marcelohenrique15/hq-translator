#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

class WebcamCapture
{
    private:
        VideoCapture cap;

    public:
        WebcamCapture(int cam = 0, int width = 640, int height = 480, double fps = 30.0);
        ~WebcamCapture();

        bool isOpened() const;
        bool readFrame(Mat& frame);
};

