#include "./gl_utils.h"
#include "../utils/log.h"
#include <glad/glad.h>
#include <stb_image.h>

void printGLInfo()
{
    LOG_I("GLVendor:   %s", glGetString(GL_VENDOR));
    LOG_I("GLVersion:  %s", glGetString(GL_VERSION));
    LOG_I("GLRenderer: %s", glGetString(GL_RENDERER));
}

unsigned int TextureFromFile(const std::string &file, const std::string &directory)
{

    std::stringstream ss;
    ss << directory << '/' << file;
    std::string filepath = ss.str();

    return TextureFromFile(filepath);
}

unsigned int TextureFromFile(const std::string &filepath)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filepath.c_str(), &width, &height, &nrComponents, 0);
    if (data != nullptr)
    {
        GLenum format;
        if (nrComponents == 1)
        {
            LOG_I("Tex format: RED - %s", filepath.c_str());
            format = GL_RED;
        }
        else if (nrComponents == 3)
        {
            LOG_I("Tex format: RGB - %s", filepath.c_str());
            format = GL_RGB;
        }
        else if (nrComponents == 4)
        {
            LOG_I("Tex format: RGBA - %s", filepath.c_str());
            format = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else
    {
        LOG_E("Texture failed to load at path: %s", filepath.c_str());
    }
    stbi_image_free(data);

    return textureID;
}

unsigned int LoadCubeMapTexture(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    int width, height, channels;
    for (unsigned int i = 0; i < faces.size(); ++i)
    {
        auto data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);
        if (data != nullptr)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            LOG_E("Failed to load cube map texture: %s", faces[i].c_str());
            glDeleteTextures(1, &textureID);
            break;
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    return textureID;
}