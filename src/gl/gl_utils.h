#if !defined(GL_UTILS_H)
#define GL_UTILS_H

#include <iostream>
#include <vector>
#include <string>

void printGLInfo();

unsigned int TextureFromFile(const std::string &file, const std::string &directory);

unsigned int TextureFromFile(const std::string &filepath);

unsigned int LoadCubeMapTexture(std::vector<std::string> faces);

#endif // GL_UTILS_H
