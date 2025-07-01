#pragma once

#include <thread>
#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <nlohmann/json.hpp>

#include "translation/LibreTranslate.h"
#include "struct/struct_data.h"

class TranslateThread
{
public:
    TranslateThread(LibreTranslateAPI& api, 
                    std::queue<std::vector<textData>>& inputQueue,
                    std::vector<textData>& outputQueue,
                    std::mutex& inputMtx,
                    std::mutex& outputMtx,
                    std::condition_variable& cond);

    void start();
    void stop();
    void join();

private:
    void run();

    LibreTranslateAPI& ai;

    std::queue<std::vector<textData>>& toTranslateQueue;
    std::vector<textData>& translatedText;
    std::mutex& toTranslateMutex;
    std::mutex& translatedMutex;
    std::condition_variable& condition;

    std::thread thread;
    std::atomic<bool> running;

    std::unordered_map<std::string, std::string> translation_cache;
};
