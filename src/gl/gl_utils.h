#if !defined(GL_UTILS_H)
#define GL_UTILS_H

#include <iostream>
#include <vector>
#include <string>

void printGLInfo();

unsigned int TextureFromFile(const std::string &file, const std::string &directory);

unsigned int TextureFromFile(const std::string &filepath);

unsigned int LoadCubeMapTexture(std::vector<std::string> faces);

std::string GLError2String(unsigned int error);

class GLChecker
{
public:
    GLChecker(const std::string &func, const std::string &filepath, unsigned int line);
    ~GLChecker();

private:
    std::string glFunc;
    std::string sourceFile;
    unsigned int sourceLine;
};

#define GL_CHECK(glFunc)                              \
    [&]() {                                           \
        GLChecker check(#glFunc, __FILE__, __LINE__); \
        return glFunc;                                \
    }()
#endif // GL_UTILS_H
