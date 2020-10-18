#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "./game.hpp"
#include "./resource_manager.hpp"
#include "../utils/log.h"
#include "../gl/gl_utils.h"

const glm::vec2 PLAYER_SIZE = glm::vec2(100.0f, 25.0f);
const float PLAYER_VELOCITY = 500.0f;
const float BALL_RADIUS = 12.5f;
const glm::vec2 BALL_INITIAL_VELOCITY(100.0f, -350.0f);

breakout::Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{
}

breakout::Game::~Game()
{
}

void breakout::Game::Init()
{
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    // load shaders
    GLClearError();
    breakout::Shader spriteShader = ResourceManager::LoadShader("shaders/breakout/sprite.vs", "shaders/breakout/sprite.fs", "", "sprite");
    breakout::Shader particleShader = ResourceManager::LoadShader("shaders/breakout/particle.vs", "shaders/breakout/particle.fs", "", "particle");
    breakout::Shader postProcessShader = ResourceManager::LoadShader("shaders/breakout/postprocess.vs", "shaders/breakout/postprocess.fs", "", "postprocess");

    spriteShader.SetInteger("image", 0, true);
    spriteShader.SetMatrix4("projection", projection);
    particleShader.SetInteger("sprite", 0, true);
    particleShader.SetMatrix4("projection", projection);
    GLClearError();

    this->renderer = new breakout::SpriteRenderer(spriteShader);

    // load textures
    ResourceManager::LoadTexture("assets/background.jpg", false, "background");
    ResourceManager::LoadTexture("assets/awesomeface.png", true, "ball");
    ResourceManager::LoadTexture("assets/block.png", true, "block");
    ResourceManager::LoadTexture("assets/block_solid.png", true, "block_solid");
    ResourceManager::LoadTexture("assets/paddle.png", true, "paddle");
    auto particleTex = ResourceManager::LoadTexture("assets/particle.png", true, "particle");
    // load levels
    GameLevel one;
    one.Load("assets/level/one.lvl", this->Width, this->Height / 2);
    GameLevel two;
    two.Load("assets/level/two.lvl", this->Width, this->Height / 2);
    GameLevel three;
    three.Load("assets/level/three.lvl", this->Width, this->Height / 2);
    GameLevel four;
    four.Load("assets/level/four.lvl", this->Width, this->Height / 2);
    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Levels.push_back(three);
    this->Levels.push_back(four);
    this->Level = 1;

    // init player
    LOG_I("Initializing the player");
    GLClearError();
    this->player = new breakout::GameObject(
        glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y),
        PLAYER_SIZE, ResourceManager::GetTexture("paddle"));

    // init ball
    LOG_I("Initializing the ball");
    GLClearError();
    this->ball = new BallObject(
        this->player->Position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f),
        BALL_RADIUS, BALL_INITIAL_VELOCITY, ResourceManager::GetTexture("ball"));
    LOG_I("Initializing the ball particles");
    GLClearError();
    this->ballParicles = new ParticleEmmiter(particleShader, particleTex, 500);

    // init post processor
    this->postProcessor = new PostProcessor(this->Width, this->Height, postProcessShader);
}

void breakout::Game::Update(float dt)
{
    this->ball->Move(dt, this->Width);
    this->ballParicles->Update(dt, *(this->ball), 10, glm::vec2(this->ball->Radius));
    doCollisions();
    if (this->ball->Position.y >= this->Height + 100.0f)
    {
        this->ResetLevel();
        this->ResetPlayer();
    }
    if (glfwGetTime() - this->startShakingTime >= 0.05f)
    {
        this->postProcessor->Shake = false;
    }
}

void breakout::Game::ProcessInput(float dt)
{
    if (this->State == GAME_ACTIVE)
    {
        float velocity = PLAYER_VELOCITY * dt;
        auto player = this->player;
        auto ball = this->ball;
        if (this->Keys[GLFW_KEY_A])
        {
            float actualVelocity = std::min(velocity, (*player).Position.x);
            (*player).Position.x -= actualVelocity;
            if (ball->Stuck)
            {
                (*ball).Position.x -= actualVelocity;
            }
        }
        if (this->Keys[GLFW_KEY_D])
        {
            float actualVelocity = std::min(velocity, this->Width - (*player).Position.x - (*player).Size.x);
            (*player).Position.x += actualVelocity;
            if (ball->Stuck)
            {
                (*ball).Position.x += actualVelocity;
            }
        }
        if (this->Keys[GLFW_KEY_SPACE])
        {
            if (this->ball->Stuck)
            {
                LOG_I("Fire the hole!");
            }
            this->ball->Stuck = false;
        }
    }
}

void breakout::Game::Render()
{
    if (this->State == GAME_ACTIVE)
    {
        this->postProcessor->BeginRender();
        // draw background
        auto background = ResourceManager::GetTexture("background");
        this->renderer->DrawSprite(background, glm::vec2(0.0f), glm::vec2(this->Width, this->Height), 0.0f);
        // draw level
        this->Levels[this->Level].Draw(*(this->renderer));
        // draw player
        this->player->Draw(*(this->renderer));
        // draw ball
        this->ballParicles->Draw();
        this->ball->Draw(*(this->renderer));
        this->postProcessor->EndRender();
        this->postProcessor->Render(glfwGetTime());
    }
}

