#include "SlimeEnemy.hpp"

#include "SpriteAssets.hpp"

SlimeEnemy::SlimeEnemy(const std::string &fontPath,
                       const glm::vec2 &spawnPosition, float difficultyScale)
    : Enemy(SpriteAssets::Path("slime.png"), {1.05F, 1.05F}, spawnPosition,
            17.0F, 85.0F + difficultyScale * 16.0F,
            3 + static_cast<int>(difficultyScale * 0.8F), 1,
            1 + static_cast<int>(difficultyScale * 0.35F), "Slime") {
    (void)fontPath;
}
