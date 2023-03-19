#include "FileProcessing.h"
#include <fstream>
#include <iostream>


FileProcessing::FileProcessing(LogSystem* logSystem)
{
    _logSystem = logSystem;
}

void FileProcessing::StartProcessing(std::string pattern, std::string path)
{
    std::ifstream  file;

    file.open(path, std::ios::in);
    
    if (!file) {
        return;
    }

    std::string lineContent;
    bool this_same_file = false;

    for (int line = 1; file; line++)
    {
        std::getline(file, lineContent);
        if (lineContent.find(pattern) != std::string::npos)
        {
            if (!this_same_file)
                _withPattern++;

            if (_logSystem)
            {
                _logSystem->NewLog(
                    std::this_thread::get_id(),
                    path,          //extract filename from path path.substr(path.find_last_of("/\\") + 1)
                    line,
                    std::move(lineContent));
            }
            _patterns++;
        }
    }
    _searched++;
}