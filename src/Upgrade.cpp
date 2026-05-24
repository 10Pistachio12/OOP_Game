#include "Upgrade.hpp"

#include "Player.hpp"
#include "WeaponInventory.hpp"

void MoveSpeedUpgrade::Apply(Player &player, WeaponInventory &) const {
    player.IncreaseMoveSpeed(35.0F);
}

void MaxHealthUpgrade::Apply(Player &player, WeaponInventory &) const {
    player.IncreaseMaxHitPoints(2);
    player.Heal(2);
}

void PickupRangeUpgrade::Apply(Player &player, WeaponInventory &) const {
    player.IncreasePickupRadius(28.0F);
}

void WeaponDamageUpgrade::Apply(Player &, WeaponInventory &weapons) const {
    weapons.IncreaseDamage(1);
}

void WeaponCooldownUpgrade::Apply(Player &, WeaponInventory &weapons) const {
    weapons.MultiplyCooldown(0.86F);
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

void WeaponLevelUpgrade::Apply(Player &, WeaponInventory &weapons) const {
    weapons.LevelUpWeapon(m_WeaponType);
}
