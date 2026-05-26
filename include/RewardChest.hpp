#ifndef REWARD_CHEST_HPP
#define REWARD_CHEST_HPP

#include "GlyphObject.hpp"

class RewardChest : public GlyphObject {
public:
    RewardChest(const std::string &fontPath, const glm::vec2 &spawnPosition);
};

#endif
