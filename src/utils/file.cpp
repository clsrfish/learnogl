#include "./file.h"
#include <fstream>
#include <sstream>
#include "../utils/log.h"

std::string readFileStr(const std::string &filePath)
{
    if (filePath == "")
    {
        return "";
    }

    std::ifstream ifs;
    // set which erro should be thrown
    ifs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        ifs.open(filePath);
        std::stringstream ss;
        ss << ifs.rdbuf();
        ifs.close();
        return ss.str();
    }
    catch (const std::exception &e)
    {
        LOG_E("Failed to read file: %s, %s", filePath.c_str(), e.what());
    }
    return "";
}