#include "OrbitingShieldWeapon.hpp"

#include <algorithm>
#include <cmath>

#include "Enemy.hpp"
#include "OrbitingProjectile.hpp"
#include "Player.hpp"

namespace {
constexpr float TWO_PI = 6.28318530718F;
}

OrbitingShieldWeapon::OrbitingShieldWeapon(const std::string &fontPath)
    : Weapon(WeaponType::OrbitingShield, 3.1F, 5),
      m_FontPath(fontPath) {}

std::vector<std::shared_ptr<Projectile>> OrbitingShieldWeapon::UpdateAndFire(
    const Player &owner, const std::vector<std::shared_ptr<Enemy>> &enemies,
    float deltaTimeSeconds) {
    std::vector<std::shared_ptr<Projectile>> spawnedProjectiles;

    m_CooldownRemaining -= deltaTimeSeconds;
    if (m_CooldownRemaining > 0.0F || !HasLiveTarget(enemies)) {
        return spawnedProjectiles;
    }

    const int orbCount = m_OrbCount + m_BonusProjectileCount;
    for (int i = 0; i < orbCount; ++i) {
        const float angle =
            m_AngleOffset + TWO_PI * static_cast<float>(i) /
                                static_cast<float>(orbCount);
        spawnedProjectiles.push_back(std::make_shared<OrbitingProjectile>(
            m_FontPath, owner, angle, m_OrbitRadius, m_AngularSpeed, m_Damage,
            m_LifetimeSeconds));
    }

    m_AngleOffset += 0.47F;
    m_CooldownRemaining = m_CooldownSeconds;

    return spawnedProjectiles;
}

std::string OrbitingShieldWeapon::GetLevelUpDescription() const {
    switch (GetLevel() + 1) {
        case 2:
            return "+1 orbiting orb";
        case 3:
            return "+1 damage";
        case 4:
            return "-14% cooldown and +0.25s duration";
        case 5:
            return "+1 orb and wider orbit";
        default:
            return "Max level";
    }
}

void OrbitingShieldWeapon::ApplyLevelUp() {
    switch (GetLevel()) {
        case 2:
            ++m_OrbCount;
            break;
        case 3:
            IncreaseDamage(1);
            break;
        case 4:
            MultiplyCooldown(0.86F);
            m_LifetimeSeconds += 0.25F;
            break;
        case 5:
            ++m_OrbCount;
            m_OrbitRadius += 10.0F;
            break;
        default:
            break;
    }
}

void OrbitingShieldWeapon::IncreaseDamage(int amount) {
    m_Damage += amount;
}

void OrbitingShieldWeapon::MultiplyCooldown(float multiplier) {
    m_CooldownSeconds = std::max(0.85F, m_CooldownSeconds * multiplier);
    m_CooldownRemaining = std::min(m_CooldownRemaining, m_CooldownSeconds);
}

bool OrbitingShieldWeapon::HasLiveTarget(
    const std::vector<std::shared_ptr<Enemy>> &enemies) const {
    return std::any_of(enemies.begin(), enemies.end(),
                       [](const auto &enemy) { return enemy->IsAlive(); });
}
