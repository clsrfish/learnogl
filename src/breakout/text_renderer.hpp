#if !defined(TEXT_RENDERER_H)
#define TEXT_RENDERER_H

#include <map>
#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace breakout
{
    struct Character
    {
        unsigned int TextureID;
        glm::ivec2 Size;
        glm::ivec2 Bearing;
        unsigned int Advance;
    };

    class TextRenderer
    {
    private:
        std::map<char, Character> characters;
        unsigned int textVAO, textVBO;

        void loadVertices();
        void loadCharacters();

    public:
        TextRenderer(/* args */);
        ~TextRenderer();
        void RenderText(const std::string &text, float x, float y, float scale, glm::vec3 color = glm::vec3(1.0f));
    };

} // namespace breakout

#endif // TEXT_RENDERER_H
