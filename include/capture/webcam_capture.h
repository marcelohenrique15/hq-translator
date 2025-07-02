#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>

class WebcamCapture
{
    private:
        cv::VideoCapture cap;

    public:
        WebcamCapture(int cam = cv::CAP_V4L2, int width = 1280, int height = 720, double fps = 60.0);
        ~WebcamCapture();

        bool isOpened() const;
        bool readFrame(cv::Mat& frame);
};

