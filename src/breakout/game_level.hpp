#if !defined(GAM_LEVEL_H)
#define GAM_LEVEL_H

#include <iostream>
#include <string>
#include <vector>
#include "./game_object.hpp"

namespace breakout
{

    class GameLevel
    {
    public:
        std::vector<GameObject> Bricks;
        GameLevel();
        ~GameLevel();

        void Load(const std::string &file, unsigned int levelWidth, unsigned int levelHeight);

        void Draw(SpriteRenderer &renderer);

        bool IsCompleted();

    private:
        void init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);
    };

} // namespace breakout

#endif // GAM_LEVEL_H
