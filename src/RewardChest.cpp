#include "RewardChest.hpp"

RewardChest::RewardChest(const std::string &fontPath,
                         const glm::vec2 &spawnPosition)
    : GlyphObject(fontPath, "#", 30, Util::Color(255, 210, 80), 6.0F, 18.0F) {
    SetPosition(spawnPosition);
}
