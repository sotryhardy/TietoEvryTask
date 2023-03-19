#pragma once
#include <string>
#include "LogSystem.h"
#include <atomic>

typedef std::atomic_uint uint;

class FileProcessing
{

public:
    FileProcessing(LogSystem* logSystem = nullptr);

    void StartProcessing(std::string pattern, std::string path);
private:

    uint _searched = 0;
    uint _withPattern = 0;
    uint _patterns = 0;

    LogSystem* _logSystem;

};