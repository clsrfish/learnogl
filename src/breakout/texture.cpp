#include <glad/glad.h>
#include "./texture.hpp"
#include "../gl/gl_utils.h"

breakout::Texture2D::Texture2D()
    : Width(0), Height(0), InternalFormat(GL_RGB), ImageFormat(GL_RGB), WrapS(GL_REPEAT), WrapT(GL_REPEAT), FilterMin(GL_LINEAR), FilterMag(GL_LINEAR)
{
    glGenTextures(1, &this->ID);
}
breakout::Texture2D::~Texture2D()
{
}

void breakout::Texture2D::Generate(unsigned int width, unsigned int height, unsigned char *data)
{
    this->Width = width;
    this->Height = height;

    glBindTexture(GL_TEXTURE_2D, this->ID);
    glTexImage2D(GL_TEXTURE_2D, 0, this->InternalFormat, width, height, 0, this->ImageFormat, GL_UNSIGNED_BYTE, data);
    // set Texture wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->WrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->WrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->FilterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->FilterMag);
    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void breakout::Texture2D::Bind() const
{
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, this->ID));
}