breakout::Collision breakout::Game::checkCollision(breakout::BallObject &cirlcle, breakout::GameObject &aabb)
{
    // get center point of cirlcle
    glm::vec2 circleCenter = cirlcle.Position + cirlcle.Radius;
    // calculate AABB info
    glm::vec2 aabbHalfExtends = aabb.Size / 2.0f;
    glm::vec2 aabbCenter = aabb.Position + aabbHalfExtends;
    // get difference vector between both centers
    glm::vec2 difference = circleCenter - aabbCenter;
    glm::vec2 clamped = glm::clamp(difference, -aabbHalfExtends, aabbHalfExtends);
    // add clamped value to AABB center, and we get the value of box closest to circle
    glm::vec2 closest = aabbCenter + clamped;
    difference = closest - circleCenter;
    if (glm::length(difference) <= cirlcle.Radius)
    {
        return std::make_tuple(true, vectorDirection(difference), difference);
    }
    else
    {
        return std::make_tuple(false, UP, glm::vec2(0.0f));
    }
}

void breakout::Game::doCollisions()
{
    auto ball = this->ball;

    // collision between ball and bricks
    for (breakout::GameObject &obj : this->Levels[this->Level].Bricks)
    {
        if (obj.IsDestroyed)
        {
            continue;
        }

        breakout::Collision collision = checkCollision(*(this->ball), obj);
        if (!std::get<0>(collision))
        {
            continue;
        }
        if (!obj.IsSolid)
        {
            obj.IsDestroyed = true;
        }
        else
        {
            this->postProcessor->Shake = true;
            this->startShakingTime = glfwGetTime();
        }
        breakout::Direction dir = std::get<1>(collision);
        glm::vec2 diffVector = std::get<2>(collision);
        if (dir == LEFT || dir == RIGHT)
        {
            (*ball).Velocity.x = -(*ball).Velocity.x;
            // reposition
            float penetration = ball->Radius - std::abs(diffVector.x);
            ball->Position.x += (dir == LEFT ? 1 : -1) * penetration;
        }
        else
        {
            (*ball).Velocity.y = -(*ball).Velocity.y;
            // reposition
            float penetration = ball->Radius - std::abs(diffVector.y);
            ball->Position.y += (dir == DOWN ? 1 : -1) * penetration;
        }
    }

    // collision between ball and paddle
    Collision collision = checkCollision(*ball, *(this->player));
    if (!(*ball).Stuck && std::get<0>(collision) && std::get<1>(collision) == UP) // only top edge collision allowed
    {
        auto paddle = this->player;
        float paddleCenter = (*paddle).Position.x + (*paddle).Size.x / 2.0f;
        float distance = std::abs((ball->Position.x + ball->Radius) - paddleCenter);
        float percentage = distance / (paddle->Size.x / 2.0f);
        // move accordingly
        float strength = 2.0f;
        glm::vec2 oldVelocity = ball->Velocity;
        ball->Velocity.x = BALL_INITIAL_VELOCITY.x * percentage * strength * (oldVelocity.x / std::abs(oldVelocity.x));
        ball->Velocity.y = -std::abs(ball->Velocity.y);
        ball->Velocity = glm::normalize(ball->Velocity) * glm::length(oldVelocity);
    }
}

breakout::Direction breakout::Game::vectorDirection(glm::vec2 target)
{
    // ball's moveing direction
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),  // up
        glm::vec2(1.0f, 0.0f),  // right
        glm::vec2(0.0f, -1.0f), // down
        glm::vec2(-1.0f, 0.0f)  // left
    };
    float max = 0.0f;
    unsigned int bestMatch = -1;
    for (unsigned int i = 0; i < 4; i++)
    {
        float dotProduct = glm::dot(glm::normalize(target), compass[i]);
        if (dotProduct > max)
        {
            max = dotProduct;
            bestMatch = i;
        }
    }
    return static_cast<Direction>(bestMatch);
}

void breakout::Game::ResetLevel()
{
    if (this->Level == 0)
    {
        this->Levels[0].Load("assets/level/one.lvl", this->Width, this->Height / 2);
    }
    else if (this->Level == 1)
    {
        this->Levels[1].Load("assets/level/two.lvl", this->Width, this->Height / 2);
    }
    else if (this->Level == 2)
    {
        this->Levels[2].Load("assets/level/three.lvl", this->Width, this->Height / 2);
    }
    else if (this->Level == 3)
    {
        this->Levels[3].Load("assets/level/four.lvl", this->Width, this->Height / 2);
    }
}

void breakout::Game::ResetPlayer()
{
    this->player->Position = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);

    this->ball->Position = this->player->Position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
    this->ball->Velocity = BALL_INITIAL_VELOCITY;
    this->ball->Stuck = true;
}