#include "ArcaneNovaWeapon.hpp"

#include <algorithm>
#include <cmath>

#include "Enemy.hpp"
#include "Player.hpp"
#include "Projectile.hpp"

namespace {
constexpr float TWO_PI = 6.28318530718F;
}

ArcaneNovaWeapon::ArcaneNovaWeapon(const std::string &fontPath)
    : Weapon(WeaponType::ArcaneNova, 2.4F, 5),
      m_FontPath(fontPath) {}

std::vector<std::shared_ptr<Projectile>> ArcaneNovaWeapon::UpdateAndFire(
    const Player &owner, const std::vector<std::shared_ptr<Enemy>> &enemies,
    float deltaTimeSeconds) {
    std::vector<std::shared_ptr<Projectile>> spawnedProjectiles;

    m_CooldownRemaining -= deltaTimeSeconds;
    if (m_CooldownRemaining > 0.0F || !HasLiveTarget(enemies)) {
        return spawnedProjectiles;
    }

    const int projectileCount = m_ProjectilesPerBurst + m_BonusProjectileCount * 2;
    for (int i = 0; i < projectileCount; ++i) {
        const float angle =
            m_BurstAngleOffset + TWO_PI * static_cast<float>(i) /
                                     static_cast<float>(projectileCount);
        const glm::vec2 direction{std::cos(angle), std::sin(angle)};
        spawnedProjectiles.push_back(std::make_shared<Projectile>(
            m_FontPath, owner.GetPosition(), direction, m_ProjectileSpeed,
            m_Damage, m_LifetimeSeconds, "o", 18, Util::Color(105, 210, 255),
            9.0F));
    }

    m_BurstAngleOffset += 0.31F;
    m_CooldownRemaining = m_CooldownSeconds;

    return spawnedProjectiles;
}

std::string ArcaneNovaWeapon::GetLevelUpDescription() const {
    switch (GetLevel() + 1) {
        case 2:
            return "+2 projectiles per burst";
        case 3:
            return "-15% cooldown";
        case 4:
            return "+1 damage";
        case 5:
            return "+2 projectiles and +0.25s lifetime";
        default:
            return "Max level";
    }
}

void ArcaneNovaWeapon::ApplyLevelUp() {
    switch (GetLevel()) {
        case 2:
            m_ProjectilesPerBurst += 2;
            break;
        case 3:
            MultiplyCooldown(0.85F);
            break;
        case 4:
            IncreaseDamage(1);
            break;
        case 5:
            m_ProjectilesPerBurst += 2;
            m_LifetimeSeconds += 0.25F;
            break;
        default:
            break;
    }
}

void ArcaneNovaWeapon::IncreaseDamage(int amount) {
    m_Damage += amount;
}

void ArcaneNovaWeapon::MultiplyCooldown(float multiplier) {
    m_CooldownSeconds = std::max(0.65F, m_CooldownSeconds * multiplier);
    m_CooldownRemaining = std::min(m_CooldownRemaining, m_CooldownSeconds);
}

bool ArcaneNovaWeapon::HasLiveTarget(
    const std::vector<std::shared_ptr<Enemy>> &enemies) const {
    return std::any_of(enemies.begin(), enemies.end(),
                       [](const auto &enemy) { return enemy->IsAlive(); });
}
