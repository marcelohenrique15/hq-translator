#include "threads/webcam_thread.h"

using namespace std;
using namespace std::chrono;

// ======= CONSTRUCTOR =======

WebcamThread::WebcamThread(
    WebcamCapture& capture, 
    queue<cv::Mat>& outputQueue, 
    mutex& mtx, 
    condition_variable& cond
): 
    cam(capture), 
    frameQueue(outputQueue), 
    queueMutex(mtx), 
    condition(cond), 
    running(false) {}

// ============================

void WebcamThread::start() 
{
    running = true;
    thread = std::thread(&WebcamThread::run, this);
}

void WebcamThread::stop() 
{
    running = false;
}

void WebcamThread::join() 
{
    if (thread.joinable()) 
    {
        thread.join();
    }
}

void WebcamThread::run() {
    while (running) {
        cv::Mat frame;
        if (!cam.readFrame(frame)) continue;

        {
            lock_guard<mutex> lock(queueMutex);
            frameQueue.push(frame);
        }

        condition.notify_one();
        this_thread::sleep_for(milliseconds(30));
    }
}
