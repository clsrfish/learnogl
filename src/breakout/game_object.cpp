#include "./game_object.hpp"

breakout::GameObject::GameObject()
    : Position(0.0f, 0.0f), Size(1.0f, 1.0f), Velocity(0.0f), Color(1.0f), Rotation(0.0f), Sprite(), IsSolid(false), IsDestroyed(false)
{
}

breakout::GameObject::GameObject(glm::vec2 pos, glm::vec2 size, breakout::Texture2D sprite, glm::vec3 color, glm::vec2 velocity)
    : Position(pos), Size(size), Sprite(sprite), Color(color), Velocity(velocity), IsSolid(false), IsDestroyed(false)
{
}

breakout::GameObject::~GameObject()
{
}

void breakout::GameObject::Draw(breakout::SpriteRenderer &renderer)
{
    renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
}