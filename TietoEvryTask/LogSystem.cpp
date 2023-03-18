#include "LogSystem.h"
#include <iostream>

void LogSystem::NewLog(std::thread::id threadId, std::string fileName, int lineNumber, std::string content)
{
    std::cout << "NewLog" << std::endl;
}