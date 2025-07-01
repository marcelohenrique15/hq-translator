#pragma once

#include <opencv2/opencv.hpp>
#include <iostream>

struct textData 
    {
      std::string original;
      std::string translated;
      cv::Rect box;
    };