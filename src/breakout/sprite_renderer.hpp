#if !defined(SPRITE_RENDER_H)
#define SPRITE_RENDER_H

#include <glm/glm.hpp>
#include "./shader.hpp"
#include "./texture.hpp"

namespace breakout
{
    class SpriteRenderer
    {
    public:
        SpriteRenderer(Shader &shader);
        ~SpriteRenderer();

        void DrawSprite(Texture2D &texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));

    private:
        Shader shader;
        unsigned int quadVAO;
        void initRenderData();
    };
} // namespace breakout

#endif // SPRITE_RENDER_H
