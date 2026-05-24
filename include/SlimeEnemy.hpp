#ifndef SLIME_ENEMY_HPP
#define SLIME_ENEMY_HPP

#include "Enemy.hpp"

class SlimeEnemy : public Enemy {
public:
    SlimeEnemy(const std::string &fontPath, const glm::vec2 &spawnPosition,
               float difficultyScale);
};

#endif
