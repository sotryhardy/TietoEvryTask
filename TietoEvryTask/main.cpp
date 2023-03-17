#include <string>
#include <iostream>
#include <filesystem>
#include <regex>

namespace fs = std::filesystem;


int main(int argc, char* argv[])
{
    std::string start_directory = ".";

    std::regex regular("(.+)" "(\\.)" "(.+)"); /* ".*" "[^\]$");*/

    //std::regex regular("([\\w-]+)" "(\\.)" "([\\w-]+)");
    std::cmatch result;

    std::string str ("email@host.ry");

    for (const auto& entry : std::filesystem::recursive_directory_iterator(start_directory))
        if (std::regex_match(entry.path().string().c_str(), result, regular))
        {
            std::cout << entry.path().string().substr(entry.path().string().find_last_of("/\\") + 1) << std::endl;
        }
    system("PAUSE");
    return 0;
}