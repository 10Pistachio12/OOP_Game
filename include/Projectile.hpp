#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include "GlyphObject.hpp"

class Projectile : public GlyphObject {
public:
    Projectile(const std::string &fontPath, const glm::vec2 &spawnPosition,
               const glm::vec2 &direction, float speed, int damage,
               float lifetimeSeconds, std::string glyph = "*",
               int fontSize = 18,
               Util::Color color = Util::Color(247, 222, 104),
               float radius = 10.0F);

    virtual void Update(float deltaTimeSeconds);

    int GetDamage() const { return m_Damage; }

private:
    glm::vec2 m_Velocity{0.0F, 0.0F};
    int m_Damage = 1;
    float m_RemainingLifetime = 0.0F;
};

#endif
