#pragma once

#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <opencv2/imgproc.hpp>

#include <string>
#include <vector>
#include <iostream>

class Detector
{
private:
    bool is_alphanumeric(const std::string& text);
    cv::Mat preprocess_image(const cv::Mat& frame);

public:
    struct TextDetection
    {
        std::string text;
        cv::Rect box;
        std::string translated;
    };

    std::vector<TextDetection> detect_text_box(const cv::Mat& frame);
};
