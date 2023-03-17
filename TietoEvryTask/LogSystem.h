#pragma once
#include <map>
#include <string>
#include <thread>
#include "FileContent.h"
#include <mutex>

class LogSystem
{
public: 

    LogSystem(std::string resultFileName, std::string logFileName);

    std::map<std::thread::id, std::map<std::string, FileContent>> logs;

    void NewLog(std::thread::id threadId, std::string fileName, int lineNumber, std::string content);

    void MakeLogFile() const;
    void MakeResultFile() const;

private:
    std::string _resultFileName;
    std::string _logFileName;
    std::mutex _mutex;
};