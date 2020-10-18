#include "./shader.hpp"
#include <glad/glad.h>
#include "../utils/log.h"
#include "../gl/gl_utils.h"

breakout::Shader::Shader()
{
}

breakout::Shader::~Shader()
{
}

breakout::Shader &breakout::Shader::Use()
{
    GL_CHECK(glUseProgram(this->ID));
    return *this;
}
void breakout::Shader::Compile(std::string &vertexSource, const std::string &fragmentSource, const std::string &geometrySource)
{
#if defined(_DEBUG)
    this->vSource = vertexSource;
    this->fSource = fragmentSource;
    this->gSource = geometrySource;
#endif
    unsigned int vShader, fShader, gShader;
    const char *src = nullptr;
    vShader = GL_CHECK(glCreateShader(GL_VERTEX_SHADER));
    GL_CHECK(glShaderSource(vShader, 1, &(src = vertexSource.c_str()), nullptr));
    GL_CHECK(glCompileShader(vShader));
    checkCompileError(vShader, "VERTEX");

    fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &(src = fragmentSource.c_str()), nullptr);
    glCompileShader(fShader);
    checkCompileError(fShader, "FRAGMENT");

    if (geometrySource != "")
    {
        gShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(gShader, 1, &(src = geometrySource.c_str()), nullptr);
        glCompileShader(gShader);
        checkCompileError(gShader, "GEOMETRY");
    }

    this->ID = glCreateProgram();
    GL_CHECK(glAttachShader(this->ID, vShader));
    GL_CHECK(glAttachShader(this->ID, fShader));
    if (geometrySource != "")
    {
        glAttachShader(this->ID, gShader);
    }
    GL_CHECK(glLinkProgram(this->ID));
    checkCompileError(this->ID, "PROGRAM");

    glDeleteShader(vShader);
    glDeleteShader(fShader);
    if (geometrySource != "")
        glDeleteShader(gShader);
}

// utility functions
void breakout::Shader::SetFloat(const std::string &name, float value, bool useShader)
{
    if (useShader)
    {
        this->Use();
    }
    glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value);
}

void breakout::Shader::SetInteger(const std::string &name, int value, bool useShader)
{
    if (useShader)
    {
        this->Use();
    }
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
}

void breakout::Shader::SetVector2f(const std::string &name, float x, float y, bool useShader)
{
    if (useShader)
    {
        this->Use();
    }
    glUniform2f(glGetUniformLocation(this->ID, name.c_str()), x, y);
}

void breakout::Shader::SetVector2f(const std::string &name, const glm::vec2 &value, bool useShader)
{
    if (useShader)
    {
        this->Use();
    }
    glUniform2f(glGetUniformLocation(this->ID, name.c_str()), value.x, value.y);
}

void breakout::Shader::SetVector3f(const std::string &name, float x, float y, float z, bool useShader)
{
    if (useShader)
    {
        this->Use();
    }
    glUniform3f(glGetUniformLocation(this->ID, name.c_str()), x, y, z);
}

void breakout::Shader::SetVector3f(const std::string &name, const glm::vec3 &value, bool useShader)
{
    if (useShader)
    {
        this->Use();
    }
    glUniform3f(glGetUniformLocation(this->ID, name.c_str()), value.x, value.y, value.z);
}

void breakout::Shader::SetVector4f(const std::string &name, float x, float y, float z, float w, bool useShader)
{
    if (useShader)
    {
        this->Use();
    }
    glUniform4f(glGetUniformLocation(this->ID, name.c_str()), x, y, z, w);
}

void breakout::Shader::SetVector4f(const std::string &name, const glm::vec4 &value, bool useShader)
{
    if (useShader)
    {
        this->Use();
    }
    glUniform4f(glGetUniformLocation(this->ID, name.c_str()), value.x, value.y, value.z, value.w);
}

void breakout::Shader::SetMatrix4(const std::string &name, const glm::mat4 &matrix, bool useShader)
{
    if (useShader)
    {
        this->Use();
    }
    glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, false, glm::value_ptr(matrix));
}

void breakout::Shader::checkCompileError(unsigned int object, const std::string &type)
{
    int status;
    int infoLength;
    char *logBuffer = nullptr;
    if (type != "PROGRAM")
    {
        glGetShaderiv(object, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE)
        {
            glGetShaderiv(object, GL_INFO_LOG_LENGTH, &infoLength);
            logBuffer = new char[infoLength];
            glGetShaderInfoLog(object, infoLength, &infoLength, logBuffer);
            LOG_E("SHADER: Compile-time error: Type: %s %s", type.c_str(), logBuffer);
        }
    }
    else
    {
        glGetProgramiv(object, GL_LINK_STATUS, &status);
        if (status == GL_FALSE)
        {
            glGetProgramiv(object, GL_INFO_LOG_LENGTH, &infoLength);
            logBuffer = new char[infoLength];
            glGetProgramInfoLog(object, infoLength, &infoLength, logBuffer);
            LOG_E("Shader: Link-time error: Type: %s %s", type.c_str(), logBuffer);
        }
    }
}