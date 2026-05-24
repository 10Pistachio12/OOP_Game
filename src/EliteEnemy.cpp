#include "EliteEnemy.hpp"

EliteEnemy::EliteEnemy(const std::string &fontPath,
                       const glm::vec2 &spawnPosition, float difficultyScale)
    : Enemy(fontPath, "E", 40, Util::Color(215, 120, 255), spawnPosition,
            28.0F, 70.0F + difficultyScale * 9.0F,
            22 + static_cast<int>(difficultyScale * 4.5F), 3,
            14 + static_cast<int>(difficultyScale * 2.5F), "Elite", true) {}
