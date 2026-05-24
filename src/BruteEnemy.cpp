#include "BruteEnemy.hpp"

BruteEnemy::BruteEnemy(const std::string &fontPath,
                       const glm::vec2 &spawnPosition, float difficultyScale)
    : Enemy(fontPath, "B", 34, Util::Color(235, 95, 95), spawnPosition, 22.0F,
            58.0F + difficultyScale * 11.0F,
            7 + static_cast<int>(difficultyScale * 1.4F), 2,
            3 + static_cast<int>(difficultyScale * 0.45F), "Brute") {}
