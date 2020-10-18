#if !defined(SHADER_H)
#define SHADER_H

#include <iostream>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace breakout
{
    class Shader
    {
    public:
        unsigned int ID;

        Shader(/* args */);
        ~Shader();

        Shader &Use();
        void Compile(std::string &vertexSource, const std::string &fragmentSource, const std::string &geometrySource = "");
        // utility functions
        void SetFloat(const std::string &name, float value, bool useShader = false);
        void SetInteger(const std::string &name, int value, bool useShader = false);
        void SetVector2f(const std::string &name, float x, float y, bool useShader = false);
        void SetVector2f(const std::string &name, const glm::vec2 &value, bool useShader = false);
        void SetVector3f(const std::string &name, float x, float y, float z, bool useShader = false);
        void SetVector3f(const std::string &name, const glm::vec3 &value, bool useShader = false);
        void SetVector4f(const std::string &name, float x, float y, float z, float w, bool useShader = false);
        void SetVector4f(const std::string &name, const glm::vec4 &value, bool useShader = false);
        void SetMatrix4(const std::string &name, const glm::mat4 &matrix, bool useShader = false);

    private:
        // check if compilation or linking failed and if so, print the error message
        void checkCompileError(unsigned int object, const std::string &type);
    };

} // namespace breakout

#endif // SHADER_H