#include "WeaponInventory.hpp"

#include <sstream>

void WeaponInventory::AddWeapon(std::unique_ptr<Weapon> weapon) {
    if (weapon == nullptr) {
        return;
    }

    m_Weapons.push_back(std::move(weapon));
}

std::vector<std::shared_ptr<Projectile>> WeaponInventory::UpdateAndFire(
    const Player &owner, const std::vector<std::shared_ptr<Enemy>> &enemies,
    float deltaTimeSeconds) {
    std::vector<std::shared_ptr<Projectile>> spawnedProjectiles;

    for (const auto &weapon : m_Weapons) {
        auto weaponProjectiles =
            weapon->UpdateAndFire(owner, enemies, deltaTimeSeconds);
        spawnedProjectiles.insert(spawnedProjectiles.end(),
                                  weaponProjectiles.begin(),
                                  weaponProjectiles.end());
    }

    return spawnedProjectiles;
}

void WeaponInventory::IncreaseDamage(int amount) {
    for (const auto &weapon : m_Weapons) {
        weapon->IncreaseDamage(amount);
    }
}

void WeaponInventory::MultiplyCooldown(float multiplier) {
    for (const auto &weapon : m_Weapons) {
        weapon->MultiplyCooldown(multiplier);
    }
}

void WeaponInventory::OnOwnerLevelUp() {
    for (const auto &weapon : m_Weapons) {
        weapon->OnOwnerLevelUp();
    }
}

std::string WeaponInventory::GetDisplayNames() const {
    if (m_Weapons.empty()) {
        return "None";
    }

    std::ostringstream stream;
    for (std::size_t i = 0; i < m_Weapons.size(); ++i) {
        if (i > 0) {
            stream << ", ";
        }
        stream << m_Weapons[i]->GetDisplayName();
    }

    return stream.str();
}
