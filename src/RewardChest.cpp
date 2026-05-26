#include "RewardChest.hpp"

#include "SpriteAssets.hpp"

RewardChest::RewardChest(const std::string &fontPath,
                         const glm::vec2 &spawnPosition)
    : GlyphObject(SpriteAssets::Path("chest.png"), {1.2F, 1.2F}, 6.0F, 18.0F) {
    (void)fontPath;

    SetPosition(spawnPosition);
}
