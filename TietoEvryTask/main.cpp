#include <string>
#include <iostream>
#include <filesystem>
#include <regex>
#include "FileProcessing.h"
#include "TaskSystem.h"

namespace fs = std::filesystem;


int main(int argc, char* argv[])
{

    if (argc == 1)
    {
        std::cout << "The program searches for <pattern> in the files in the given directory" << std::endl;
        std::cout << "-d or --dir: start directory where program needs to look for files (also in subfolders). Default value = current directory" << std::endl;
        std::cout << "-l or --log_file: name of the log file. Default value = specific_grep.log" << std::endl;
        std::cout << "-r or --result_file: name of the file where result is given. Default value = specific_grep.txt" << std::endl;
        std::cout << "-t or --threads: number of threads in the pool. Default value = 4" << std::endl;
        return 0;
    }

    std::string start_directory = ".";
    std::string resultFileName = "specific_grep.txt";
    std::string logFileName = "specific_grep.log";
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
            if (!std::regex_match(argv[i + 1], regular) )
            {
                std::cout << "Wrong value for thread number";
                return -1;
            }
            threadsNumber = atoi(argv[++i]); // std::strtol(argv[++i],)
        }
    }

    pattern = argv[argc - 1];


    //std::regex regular(".*" "\\." ".*"); /* ".*" "[^\]$");*/

    //std::regex regular("([\\w-]+)" "(\\.)" "([\\w-]+)");
    //.substr(entry.path().string().find_last_of("/\\") + 1)
    std::cmatch result;



    LogSystem* logSystem = new LogSystem(resultFileName, logFileName);

    FileProcessing fileProcessing(logSystem);
    {
        TaskSystem taskSystem(threadsNumber);
        for (const auto& entry : std::filesystem::recursive_directory_iterator(start_directory))
        {
            //if (std::regex_match(entry.path().string().c_str(), result, regular))
            //{
              //  std::cout << entry.path().string() << std::endl;

            taskSystem.async_(
                [pattern, entry, &fileProcessing]()
                {
                    fileProcessing.StartProcessing(pattern, entry.path().string());
                });
        }
    }
    logSystem->MakeLogFile();
    logSystem->MakeResultFile();
    system("PAUSE");
    return 0;
}