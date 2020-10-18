#if !defined(GAME_OBJECT_H)
#define GAME_OBJECT_H
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include "./texture.hpp"
#include "./sprite_renderer.hpp"

namespace breakout
{

    class GameObject
    {
    private:
        /* data */
    public:
        glm::vec2 Position, Size, Velocity;
        glm::vec3 Color;
        float Rotation;
        bool IsSolid;
        bool IsDestroyed;

        Texture2D Sprite;

        GameObject();
        GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f));

        ~GameObject();

        virtual void Draw(SpriteRenderer &renderer);
    };

} // namespace breakout

#endif // GAME_OBJECT_H
