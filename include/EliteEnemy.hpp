#ifndef ELITE_ENEMY_HPP
#define ELITE_ENEMY_HPP

#include "Enemy.hpp"

class EliteEnemy : public Enemy {
public:
    EliteEnemy(const std::string &fontPath, const glm::vec2 &spawnPosition,
               float difficultyScale);
};

#endif
