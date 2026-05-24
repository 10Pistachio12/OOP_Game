#include "SlimeEnemy.hpp"

SlimeEnemy::SlimeEnemy(const std::string &fontPath,
                       const glm::vec2 &spawnPosition, float difficultyScale)
    : Enemy(fontPath, "s", 28, Util::Color(100, 225, 120), spawnPosition,
            17.0F, 85.0F + difficultyScale * 16.0F,
            3 + static_cast<int>(difficultyScale * 0.8F), 1,
            1 + static_cast<int>(difficultyScale * 0.35F), "Slime") {}
