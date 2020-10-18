#if !defined(GAME_H)
#define GAME_H

#include <iostream>
#include <vector>
#include <tuple>
#include "./sprite_renderer.hpp"
#include "./game_level.hpp"
#include "./ball_object.hpp"
// Represents the current state of the game

namespace breakout
{

    enum GameState
    {
        GAME_ACTIVE,
        GAME_MENU,
        GAME_WIN
    };

    enum Direction
    {
        UP,
        RIGHT,
        DOWN,
        LEFT
    };

    typedef std::tuple<bool, Direction, glm::vec2> Collision;

    class Game
    {
    public:
        // game state
        GameState State;

        bool Keys[1024];
        unsigned int Width, Height;

        // level
        std::vector<GameLevel> Levels;
        unsigned int Level;

        Game(unsigned int width, unsigned int height);
        ~Game();
        // initialize game state (load all shaders/textures/levels)
        void Init();
        // game loop
        void ProcessInput(float dt);
        void Update(float dt);
        void Render();

        void ResetLevel();
        void ResetPlayer();

    private:
        SpriteRenderer *renderer;
        GameObject *player;
        BallObject *ball;

        /** 
         * AABB-Circle collision detection
         */
        Collision checkCollision(BallObject &circle, GameObject &aabb);
        void doCollisions();
        Direction vectorDirection(glm::vec2 target);
    };

} // namespace breakout

#endif // GAME_H
