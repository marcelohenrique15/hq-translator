#pragma once

#include <thread>
#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>

#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <opencv2/imgproc.hpp>

#include <string>
#include <vector>
#include <iostream>

#include "detection/text_detector.h"
#include "struct/struct_data.h"

class DetectionThread
{
    public:
        DetectionThread(Detector& tesseract,
                        std::queue<cv::Mat>& inputQueue,
                        std::queue<std::vector<textData>>& outputQueue,
                        std::mutex& inputMtx,
                        std::mutex& outputMtx,
                        std::condition_variable& inputCond,
                        std::condition_variable& outputCond);
        void start();
        void stop();
        void join();

    private:
        void run();
        
        Detector& ai;
        std::queue<cv::Mat>& toDetectQueue;
        std::queue<std::vector<textData>>& detectedQueue;
        std::mutex& toDetectMutex;
        std::mutex& detectedMutex;
        std::condition_variable& inCondition;
        std::condition_variable& outCondition;

        std::atomic<bool> running;
        std::thread thread;
};