#include "ExperienceGem.hpp"

#include <glm/geometric.hpp>

#include "SpriteAssets.hpp"

ExperienceGem::ExperienceGem(const std::string &fontPath,
                             const glm::vec2 &spawnPosition, int value)
    : GlyphObject(SpriteAssets::Path("gem.png"), {0.7F, 0.7F}, 4.0F, 12.0F),
      m_Value(value) {
    (void)fontPath;

    SetPosition(spawnPosition);
}

void ExperienceGem::Update(float deltaTimeSeconds,
                           const glm::vec2 &targetPosition,
                           float attractionRadius) {
    glm::vec2 direction = targetPosition - GetPosition();
    const float distance = glm::length(direction);
    if (distance <= 0.0F || distance > attractionRadius) {
        return;
    }

    direction = glm::normalize(direction);
    const float pullRatio = 1.0F - distance / attractionRadius;
    const float speed = m_AttractionSpeed + pullRatio * 340.0F;
    m_Transform.translation += direction * speed * deltaTimeSeconds;
}
