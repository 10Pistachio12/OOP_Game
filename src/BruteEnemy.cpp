#include "BruteEnemy.hpp"

#include "SpriteAssets.hpp"

BruteEnemy::BruteEnemy(const std::string &fontPath,
                       const glm::vec2 &spawnPosition, float difficultyScale)
    : Enemy(SpriteAssets::Path("brute.png"), {1.35F, 1.35F}, spawnPosition, 22.0F,
            58.0F + difficultyScale * 11.0F,
            7 + static_cast<int>(difficultyScale * 1.4F), 2,
            3 + static_cast<int>(difficultyScale * 0.45F), "Brute") {
    (void)fontPath;
}
