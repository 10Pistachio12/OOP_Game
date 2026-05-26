#include "EliteEnemy.hpp"

#include "SpriteAssets.hpp"

EliteEnemy::EliteEnemy(const std::string &fontPath,
                       const glm::vec2 &spawnPosition, float difficultyScale)
    : Enemy(SpriteAssets::Path("elite.png"), {1.6F, 1.6F}, spawnPosition,
            28.0F, 70.0F + difficultyScale * 9.0F,
            22 + static_cast<int>(difficultyScale * 4.5F), 3,
            14 + static_cast<int>(difficultyScale * 2.5F), "Elite", true) {
    (void)fontPath;
}
