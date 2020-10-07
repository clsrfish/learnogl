#include "./shader.h"
#include "../utils/file.h"

Shader::Shader(const std::string &vsPath, const std::string &fsPath)
    : program(0)
{

    std::string vs = readFileStr(vsPath);
    std::string fs = readFileStr(fsPath);
    auto vertexShader = CreateShader(GL_VERTEX_SHADER, vs);
    auto fragShader = CreateShader(GL_FRAGMENT_SHADER, fs);
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);
    int linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if (linkStatus == GL_FALSE)
    {
        int infoLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLength);
        char *logBuffer = new char[infoLength];
        glGetProgramInfoLog(program, infoLength, &infoLength, logBuffer);
        LOG_E("ERROR::PROGRAM::LINK_FAILED::\n%s", logBuffer);
        glDeleteProgram(program);
        program = 0;
    }
#ifndef _DEBUG
    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);
#endif
}

Shader::~Shader()
{
    glDeleteProgram(program);
}

unsigned int Shader::CreateShader(unsigned int shaderType, const std::string &source) const
{
    unsigned int fragShader = glCreateShader(shaderType);
    const int sourceLen = source.length();
    const char *src = source.c_str();
    glShaderSource(fragShader, 1, &src, &sourceLen);
    glCompileShader(fragShader);
    int compileStatus;
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus == GL_FALSE)
    {
        int infoLength;
        glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &infoLength);
        char *logBuffer = new char[infoLength];
        glGetShaderInfoLog(fragShader, infoLength, nullptr, logBuffer);
        LOG_E("ERROR::SHADER::%s::COMPILATION_FAILED::\n%s\nSOURCE::%s",
              (shaderType == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT"), logBuffer, src);
        glDeleteShader(fragShader);
        return GL_INVALID_VALUE;
    }
    return fragShader;
}

bool Shader::Use() const
{
    if (program != 0)
    {
        glUseProgram(program);
        return true;
    }
    LOG_E("Failed to use shader program");
    return false;
}

int Shader::GetUniformLocation(const std::string &name) const
{
    int currentProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    if (currentProgram != program)
    {
        return -1;
    }
    return glGetUniformLocation(program, name.c_str());
}

void Shader::SetBool(const std::string &name, bool value) const
{
    auto loc = GetUniformLocation(name);
    if (loc != -1)
    {
        glUniform1i(loc, (int)value);
    }
}

void Shader::SetInt(const std::string &name, int value) const
{
    auto loc = GetUniformLocation(name);
    if (loc != -1)
    {
        glUniform1i(loc, value);
    }
}

void Shader::SetFloat(const std::string &name, float value) const
{
    auto loc = GetUniformLocation(name);
    if (loc != -1)
    {
        glUniform1f(loc, value);
    }
}

void Shader::SetMatrix4(const std::string &name, float *matrix) const
{
    auto loc = GetUniformLocation(name);
    if (loc != -1)
    {
        glUniformMatrix4fv(loc, 1, GL_FALSE, matrix);
    }
}

void Shader::SetVec3(const std::string &name, float v0, float v1, float v2) const
{
    auto loc = GetUniformLocation(name);
    if (loc != -1)
    {
        glUniform3f(loc, v0, v1, v2);
    }
}