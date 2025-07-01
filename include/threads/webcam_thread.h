#pragma once

#include <thread>
#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <opencv2/opencv.hpp>
#include "capture/webcam_capture.h"

class WebcamThread {
public:
    WebcamThread(WebcamCapture& capture,
                 std::queue<cv::Mat>& outputQueue,
                 std::mutex& mtx,
                 std::condition_variable& cond);

    void start();
    void stop();
    void join();

private:
    void run();

    WebcamCapture& cam;
    std::queue<cv::Mat>& frameQueue;
    std::mutex& queueMutex;
    std::condition_variable& condition;
    std::atomic<bool> running;
    std::thread thread;
};
