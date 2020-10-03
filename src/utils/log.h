#include <iostream>
#include <iomanip>
#include <sstream>

#if !defined(LOG_H)
#define LOG_H

// https://stackoverflow.com/questions/9158150/colored-output-in-c/9158263
//the following are UBUNTU/LINUX, and MacOS ONLY terminal color codes.
#define RESET "\033[0m"
#define BLACK "\033[30m"              /* Black */
#define RED "\033[31m"                /* Red */
#define GREEN "\033[32m"              /* Green */
#define YELLOW "\033[33m"             /* Yellow */
#define BLUE "\033[34m"               /* Blue */
#define MAGENTA "\033[35m"            /* Magenta */
#define CYAN "\033[36m"               /* Cyan */
#define WHITE "\033[37m"              /* White */
#define BOLDBLACK "\033[1m\033[30m"   /* Bold Black */
#define BOLDRED "\033[1m\033[31m"     /* Bold Red */
#define BOLDGREEN "\033[1m\033[32m"   /* Bold Green */
#define BOLDYELLOW "\033[1m\033[33m"  /* Bold Yellow */
#define BOLDBLUE "\033[1m\033[34m"    /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m" /* Bold Magenta */
#define BOLDCYAN "\033[1m\033[36m"    /* Bold Cyan */
#define BOLDWHITE "\033[1m\033[37m"   /* Bold White */

#ifdef Windows
// Windows下文件目录层级是'\\'
#define __FILENAME__ (strrchr(__FILE__, '\\') + 1)
#elif Linux
// Linux下文件目录层级是'/'
#define __FILENAME__ (strrchr(__FILE__, '/') + 1)
#else
#define __FILENAME__ (strrchr(__FILE__, '/') + 1)
#endif

// https://stackoverflow.com/questions/1639797/template-issue-causes-linker-error-c
// https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
template <typename... Args>
std::string string_format(const std::string &format, Args... args)
{
    size_t size = snprintf(nullptr, 0, format.c_str(), args...) + 1; // Extra space for '\0'
    if (size <= 0)
    {
        throw std::runtime_error("Error during formatting.");
    }
    std::unique_ptr<char[]> buf(new char[size]);
    snprintf(buf.get(), size, format.c_str(), args...);
    return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}
#define WIDTH 4

#define LOG_I(FORMAT, ...) \
    std::cout << BOLDGREEN << std::left << std::setw(WIDTH) << std::setfill(' ') << "[I]" << RESET << GREEN << string_format(FORMAT, ##__VA_ARGS__) << RESET << std::endl;

#define LOG_D(FORMAT, ...) \
    std::cout << BOLDBLUE << std::left << std::setw(WIDTH) << std::setfill(' ') << "[D]" << RESET << BLUE << string_format(FORMAT, ##__VA_ARGS__) << RESET << std::endl;

#define LOG_W(FORMAT, ...) \
    std::cout << BOLDYELLOW << std::left << std::setw(WIDTH) << std::setfill(' ') << "[W]" << RESET << YELLOW << string_format(FORMAT, ##__VA_ARGS__) << RESET << std::endl;

#define LOG_E(FORMAT, ...) \
    std::cerr << BOLDRED << std::left << std::setw(WIDTH) << std::setfill(' ') << "[E]" << RESET << RED << string_format(FORMAT, ##__VA_ARGS__) << RESET << std::endl;

#endif // LOG_H