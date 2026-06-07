#include "Projectile.hpp"

#include <glm/geometric.hpp>

#include "SpriteAssets.hpp"

namespace {
std::string ProjectileSpritePath(const std::string &glyph) {
    if (glyph == "o") {
        return SpriteAssets::Path("nova_bolt.png");
    }
    if (glyph == "s") {
        return SpriteAssets::Path("shield_orb.png");
    }

    return SpriteAssets::Path("magic_bolt.png");
}

glm::vec2 ProjectileSpriteScale(const std::string &glyph) {
    return glyph == "s" ? glm::vec2{0.7F, 0.7F} : glm::vec2{0.55F, 0.55F};
}
} // namespace

Projectile::Projectile(const std::string &fontPath, const glm::vec2 &spawnPosition,
                       const glm::vec2 &direction, float speed, int damage,
                       float lifetimeSeconds, std::string glyph, int fontSize,
                       Util::Color color, float radius)
    : GlyphObject(ProjectileSpritePath(glyph), ProjectileSpriteScale(glyph),
                  7.0F, radius),
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
