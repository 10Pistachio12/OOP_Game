#include "AegisHaloWeapon.hpp"

#include <algorithm>
#include <cmath>

#include "Enemy.hpp"
#include "OrbitingProjectile.hpp"
#include "Player.hpp"

namespace {
constexpr float TWO_PI = 6.28318530718F;
}

AegisHaloWeapon::AegisHaloWeapon(const std::string &fontPath)
    : Weapon(WeaponType::AegisHalo, 1.75F, 1),
      m_FontPath(fontPath) {}

std::vector<std::shared_ptr<Projectile>> AegisHaloWeapon::UpdateAndFire(
    const Player &owner, const std::vector<std::shared_ptr<Enemy>> &enemies,
    float deltaTimeSeconds) {
    std::vector<std::shared_ptr<Projectile>> spawnedProjectiles;

    m_CooldownRemaining -= deltaTimeSeconds;
    if (m_CooldownRemaining > 0.0F || !HasLiveTarget(enemies)) {
        return spawnedProjectiles;
    }

    const int orbCount = m_BaseOrbCount + m_BonusProjectileCount;
    for (int i = 0; i < orbCount; ++i) {
        const float angle =
            m_AngleOffset + TWO_PI * static_cast<float>(i) /
                                static_cast<float>(orbCount);
        spawnedProjectiles.push_back(std::make_shared<OrbitingProjectile>(
            m_FontPath, owner, angle, m_InnerOrbitRadius, m_AngularSpeed,
            m_Damage, m_LifetimeSeconds));

        const float outerAngle = angle + TWO_PI / static_cast<float>(orbCount) *
                                             0.5F;
        spawnedProjectiles.push_back(std::make_shared<OrbitingProjectile>(
            m_FontPath, owner, outerAngle, m_OuterOrbitRadius,
            -m_AngularSpeed * 0.78F, m_Damage, m_LifetimeSeconds * 0.92F));
    }

    m_AngleOffset += 0.39F;
    m_CooldownRemaining = m_CooldownSeconds;

    return spawnedProjectiles;
}

void AegisHaloWeapon::IncreaseDamage(int amount) {
    m_Damage += amount;
}

void AegisHaloWeapon::MultiplyCooldown(float multiplier) {
    m_CooldownSeconds = std::max(0.55F, m_CooldownSeconds * multiplier);
    m_CooldownRemaining = std::min(m_CooldownRemaining, m_CooldownSeconds);
}

void AegisHaloWeapon::IncreaseProjectileCount(int amount) {
    m_BonusProjectileCount += amount;
}

bool AegisHaloWeapon::HasLiveTarget(
    const std::vector<std::shared_ptr<Enemy>> &enemies) const {
    return std::any_of(enemies.begin(), enemies.end(),
                       [](const auto &enemy) { return enemy->IsAlive(); });
}
