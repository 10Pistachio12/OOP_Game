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
    : Weapon(2.4F),
      m_FontPath(fontPath) {}

std::vector<std::shared_ptr<Projectile>> ArcaneNovaWeapon::UpdateAndFire(
    const Player &owner, const std::vector<std::shared_ptr<Enemy>> &enemies,
    float deltaTimeSeconds) {
    std::vector<std::shared_ptr<Projectile>> spawnedProjectiles;

    m_CooldownRemaining -= deltaTimeSeconds;
    if (m_CooldownRemaining > 0.0F || !HasLiveTarget(enemies)) {
        return spawnedProjectiles;
    }

    for (int i = 0; i < m_ProjectilesPerBurst; ++i) {
        const float angle =
            m_BurstAngleOffset + TWO_PI * static_cast<float>(i) /
                                     static_cast<float>(m_ProjectilesPerBurst);
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

void ArcaneNovaWeapon::OnOwnerLevelUp() {
    m_CooldownSeconds = std::max(0.65F, m_CooldownSeconds * 0.94F);
    if (m_ProjectilesPerBurst < 16) {
        ++m_ProjectilesPerBurst;
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
