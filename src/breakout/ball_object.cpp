#include "./ball_object.hpp"

breakout::BallObject::BallObject()
    : GameObject(), Radius(12.0f), Stuck(true)
{
}

breakout::BallObject::BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite)
    : GameObject(pos, glm::vec2(radius * 2.0f), sprite, glm::vec3(1.0f), velocity), Radius(radius), Stuck(true)
{
}

breakout::BallObject::~BallObject()
{
}

glm::vec2 breakout::BallObject::Move(float dt, unsigned int windowWidth)
{

    if (!this->Stuck)
    {
        this->Position += this->Velocity * dt;
        if (this->Position.x <= 0.0f)
        {
            this->Position.x = 0.0f;
            this->Velocity.x = -this->Velocity.x;
        }
        else if (this->Position.x + this->Size.x >= windowWidth)
        {
            this->Position.x = windowWidth - this->Size.x;
            this->Velocity.x = -this->Velocity.x;
        }
        if (this->Position.y <= 0.0f)
        {
            this->Position.y = 0.0f;
            this->Velocity.y = -this->Velocity.y;
        }
    }
    return this->Position;
}