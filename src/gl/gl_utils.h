#if !defined(GL_UTILS_H)
#define GL_UTILS_H

#include <iostream>
#include <string>

void printGLInfo();

unsigned int TextureFromFile(const std::string &file, const std::string &directory);

#endif // GL_UTILS_H
