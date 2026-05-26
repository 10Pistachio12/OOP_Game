#include "Projectile.hpp"

#include <glm/geometric.hpp>

#include "SpriteAssets.hpp"

namespace {
std::string ProjectileSpritePath(const std::string &glyph) {
    return SpriteAssets::Path(glyph == "o" ? "nova_bolt.png"
                                            : "magic_bolt.png");
}
} // namespace

Projectile::Projectile(const std::string &fontPath, const glm::vec2 &spawnPosition,
                       const glm::vec2 &direction, float speed, int damage,
                       float lifetimeSeconds, std::string glyph, int fontSize,
                       Util::Color color, float radius)
    : GlyphObject(ProjectileSpritePath(glyph), {0.55F, 0.55F}, 7.0F, radius),
      m_Damage(damage),
      m_RemainingLifetime(lifetimeSeconds) {
    (void)fontPath;
    (void)fontSize;
    (void)color;

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
