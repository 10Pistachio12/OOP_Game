#include "StarfallNovaWeapon.hpp"

#include <algorithm>
#include <cmath>

#include "Enemy.hpp"
#include "Player.hpp"
#include "Projectile.hpp"

namespace {
constexpr float TWO_PI = 6.28318530718F;
}

StarfallNovaWeapon::StarfallNovaWeapon(const std::string &fontPath)
    : Weapon(WeaponType::StarfallNova, 1.05F, 1),
      m_FontPath(fontPath) {}

std::vector<std::shared_ptr<Projectile>> StarfallNovaWeapon::UpdateAndFire(
    const Player &owner, const std::vector<std::shared_ptr<Enemy>> &enemies,
    float deltaTimeSeconds) {
    std::vector<std::shared_ptr<Projectile>> spawnedProjectiles;

    m_CooldownRemaining -= deltaTimeSeconds;
    if (m_CooldownRemaining > 0.0F || !HasLiveTarget(enemies)) {
        return spawnedProjectiles;
    }

    const int projectileCount =
        m_BaseProjectilesPerBurst + m_BonusProjectileCount * 3;
    for (int i = 0; i < projectileCount; ++i) {
        const float angle =
            m_BurstAngleOffset + TWO_PI * static_cast<float>(i) /
                                     static_cast<float>(projectileCount);
        const glm::vec2 direction{std::cos(angle), std::sin(angle)};
        const bool outerRing = i % 2 == 0;
        const float speedMultiplier = outerRing ? 1.0F : 0.72F;
        spawnedProjectiles.push_back(std::make_shared<Projectile>(
            m_FontPath, owner.GetPosition(), direction,
            m_ProjectileSpeed * speedMultiplier, m_Damage, m_LifetimeSeconds,
            "o", 18, Util::Color(150, 245, 255), outerRing ? 10.5F : 8.5F));
    }

    m_BurstAngleOffset += 0.47F;
    m_CooldownRemaining = m_CooldownSeconds;

    return spawnedProjectiles;
}

void StarfallNovaWeapon::IncreaseDamage(int amount) {
    m_Damage += amount;
}

void StarfallNovaWeapon::MultiplyCooldown(float multiplier) {
    m_CooldownSeconds = std::max(0.38F, m_CooldownSeconds * multiplier);
    m_CooldownRemaining = std::min(m_CooldownRemaining, m_CooldownSeconds);
}

void StarfallNovaWeapon::IncreaseProjectileCount(int amount) {
    m_BonusProjectileCount += amount;
}

bool StarfallNovaWeapon::HasLiveTarget(
    const std::vector<std::shared_ptr<Enemy>> &enemies) const {
    return std::any_of(enemies.begin(), enemies.end(),
                       [](const auto &enemy) { return enemy->IsAlive(); });
}
