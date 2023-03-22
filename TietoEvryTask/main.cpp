#include <string>
#include <iostream>
#include <filesystem>
#include <regex>
#include "FileProcessing.h"
#include "TaskSystem.h"

namespace fs = std::filesystem;


int main(int argc, char* argv[])
{
    const clock_t start = clock();
    if (argc == 1)
    {
        std::cout << "The program searches for <pattern> in the files in the given directory" << std::endl;
        std::cout << "-d or --dir: start directory where program needs to look for files (also in subfolders). Default value = current directory" << std::endl;
        std::cout << "-l or --log_file: name of the log file. Default value = specific_grep.log" << std::endl;
        std::cout << "-r or --result_file: name of the file where result is given. Default value = specific_grep.txt" << std::endl;
        std::cout << "-t or --threads: number of threads in the pool. Default value = 4" << std::endl;
        return 0;
    }

    if (argc % 2)
    {
        std::cout << "Wrong parametrs" << std::endl;
        return 0;
    }

    std::string start_directory = ".";
    std::string resultFileName(argv[0]);
    resultFileName += ".txt";
    std::string logFileName(argv[0]);
    logFileName += ".log";
    std::string pattern;
    int threadsNumber = 4;

    std::regex regular("[0-9]+");

    for (int i = 1; i < argc - 1; i++)
    {
        if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--dir") == 0)
        {
            start_directory = argv[++i];
            continue;
        }
        if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--log_file") == 0)
        {
            logFileName = argv[++i];
            continue;
        }
        if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--result_file") == 0)
        {
            resultFileName = argv[++i];
            continue;
        }
        if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--threads") == 0)
        {
            if (!std::regex_match(argv[i + 1], regular))
            {
                std::cout << "Wrong value for thread number";
                return -1;
            }
            threadsNumber = atoi(argv[++i]); 
        }
    }

    pattern = argv[argc - 1];
    LogSystem* logSystem = new LogSystem(resultFileName, logFileName);

    FileProcessing fileProcessing(logSystem);

    TaskSystem taskSystem(threadsNumber);
    taskSystem.Start();
    for (const auto& entry : std::filesystem::recursive_directory_iterator(start_directory))
    {
        taskSystem.async_(
            [pattern, entry, &fileProcessing]()
            {
                fileProcessing.StartProcessing(pattern, entry.path().string());
            });
        //fileProcessing.StartProcessing(pattern, entry.path().string());
    }

    while (taskSystem.busy()) {}

    taskSystem.Stop();

    logSystem->MakeLogFile();
    logSystem->MakeResultFile();

    Statictic stats = fileProcessing.GetStatistic();

    clock_t end = clock();
    clock_t delta = end - start;
    
    std::cout << "Searched files: " << stats.searched << std::endl;
    std::cout << "Files with pattern: " << stats.withPattern << std::endl;
    std::cout << "Patterns number: " << stats.patterns_number << std::endl;
    std::cout << "Result file: " << resultFileName << std::endl;
    std::cout << "Log File: " << logFileName << std::endl;
    std::cout << "Used threads: " << threadsNumber << std::endl;
    std::cout << "Elapsed time: " << static_cast<double>(delta) / CLOCKS_PER_SEC << std::endl;
    return 0;
}