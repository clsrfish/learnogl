#include "./resource_manager.hpp"
#include <stb_image.h>
#include "../utils/log.h"
#include "../utils/file.h"

std::map<std::string, breakout::Texture2D> breakout::ResourceManager::Textures;
std::map<std::string, breakout::Shader> breakout::ResourceManager::Shaders;

breakout::ResourceManager::ResourceManager(/* args */)
{
}

breakout::ResourceManager::~ResourceManager()
{
}

breakout::Shader breakout::ResourceManager::LoadShader(const std::string &vShaderFile, const std::string &fShaderFile, const std::string &gShaderFile, const std::string &name)
{
    LOG_I("Loading shader: %s", name.c_str());
    Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return Shaders[name];
}

breakout::Shader breakout::ResourceManager::GetShader(const std::string &name)
{
    return Shaders[name];
}

breakout::Texture2D breakout::ResourceManager::LoadTexture(const std::string &file, bool alpha, const std::string &name)
{
    LOG_I("Loading texture: %s", name.c_str());
    Textures[name] = loadTextureFromFile(file, alpha);
    return Textures[name];
}

breakout::Texture2D breakout::ResourceManager::GetTexture(const std::string &name)
{
    return Textures[name];
}

void breakout::ResourceManager::Clear()
{
    for (auto iter : Shaders)
    {
        glDeleteShader(iter.second.ID);
    }
    for (auto iter : Textures)
    {
        glDeleteTextures(1, &iter.second.ID);
    }
}

breakout::Shader breakout::ResourceManager::loadShaderFromFile(const std::string &vShaderFile, const std::string &fShaderFile, const std::string &gShaderFile)
{
    breakout::Shader shader;
    std::string vShaderSource = readFileStr(vShaderFile);
    std::string fShaderSource = readFileStr(fShaderFile);
    std::string gShaderSource = readFileStr(gShaderFile);
    shader.Compile(vShaderSource, fShaderSource, gShaderSource);
    return shader;
}

breakout::Texture2D breakout::ResourceManager::loadTextureFromFile(const std::string &file, bool alpha)
{
    // create texture object
    Texture2D texture;
    // load image
    int width, height, nrChannels;
    unsigned char *data = stbi_load(file.c_str(), &width, &height, &nrChannels, 0);
    if (nrChannels == 1)
    {
        texture.InternalFormat = GL_RED;
        texture.ImageFormat = GL_RED;
    }
    else if (nrChannels == 3)
    {
        texture.InternalFormat = GL_RGB;
        texture.ImageFormat = GL_RGB;
    }
    else if (nrChannels == 4)
    {
        texture.InternalFormat = GL_RGBA;
        texture.ImageFormat = GL_RGBA;
    }

    // now generate texture
    texture.Generate(width, height, data);
    // and finally free image data
    stbi_image_free(data);
    return texture;
}