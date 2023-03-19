#include "LogSystem.h"
#include "FileContent.h"
#include <iostream>
#include <fstream>

LogSystem::LogSystem(std::string resultFileName, std::string logFileName)
{
    _resultFileName = resultFileName;
    _logFileName = logFileName;
}

void LogSystem::NewLog(std::thread::id threadId, std::string fileName, int lineNumber, std::string content)
{
    std::lock_guard<std::mutex> lock(_mutex);
    logs[threadId][fileName].lineContent.emplace_back(std::make_pair(lineNumber, std::move(content)));
}

void LogSystem::MakeLogFile() const
{
    std::ofstream logFile(_logFileName);

    for (const auto& id : logs)
    {
        logFile << id.first << ": ";
        for (const auto& file : id.second)
        {
            logFile << file.first.substr(file.first.find_last_of("/\\") + 1) << ", ";
        }
        logFile << '\n';
    }
}

void LogSystem::MakeResultFile() const
{
    std::ofstream resultFile(_resultFileName);

    for (const auto& id : logs)
    {
        for (const auto& file : id.second)
        {
            for (const auto& line : file.second.lineContent)
            {
                resultFile << file.first << ":" << line.first << ": " << line.second << "\n";
            }
        }
    }

}