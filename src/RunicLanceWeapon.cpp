#include "RunicLanceWeapon.hpp"

#include <algorithm>
#include <cmath>
#include <limits>

#include <glm/geometric.hpp>

#include "Enemy.hpp"
#include "Player.hpp"
#include "Projectile.hpp"

RunicLanceWeapon::RunicLanceWeapon(const std::string &fontPath)
    : Weapon(WeaponType::RunicLance, 0.24F, 1),
      m_FontPath(fontPath) {}

std::vector<std::shared_ptr<Projectile>> RunicLanceWeapon::UpdateAndFire(
    const Player &owner, const std::vector<std::shared_ptr<Enemy>> &enemies,
    float deltaTimeSeconds) {
    std::vector<std::shared_ptr<Projectile>> spawnedProjectiles;

    m_CooldownRemaining -= deltaTimeSeconds;
    if (m_CooldownRemaining > 0.0F) {
        return spawnedProjectiles;
    }

    std::shared_ptr<Enemy> nearestEnemy = nullptr;
    float nearestDistance = std::numeric_limits<float>::max();

    for (const auto &enemy : enemies) {
        if (!enemy->IsAlive()) {
            continue;
        }

        const float distance =
            glm::distance(owner.GetPosition(), enemy->GetPosition());
        if (distance < nearestDistance) {
            nearestDistance = distance;
            nearestEnemy = enemy;
        }
    }

    if (nearestEnemy == nullptr) {
        return spawnedProjectiles;
    }

    glm::vec2 baseDirection = nearestEnemy->GetPosition() - owner.GetPosition();
    if (glm::length(baseDirection) > 0.0F) {
        baseDirection = glm::normalize(baseDirection);
    }

    const int projectileCount = m_BaseProjectileCount + m_BonusProjectileCount;
    for (int i = 0; i < projectileCount; ++i) {
        const float spreadStep = projectileCount == 1 ? 0.0F : 0.12F;
        const float angleOffset =
            (static_cast<float>(i) -
             static_cast<float>(projectileCount - 1) * 0.5F) *
            spreadStep;
        const float cosAngle = std::cos(angleOffset);
        const float sinAngle = std::sin(angleOffset);
        const glm::vec2 direction{
            baseDirection.x * cosAngle - baseDirection.y * sinAngle,
            baseDirection.x * sinAngle + baseDirection.y * cosAngle,
        };

        spawnedProjectiles.push_back(std::make_shared<Projectile>(
            m_FontPath, owner.GetPosition(), direction, m_ProjectileSpeed,
            m_Damage, m_LifetimeSeconds, "*", 18,
            Util::Color(160, 255, 210), 11.0F));
    }

    m_CooldownRemaining = m_CooldownSeconds;
    return spawnedProjectiles;
}

void RunicLanceWeapon::IncreaseDamage(int amount) {
    m_Damage += amount;
}

void RunicLanceWeapon::MultiplyCooldown(float multiplier) {
    m_CooldownSeconds = std::max(0.12F, m_CooldownSeconds * multiplier);
    m_CooldownRemaining = std::min(m_CooldownRemaining, m_CooldownSeconds);
}

void RunicLanceWeapon::IncreaseProjectileCount(int amount) {
    m_BonusProjectileCount += amount;
}
