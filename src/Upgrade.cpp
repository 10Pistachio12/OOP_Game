#include "Upgrade.hpp"

#include "Player.hpp"
#include "WeaponInventory.hpp"

std::string GetUpgradeRarityLabel(UpgradeRarity rarity) {
    switch (rarity) {
        case UpgradeRarity::Common:
            return "Common";
        case UpgradeRarity::Uncommon:
            return "Uncommon";
        case UpgradeRarity::Rare:
            return "Rare";
        default:
            return "Unknown";
    }
}

int GetPassiveUpgradeMaxLevel(PassiveUpgradeType type) {
    switch (type) {
        case PassiveUpgradeType::MoveSpeed:
            return 4;
        case PassiveUpgradeType::MaxHealth:
            return 4;
        case PassiveUpgradeType::PickupRange:
            return 4;
        case PassiveUpgradeType::WeaponDamage:
            return 5;
        case PassiveUpgradeType::WeaponCooldown:
            return 5;
        case PassiveUpgradeType::ProjectileCount:
            return 3;
        case PassiveUpgradeType::None:
        case PassiveUpgradeType::Count:
        default:
            return 0;
    }
}

PassiveUpgrade::PassiveUpgrade(PassiveUpgradeType type, int nextLevel,
                               int maxLevel)
    : m_Type(type),
      m_NextLevel(nextLevel),
      m_MaxLevel(maxLevel) {}

std::string PassiveUpgrade::FormatLeveledName(
    const std::string &baseName) const {
    return baseName + " Lv." + std::to_string(m_NextLevel) + "/" +
           std::to_string(m_MaxLevel);
}

MoveSpeedUpgrade::MoveSpeedUpgrade(int nextLevel, int maxLevel)
    : PassiveUpgrade(PassiveUpgradeType::MoveSpeed, nextLevel, maxLevel) {}

std::string MoveSpeedUpgrade::GetName() const {
    return FormatLeveledName("Swift Feet");
}

std::string MoveSpeedUpgrade::GetDescription() const {
    return "+35 move speed";
}

void MoveSpeedUpgrade::Apply(Player &player, WeaponInventory &) const {
    player.IncreaseMoveSpeed(35.0F);
}

MaxHealthUpgrade::MaxHealthUpgrade(int nextLevel, int maxLevel)
    : PassiveUpgrade(PassiveUpgradeType::MaxHealth, nextLevel, maxLevel) {}

std::string MaxHealthUpgrade::GetName() const {
    return FormatLeveledName("Vital Core");
}

std::string MaxHealthUpgrade::GetDescription() const {
    return "+2 max HP and heal 2";
}

void MaxHealthUpgrade::Apply(Player &player, WeaponInventory &) const {
    player.IncreaseMaxHitPoints(2);
    player.Heal(2);
}

PickupRangeUpgrade::PickupRangeUpgrade(int nextLevel, int maxLevel)
    : PassiveUpgrade(PassiveUpgradeType::PickupRange, nextLevel, maxLevel) {}

std::string PickupRangeUpgrade::GetName() const {
    return FormatLeveledName("Magnet Pulse");
}

std::string PickupRangeUpgrade::GetDescription() const {
    return "+28 pickup range";
}

void PickupRangeUpgrade::Apply(Player &player, WeaponInventory &) const {
    player.IncreasePickupRadius(28.0F);
}

WeaponDamageUpgrade::WeaponDamageUpgrade(int nextLevel, int maxLevel)
    : PassiveUpgrade(PassiveUpgradeType::WeaponDamage, nextLevel, maxLevel) {}

std::string WeaponDamageUpgrade::GetName() const {
    return FormatLeveledName("Sharper Arsenal");
}

std::string WeaponDamageUpgrade::GetDescription() const {
    return "+1 damage to all weapons";
}

void WeaponDamageUpgrade::Apply(Player &, WeaponInventory &weapons) const {
    weapons.IncreaseDamage(1);
}

WeaponCooldownUpgrade::WeaponCooldownUpgrade(int nextLevel, int maxLevel)
    : PassiveUpgrade(PassiveUpgradeType::WeaponCooldown, nextLevel, maxLevel) {}

std::string WeaponCooldownUpgrade::GetName() const {
    return FormatLeveledName("Quick Cast");
}

std::string WeaponCooldownUpgrade::GetDescription() const {
    return "-14% cooldown on all weapons";
}

void WeaponCooldownUpgrade::Apply(Player &, WeaponInventory &weapons) const {
    weapons.MultiplyCooldown(0.86F);
}

ProjectileCountUpgrade::ProjectileCountUpgrade(int nextLevel, int maxLevel)
    : PassiveUpgrade(PassiveUpgradeType::ProjectileCount, nextLevel,
                     maxLevel) {}

std::string ProjectileCountUpgrade::GetName() const {
    return FormatLeveledName("Twin Casting");
}

std::string ProjectileCountUpgrade::GetDescription() const {
    return "+1 projectile pattern to all weapons";
}

void ProjectileCountUpgrade::Apply(Player &, WeaponInventory &weapons) const {
    weapons.IncreaseProjectileCount(1);
}

void OverflowRecoveryUpgrade::Apply(Player &player, WeaponInventory &) const {
    player.Heal(4);
}

UnlockWeaponUpgrade::UnlockWeaponUpgrade(WeaponType weaponType)
    : m_WeaponType(weaponType) {}

std::string UnlockWeaponUpgrade::GetName() const {
    return "Unlock " + GetWeaponTypeDisplayName(m_WeaponType);
}

std::string UnlockWeaponUpgrade::GetDescription() const {
    switch (m_WeaponType) {
        case WeaponType::ArcaneNova:
            return "Adds a radial burst weapon";
        case WeaponType::OrbitingShield:
            return "Adds orbiting guard orbs around the player";
        case WeaponType::MagicBolt:
            return "Adds an auto-targeting bolt weapon";
        default:
            return "Adds a new weapon";
    }
}

void UnlockWeaponUpgrade::Apply(Player &, WeaponInventory &weapons) const {
    weapons.UnlockWeapon(m_WeaponType);
}

WeaponLevelUpgrade::WeaponLevelUpgrade(WeaponType weaponType, int nextLevel,
                                       std::string description)
    : m_WeaponType(weaponType),
      m_NextLevel(nextLevel),
      m_Description(std::move(description)) {}

std::string WeaponLevelUpgrade::GetName() const {
    return GetWeaponTypeDisplayName(m_WeaponType) + " Lv." +
           std::to_string(m_NextLevel);
}

std::string WeaponLevelUpgrade::GetDescription() const {
    return m_Description;
}

UpgradeRarity WeaponLevelUpgrade::GetRarity() const {
    if (m_NextLevel >= 5) {
        return UpgradeRarity::Rare;
    }

    if (m_NextLevel >= 3) {
        return UpgradeRarity::Uncommon;
    }

    return UpgradeRarity::Common;
}

void WeaponLevelUpgrade::Apply(Player &, WeaponInventory &weapons) const {
    weapons.LevelUpWeapon(m_WeaponType);
}
