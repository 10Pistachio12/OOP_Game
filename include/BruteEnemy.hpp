#ifndef BRUTE_ENEMY_HPP
#define BRUTE_ENEMY_HPP

#include "Enemy.hpp"

class BruteEnemy : public Enemy {
public:
    BruteEnemy(const std::string &fontPath, const glm::vec2 &spawnPosition,
               float difficultyScale);
};

#endif
