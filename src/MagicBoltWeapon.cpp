#include "MagicBoltWeapon.hpp"

#include <glm/geometric.hpp>

#include "Enemy.hpp"
#include "Player.hpp"
#include "Projectile.hpp"

MagicBoltWeapon::MagicBoltWeapon(const std::string &fontPath)
    : Weapon(0.6F),
      m_FontPath(fontPath) {}

std::vector<std::shared_ptr<Projectile>> MagicBoltWeapon::UpdateAndFire(
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

        const float distance = glm::distance(owner.GetPosition(), enemy->GetPosition());
        if (distance < nearestDistance) {
            nearestDistance = distance;
            nearestEnemy = enemy;
        }
    }

    if (nearestEnemy == nullptr) {
        return spawnedProjectiles;
    }

    spawnedProjectiles.push_back(std::make_shared<Projectile>(
        m_FontPath, owner.GetPosition(),
        nearestEnemy->GetPosition() - owner.GetPosition(), m_ProjectileSpeed,
        m_Damage, m_LifetimeSeconds));
    m_CooldownRemaining = m_CooldownSeconds;

    return spawnedProjectiles;
}

void MagicBoltWeapon::OnOwnerLevelUp() {
    m_CooldownSeconds = std::max(0.15F, m_CooldownSeconds * 0.92F);
    ++m_Damage;
    m_ProjectileSpeed += 18.0F;
}

void MagicBoltWeapon::IncreaseDamage(int amount) {
    m_Damage += amount;
}

void MagicBoltWeapon::MultiplyCooldown(float multiplier) {
    m_CooldownSeconds = std::max(0.15F, m_CooldownSeconds * multiplier);
    m_CooldownRemaining = std::min(m_CooldownRemaining, m_CooldownSeconds);
}
