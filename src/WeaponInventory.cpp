#include "WeaponInventory.hpp"

#include <algorithm>
#include <sstream>

#include "ArcaneNovaWeapon.hpp"
#include "MagicBoltWeapon.hpp"
#include "OrbitingShieldWeapon.hpp"
#include "RunicLanceWeapon.hpp"
#include "UpgradeManager.hpp"

namespace {
constexpr int MAGIC_BOLT_EVOLUTION_DAMAGE_LEVEL = 3;
}

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

std::string WeaponInventory::LevelUpRandomWeapon(std::mt19937 &rng) {
    std::vector<Weapon *> candidates;
    for (const auto &weapon : m_Weapons) {
        if (weapon->CanLevelUp()) {
            candidates.push_back(weapon.get());
        }
    }

    if (candidates.empty()) {
        return "";
    }

    const int selectedIndex = std::uniform_int_distribution<int>(
        0, static_cast<int>(candidates.size()) - 1)(rng);
    Weapon *selectedWeapon = candidates[static_cast<std::size_t>(selectedIndex)];
    selectedWeapon->LevelUp();

    return selectedWeapon->GetDisplayName() + " Lv." +
           std::to_string(selectedWeapon->GetLevel());
}

std::string WeaponInventory::EvolveRandomWeapon(
    std::mt19937 &rng, const UpgradeManager &upgrades) {
    std::vector<WeaponType> candidates;
    if (CanEvolveMagicBolt(upgrades)) {
        candidates.push_back(WeaponType::MagicBolt);
    }

    if (candidates.empty()) {
        return "";
    }

    const int selectedIndex = std::uniform_int_distribution<int>(
        0, static_cast<int>(candidates.size()) - 1)(rng);
    const WeaponType selectedType =
        candidates[static_cast<std::size_t>(selectedIndex)];

    switch (selectedType) {
        case WeaponType::MagicBolt: {
            auto evolvedWeapon = std::make_unique<RunicLanceWeapon>(m_FontPath);
            ApplyWeaponPassiveHistory(*evolvedWeapon, upgrades);
            if (ReplaceWeapon(WeaponType::MagicBolt,
                              std::move(evolvedWeapon))) {
                return "Magic Bolt evolved into Runic Lance";
            }
            break;
        }
        default:
            break;
    }

    return "";
}

bool WeaponInventory::LevelUpAllWeapons() {
    bool didLevelUp = false;
    for (const auto &weapon : m_Weapons) {
        didLevelUp = weapon->LevelUp() || didLevelUp;
    }

    return didLevelUp;
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

void WeaponInventory::IncreaseProjectileCount(int amount) {
    for (const auto &weapon : m_Weapons) {
        weapon->IncreaseProjectileCount(amount);
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
        if (m_Weapons[i]->IsEvolved()) {
            stream << " EVO";
        } else {
            stream << " Lv." << m_Weapons[i]->GetLevel();
        }
    }

    return stream.str();
}

std::string WeaponInventory::GetEvolutionStatus(
    const UpgradeManager &upgrades) const {
    if (HasWeapon(WeaponType::RunicLance)) {
        return "Runic Lance evolved";
    }

    const Weapon *magicBolt = FindWeapon(WeaponType::MagicBolt);
    if (magicBolt == nullptr) {
        return "Magic Bolt not owned";
    }

    std::ostringstream stream;
    stream << "Magic Bolt -> Runic Lance: ";
    if (CanEvolveMagicBolt(upgrades)) {
        stream << "READY - open an elite chest";
        return stream.str();
    }

    const int weaponLevel = magicBolt->GetLevel();
    const int weaponMaxLevel = magicBolt->GetMaxLevel();
    const int damageLevel =
        upgrades.GetPassiveUpgradeLevel(PassiveUpgradeType::WeaponDamage);
    stream << "Magic Bolt Lv." << weaponLevel << "/" << weaponMaxLevel
           << ", Sharper Arsenal Lv."
           << std::min(damageLevel, MAGIC_BOLT_EVOLUTION_DAMAGE_LEVEL) << "/"
           << MAGIC_BOLT_EVOLUTION_DAMAGE_LEVEL;

    return stream.str();
}

void WeaponInventory::ApplyWeaponPassiveHistory(
    Weapon &weapon, const UpgradeManager &upgrades) const {
    weapon.IncreaseDamage(
        upgrades.GetPassiveUpgradeLevel(PassiveUpgradeType::WeaponDamage));

    for (int i = 0;
         i < upgrades.GetPassiveUpgradeLevel(PassiveUpgradeType::WeaponCooldown);
         ++i) {
        weapon.MultiplyCooldown(0.86F);
    }

    weapon.IncreaseProjectileCount(
        upgrades.GetPassiveUpgradeLevel(PassiveUpgradeType::ProjectileCount));
}

bool WeaponInventory::CanEvolveMagicBolt(
    const UpgradeManager &upgrades) const {
    const Weapon *weapon = FindWeapon(WeaponType::MagicBolt);
    return weapon != nullptr && !weapon->CanLevelUp() &&
           upgrades.HasPassiveUpgradeLevel(
               PassiveUpgradeType::WeaponDamage,
               MAGIC_BOLT_EVOLUTION_DAMAGE_LEVEL);
}

bool WeaponInventory::ReplaceWeapon(WeaponType originalType,
                                    std::unique_ptr<Weapon> evolvedWeapon) {
    if (evolvedWeapon == nullptr) {
        return false;
    }

    const auto it =
        std::find_if(m_Weapons.begin(), m_Weapons.end(),
                     [originalType](const auto &weapon) {
                         return weapon->GetType() == originalType;
                     });
    if (it == m_Weapons.end()) {
        return false;
    }

    *it = std::move(evolvedWeapon);
    return true;
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
        case WeaponType::OrbitingShield:
            return std::make_unique<OrbitingShieldWeapon>(m_FontPath);
        case WeaponType::RunicLance:
            return std::make_unique<RunicLanceWeapon>(m_FontPath);
        default:
            return nullptr;
    }
}
