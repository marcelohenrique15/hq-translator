#pragma once

#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <memory>
#include <algorithm>
#include <cctype>

namespace detection {

    struct DetectedText {
        std::string text;
        cv::Rect bbox;
    };

    std::vector<DetectedText> detect_text_and_boxes(const cv::Mat& image);

    void draw_text_boxes(cv::Mat& frame, const std::vector<DetectedText>& detected_texts);

    cv::Mat preprocess_image(const cv::Mat& input_image);

} 

