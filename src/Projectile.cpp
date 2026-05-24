#include "Projectile.hpp"

#include <glm/geometric.hpp>

Projectile::Projectile(const std::string &fontPath, const glm::vec2 &spawnPosition,
                       const glm::vec2 &direction, float speed, int damage,
                       float lifetimeSeconds)
    : GlyphObject(fontPath, "*", 18, Util::Color(247, 222, 104), 7.0F, 10.0F),
      m_Damage(damage),
      m_RemainingLifetime(lifetimeSeconds) {
    SetPosition(spawnPosition);

    if (glm::length(direction) > 0.0F) {
        m_Velocity = glm::normalize(direction) * speed;
    }
}

void Projectile::Update(float deltaTimeSeconds) {
    m_Transform.translation += m_Velocity * deltaTimeSeconds;
    m_RemainingLifetime -= deltaTimeSeconds;
    if (m_RemainingLifetime <= 0.0F) {
        Destroy();
    }
}
