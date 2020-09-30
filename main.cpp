#include "./src/hello.cpp"
#include "./config.h"
#include <iostream>

int main(int argc, char const *argv[])
{
    std::cout << PROJECT_NAME << "@" << PROJECT_VERSION << std::endl;
    hello();
    return 0;
}