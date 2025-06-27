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

using namespace std;
using namespace cv;

class Detector
{
    private:
        bool is_alphanumeric(const string& text);
        Mat preprocess_image(const Mat& frame);

    public:
        struct TextDetection
            {
                string text;
                Rect box;
            };
            
        vector<TextDetection> detect_text_box(const Mat& frame);
};