#ifndef BAT_ENEMY_HPP
#define BAT_ENEMY_HPP

#include "Enemy.hpp"

class BatEnemy : public Enemy {
public:
    BatEnemy(const std::string &fontPath, const glm::vec2 &spawnPosition,
             float difficultyScale);

    void Update(float deltaTimeSeconds, const glm::vec2 &targetPosition) override;

private:
    float m_DriftTimer = 0.0F;
};

#endif
