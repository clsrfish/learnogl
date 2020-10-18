#include <iostream>
#include <string>
#include <map>
#include "./shader.hpp"
#include "./texture.hpp"

namespace breakout
{

    class ResourceManager
    {
    private:
        /* data */
        ResourceManager(/* args */);
        ~ResourceManager();
        // loads and generates a shader from file
        static Shader loadShaderFromFile(const std::string &vShaderFile, const std::string &fShaderFile, const std::string &gShaderFile = nullptr);

        // loads a single texture from file
        static Texture2D loadTextureFromFile(const std::string &file, bool alpha);

    public:
        static std::map<std::string, Shader> Shaders;
        static std::map<std::string, Texture2D> Textures;
        // loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
        static Shader LoadShader(const std::string &vShaderFile, const std::string &fShaderFile, const std::string &gShaderFile, const std::string &name);
        // retrieve a stored shader
        static Shader GetShader(const std::string &name);

        // loads (and generates) a texture from file
        static Texture2D LoadTexture(const std::string &file, bool alpha, const std::string &name);
        // retrieve a stored texture
        static Texture2D GetTexture(const std::string &name);
        // properly de-allocate the all loaded resources
        static void Clear();
    };

} // namespace breakout
