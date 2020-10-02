#include <iostream>
#include "./config.h"
#include "./src/utils/log.h"
#include "./src/index.cpp"

int main(int argc, char const *argv[])
{
    LOG_I("%s@%s", PROJECT_NAME, PROJECT_VERSION);
    index();
    return 0;
}