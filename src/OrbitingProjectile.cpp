#include "OrbitingProjectile.hpp"

#include <cmath>

#include "Enemy.hpp"
#include "Player.hpp"

namespace {
constexpr float CONTACT_DAMAGE_COOLDOWN_SECONDS = 0.45F;
}

OrbitingProjectile::OrbitingProjectile(const std::string &fontPath,
                                       const Player &owner,
                                       float initialAngle,
                                       float orbitRadius,
                                       float angularSpeed, int damage,
                                       float lifetimeSeconds)
    : Projectile(fontPath, owner.GetPosition(), {0.0F, 0.0F}, 0.0F, damage,
                 lifetimeSeconds, "s", 18, Util::Color(120, 245, 210),
                 11.0F),
      m_Owner(&owner),
      m_Angle(initialAngle),
      m_OrbitRadius(orbitRadius),
      m_AngularSpeed(angularSpeed),
      m_RemainingLifetime(lifetimeSeconds) {}

void OrbitingProjectile::Update(float deltaTimeSeconds) {
    m_RemainingLifetime -= deltaTimeSeconds;
    if (m_RemainingLifetime <= 0.0F) {
        Destroy();
        return;
    }

    for (auto it = m_EnemyHitCooldowns.begin();
         it != m_EnemyHitCooldowns.end();) {
        it->second -= deltaTimeSeconds;
        if (it->second <= 0.0F) {
            it = m_EnemyHitCooldowns.erase(it);
        } else {
            ++it;
        }
    }

    m_Angle += m_AngularSpeed * deltaTimeSeconds;
    SetPosition(m_Owner->GetPosition() +
                glm::vec2(std::cos(m_Angle), std::sin(m_Angle)) *
                    m_OrbitRadius);
    m_Transform.rotation += m_AngularSpeed * deltaTimeSeconds;
}

void OrbitingProjectile::OnHitEnemy(Enemy &enemy) {
    m_EnemyHitCooldowns[&enemy] = CONTACT_DAMAGE_COOLDOWN_SECONDS;
}

bool OrbitingProjectile::CanDamageEnemy(const Enemy &enemy) const {
    return m_EnemyHitCooldowns.find(&enemy) == m_EnemyHitCooldowns.end();
}
