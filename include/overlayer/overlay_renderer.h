#pragma once
#include <opencv2/opencv.hpp>
#include <string>

using namespace std;

class Overlay
{
    public:
        static void drawTextOverlay(cv::Mat& frame, const std::string& translated_text, const cv::Rect& box);
};