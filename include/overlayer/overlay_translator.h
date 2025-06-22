#pragma once
#include <opencv2/opencv.hpp>
#include <string>

using namespace cv;
using namespace std;

class Overlay
{
    public:
        static void drawTextOverlay(Mat& frame, const string& translated_text, const cv::Rect& origin);
};