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

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filepath.c_str(), &width, &height, &nrComponents, 0);
    if (data != nullptr)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        LOG_E("Texture failed to load at path: %s", filepath.c_str());
    }
    stbi_image_free(data);

    return textureID;
}