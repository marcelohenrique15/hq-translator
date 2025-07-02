#include "struct/struct_data.h"
#include "translation/LibreTranslate.h"
#include "threads/translate_thread.h"

#include <thread>
#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <nlohmann/json.hpp>

using namespace std;

// ======= CONSTRUCTOR =======

TranslateThread::TranslateThread(
    LibreTranslateAPI& api,
    queue<std::vector<textData>>& inputQueue,
    vector<textData>& outputQueue,
    mutex& inputMtx,
    mutex& outputMtx,
    condition_variable& cond
) :
    ai(api),
    toTranslateQueue(inputQueue),
    translatedText(outputQueue),
    toTranslateMutex(inputMtx),
    translatedMutex(outputMtx),
    condition(cond),
    running(false){}

// ============================

void TranslateThread::start()
{
    running = true;
    thread = std::thread(&TranslateThread::run, this);
}

void TranslateThread::stop()
{
    running = false;
}

void TranslateThread::join()
{
    if(thread.joinable())
    {
        thread.join();
    }
}

void TranslateThread::run()
{
    while (running) 
    {
        vector<textData> toTranslate;

        {
            unique_lock<mutex> lock(toTranslateMutex);
            condition.wait(lock, [&]() {
                return !toTranslateQueue.empty() || !running;
            });
        }

        if (!running) break;
    
        toTranslate = toTranslateQueue.front();
        toTranslateQueue.pop();

        for (auto& dt : toTranslate)
        {
            auto it = translation_cache.find(dt.original);
            if (it != translation_cache.end()) 
            {
                dt.translated = it->second;
            } 

            else 
            {
                string result = ai.translateText(dt.original, "en", "pt");
                dt.translated = result;
                translation_cache[dt.original] = result;
            }
        }
        lock_guard<mutex> lock(translatedMutex);
        translatedText = toTranslate;
        condition.notify_one();
    }
}

