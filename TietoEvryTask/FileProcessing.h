#pragma once
#include <string>
#include "LogSystem.h"
#include <atomic>
#include "Statistic.h"

class FileProcessing
{

public:
    FileProcessing(LogSystem* logSystem = nullptr);

    void StartProcessing(std::string pattern, std::string path);

    Statictic GetStatistic() const;
private:

    std::atomic_uint _searched = 0;
    std::atomic_uint _withPattern = 0;
    std::atomic_uint _patterns = 0;

    LogSystem* _logSystem;
};