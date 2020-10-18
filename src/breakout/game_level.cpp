#include <sstream>
#include <fstream>
#include "./game_level.hpp"
#include "./resource_manager.hpp"
#include "../utils/log.h"
#include "../utils/file.h"

breakout::GameLevel::GameLevel(/* args */)
{
}

breakout::GameLevel::~GameLevel()
{
}

void breakout::GameLevel::Load(const std::string &file, unsigned int levelWidth, unsigned int levelHeight)
{
    LOG_I("Loading level: %s", file.c_str());
    this->Bricks.clear();
    unsigned int tileCode;
    GameLevel level;
    std::string line;
    std::ifstream fs(file.c_str());
    std::vector<std::vector<unsigned int>> tileData;
    if (fs)
    {
        while (std::getline(fs, line))
        {
            std::istringstream sstream(line);
            std::vector<unsigned int> row;
            while (sstream >> tileCode)
            {
                row.push_back(tileCode);
            }
            tileData.push_back(row);
        }
    }
    if (tileData.size() > 0)
    {
        init(tileData, levelWidth, levelHeight);
    }
}

void breakout::GameLevel::Draw(breakout::SpriteRenderer &renderer)
{

    for (auto iter : this->Bricks)
    {
        if (iter.IsDestroyed)
        {
            continue;
        }
        iter.Draw(renderer);
    }
}

bool breakout::GameLevel::IsCompleted()
{
    for (auto iter : this->Bricks)
    {
        if (!iter.IsSolid && !iter.IsDestroyed)
        {
            return false;
        }
    }

    return true;
}

void breakout::GameLevel::init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight)
{
    unsigned int height = tileData.size();
    unsigned int width = tileData[0].size();
    float unitWidth = levelWidth / static_cast<float>(width);
    float unitHeight = levelHeight / static_cast<float>(height);
    glm::vec2 size = glm::vec2(unitWidth, unitHeight);

    for (unsigned int y = 0; y < height; y++)
    {
        for (unsigned int x = 0; x < width; x++)
        {
            if (tileData[y][x] == 1)
            { // solid
                glm::vec2 pos = glm::vec2(unitWidth * x, unitHeight * y);
                breakout::GameObject obj(pos, size, breakout::ResourceManager::GetTexture("block_solid"), glm::vec3(0.8f, 0.8f, 0.7f));
                obj.IsSolid = true;
                this->Bricks.push_back(obj);
            }
            else if (tileData[y][x] > 1)
            {
                glm::vec3 color = glm::vec3(1.0f);
                if (tileData[y][x] == 2)
                {
                    color = glm::vec3(0.2f, 0.6f, 1.0f);
                }
                else if (tileData[y][x] == 3)
                {
                    color = glm::vec3(0.0f, 0.7f, 0.0f);
                }
                else if (tileData[y][x] == 4)
                {
                    color = glm::vec3(0.8f, 0.8f, 0.4f);
                }
                else if (tileData[y][x] == 5)
                {
                    color = glm::vec3(1.0f, 0.5f, 0.0f);
                }
                glm::vec2 pos = glm::vec2(unitWidth * x, unitHeight * y);
                breakout::GameObject obj(pos, size, breakout::ResourceManager::GetTexture("block"), color);
                this->Bricks.push_back(obj);
            }
        }
    }
}
