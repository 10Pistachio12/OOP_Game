#include "WeaponInventory.hpp"

#include <algorithm>
#include <sstream>

#include "ArcaneNovaWeapon.hpp"
#include "MagicBoltWeapon.hpp"

WeaponInventory::WeaponInventory(std::string fontPath)
    : m_FontPath(std::move(fontPath)) {}

void WeaponInventory::AddWeapon(std::unique_ptr<Weapon> weapon) {
    if (weapon == nullptr) {
        return;
    }
    if (HasWeapon(weapon->GetType())) {
        return;
    }

    m_Weapons.push_back(std::move(weapon));
}

bool WeaponInventory::UnlockWeapon(WeaponType type) {
    if (HasWeapon(type)) {
        return false;
    }

    auto weapon = CreateWeapon(type);
    if (weapon == nullptr) {
        return false;
    }

    AddWeapon(std::move(weapon));
    return true;
}

bool WeaponInventory::LevelUpWeapon(WeaponType type) {
    Weapon *weapon = FindWeapon(type);
    if (weapon == nullptr) {
        return false;
    }

    return weapon->LevelUp();
}

bool WeaponInventory::HasWeapon(WeaponType type) const {
    return FindWeapon(type) != nullptr;
}

bool WeaponInventory::CanLevelUpWeapon(WeaponType type) const {
    const Weapon *weapon = FindWeapon(type);
    return weapon != nullptr && weapon->CanLevelUp();
}

int WeaponInventory::GetWeaponLevel(WeaponType type) const {
    const Weapon *weapon = FindWeapon(type);
    return weapon == nullptr ? 0 : weapon->GetLevel();
}

int WeaponInventory::GetWeaponMaxLevel(WeaponType type) const {
    const Weapon *weapon = FindWeapon(type);
    return weapon == nullptr ? 0 : weapon->GetMaxLevel();
}

std::string WeaponInventory::GetWeaponLevelUpDescription(WeaponType type) const {
    const Weapon *weapon = FindWeapon(type);
    return weapon == nullptr ? "" : weapon->GetLevelUpDescription();
}

std::vector<WeaponType> WeaponInventory::GetOwnedWeaponTypes() const {
    std::vector<WeaponType> types;
    for (const auto &weapon : m_Weapons) {
        types.push_back(weapon->GetType());
    }

    return types;
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

std::string WeaponInventory::GetDisplayNames() const {
    if (m_Weapons.empty()) {
        return "None";
    }

    std::ostringstream stream;
    for (std::size_t i = 0; i < m_Weapons.size(); ++i) {
        if (i > 0) {
            stream << ", ";
        }
        stream << m_Weapons[i]->GetDisplayName() << " Lv."
               << m_Weapons[i]->GetLevel();
    }

    return stream.str();
}

Weapon *WeaponInventory::FindWeapon(WeaponType type) {
    const auto it =
        std::find_if(m_Weapons.begin(), m_Weapons.end(),
                     [type](const auto &weapon) { return weapon->GetType() == type; });
    return it == m_Weapons.end() ? nullptr : it->get();
}

const Weapon *WeaponInventory::FindWeapon(WeaponType type) const {
    const auto it =
        std::find_if(m_Weapons.begin(), m_Weapons.end(),
                     [type](const auto &weapon) { return weapon->GetType() == type; });
    return it == m_Weapons.end() ? nullptr : it->get();
}

std::unique_ptr<Weapon> WeaponInventory::CreateWeapon(WeaponType type) const {
    switch (type) {
        case WeaponType::MagicBolt:
            return std::make_unique<MagicBoltWeapon>(m_FontPath);
        case WeaponType::ArcaneNova:
            return std::make_unique<ArcaneNovaWeapon>(m_FontPath);
        default:
            return nullptr;
    }
}
