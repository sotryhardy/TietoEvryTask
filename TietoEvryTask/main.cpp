#include <string>
#include <iostream>
#include <filesystem>
#include <regex>
#include "FileProcessing.h"

namespace fs = std::filesystem;


int main(int argc, char* argv[])
{
    std::string start_directory = ".";

    std::regex regular(".*" "\\." ".*"); /* ".*" "[^\]$");*/

    //std::regex regular("([\\w-]+)" "(\\.)" "([\\w-]+)");
    //.substr(entry.path().string().find_last_of("/\\") + 1)
    std::cmatch result;

    std::string str ("email@host.ry");

    FileProcessing fileProcessing;

    for (const auto& entry : std::filesystem::recursive_directory_iterator(start_directory))
        //if (std::regex_match(entry.path().string().c_str(), result, regular))
        //{
          //  std::cout << entry.path().string() << std::endl;

            fileProcessing.StartProcessing("#include <map>", entry.path().string());
        //}
    system("PAUSE");
    return 0;
}