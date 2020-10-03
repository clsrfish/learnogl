#include <glad/glad.h>
#include <iostream>
#include "../utils/log.h"

#if !defined(SHADER_H)
#define SHADER_H

class Shader
{
public:
    Shader(const std::string &vsPath, const std::string &fsPath);
    ~Shader();
    Shader(const Shader &) = delete;
    Shader &operator=(const Shader &) = delete;

    bool Use() const;
    void SetBool(const std::string &name, bool value) const;
    void SetInt(const std::string &name, int value) const;
    void SetFloat(const std::string &name, float value) const;

private:
    unsigned int program;

    unsigned int CreateShader(GLenum shaderType, const std::string &shaderSource) const;
    int GetUniformLocation(const std::string &name) const;
};

#endif // SHADER_H