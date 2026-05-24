#include "MagicBoltWeapon.hpp"

#include <glm/geometric.hpp>

#include "Enemy.hpp"
#include "Player.hpp"
#include "Projectile.hpp"

MagicBoltWeapon::MagicBoltWeapon(const std::string &fontPath)
    : Weapon(WeaponType::MagicBolt, 0.6F, 5),
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

std::string MagicBoltWeapon::GetLevelUpDescription() const {
    switch (GetLevel() + 1) {
        case 2:
            return "+1 damage";
        case 3:
            return "-12% cooldown";
        case 4:
            return "+45 projectile speed and +0.2s lifetime";
        case 5:
            return "+1 damage and -10% cooldown";
        default:
            return "Max level";
    }
}

void MagicBoltWeapon::ApplyLevelUp() {
    switch (GetLevel()) {
        case 2:
            IncreaseDamage(1);
            break;
        case 3:
            MultiplyCooldown(0.88F);
            break;
        case 4:
            m_ProjectileSpeed += 45.0F;
            m_LifetimeSeconds += 0.2F;
            break;
        case 5:
            IncreaseDamage(1);
            MultiplyCooldown(0.90F);
            break;
        default:
            break;
    }
}

void MagicBoltWeapon::IncreaseDamage(int amount) {
    m_Damage += amount;
}

void MagicBoltWeapon::MultiplyCooldown(float multiplier) {
    m_CooldownSeconds = std::max(0.15F, m_CooldownSeconds * multiplier);
    m_CooldownRemaining = std::min(m_CooldownRemaining, m_CooldownSeconds);
}
