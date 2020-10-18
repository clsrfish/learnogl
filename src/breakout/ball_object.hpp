#if !defined(BALL_OBJECT_H)
#define BALL_OBJECT_H

#include "./game_object.hpp"

namespace breakout
{
    class BallObject : public GameObject
    {
    public:
        // ball state
        float Radius;
        bool Stuck;

        BallObject();
        BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite);
        ~BallObject();

        glm::vec2 Move(float dt, unsigned int windowWidth);
        void Reset(glm::vec2 pos, glm::vec2 velocity);

    private:
        /* data */
    };

} // namespace breakout

#endif // BALL_OBJECT_H
