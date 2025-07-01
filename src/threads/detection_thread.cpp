#include "threads/detection_thread.h"
#include "struct/struct_data.h"
#include "detection/text_detector.h"

#include <thread>
#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>

using namespace std;
using namespace std::chrono;

DetectionThread::DetectionThread(
    Detector& tesseract,
    std::queue<cv::Mat>& inputQueue,
    std::queue<std::vector<textData>>& outputQueue,
    std::mutex& inputMtx,
    std::mutex& outputMtx,
    std::condition_variable& inputCond,
    std::condition_variable& outputCond
) :
    ai(tesseract),
    toDetectQueue(inputQueue),
    detectedQueue(outputQueue),
    toDetectMutex(inputMtx),
    detectedMutex(outputMtx),
    inCondition(inputCond),
    outCondition(outputCond),
    running(false){}

void DetectionThread::start()
{
    running = true;
    thread = std::thread(&DetectionThread::run, this);
}

void DetectionThread::stop()
{
    running = false;
}

void DetectionThread::join()
{
    if(thread.joinable())
    {
        thread.join();
    }
}

void DetectionThread::run()
{
    while(running)
    {
        cv::Mat toDetect;

        {
            unique_lock<mutex> lock(toDetectMutex);
            inCondition.wait(lock, [&](){
                return !toDetectQueue.empty() || !running;
            });
        }

        if(!running) break;

        toDetect = toDetectQueue.front();
        toDetectQueue.pop();

        std::vector<textData> detectedText = ai.detect_text_box(toDetect);

        {
            lock_guard<mutex> lock(detectedMutex);
            detectedQueue.push(detectedText);
        }
        outCondition.notify_one();
        this_thread::sleep_for(milliseconds(30));
    }
}