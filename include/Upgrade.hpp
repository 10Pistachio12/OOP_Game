#ifndef UPGRADE_HPP
#define UPGRADE_HPP

#include "pch.hpp"

#include "Weapon.hpp"

class Player;
class WeaponInventory;

enum class UpgradeRarity {
    Common,
    Uncommon,
    Rare,
};

enum class PassiveUpgradeType {
    None = 0,
    MoveSpeed,
    MaxHealth,
    PickupRange,
    WeaponDamage,
    WeaponCooldown,
    ProjectileCount,
    Count,
};

std::string GetUpgradeRarityLabel(UpgradeRarity rarity);
int GetPassiveUpgradeMaxLevel(PassiveUpgradeType type);

class Upgrade {
public:
    virtual ~Upgrade() = default;

    virtual std::string GetName() const = 0;
    virtual std::string GetDescription() const = 0;
    virtual UpgradeRarity GetRarity() const { return UpgradeRarity::Common; }
    virtual PassiveUpgradeType GetPassiveType() const {
        return PassiveUpgradeType::None;
    }
    virtual void Apply(Player &player, WeaponInventory &weapons) const = 0;
};

class PassiveUpgrade : public Upgrade {
public:
    PassiveUpgrade(PassiveUpgradeType type, int nextLevel, int maxLevel);

    PassiveUpgradeType GetPassiveType() const override { return m_Type; }

protected:
    std::string FormatLeveledName(const std::string &baseName) const;

private:
    PassiveUpgradeType m_Type = PassiveUpgradeType::None;
    int m_NextLevel = 1;
    int m_MaxLevel = 1;
};

class MoveSpeedUpgrade final : public PassiveUpgrade {
public:
    MoveSpeedUpgrade(int nextLevel, int maxLevel);

    std::string GetName() const override;
    std::string GetDescription() const override;
    void Apply(Player &player, WeaponInventory &weapons) const override;
};

class MaxHealthUpgrade final : public PassiveUpgrade {
public:
    MaxHealthUpgrade(int nextLevel, int maxLevel);

    std::string GetName() const override;
    std::string GetDescription() const override;
    void Apply(Player &player, WeaponInventory &weapons) const override;
};

class PickupRangeUpgrade final : public PassiveUpgrade {
public:
    PickupRangeUpgrade(int nextLevel, int maxLevel);

    std::string GetName() const override;
    std::string GetDescription() const override;
    void Apply(Player &player, WeaponInventory &weapons) const override;
};

class WeaponDamageUpgrade final : public PassiveUpgrade {
public:
    WeaponDamageUpgrade(int nextLevel, int maxLevel);

    std::string GetName() const override;
    std::string GetDescription() const override;
    UpgradeRarity GetRarity() const override { return UpgradeRarity::Uncommon; }
    void Apply(Player &player, WeaponInventory &weapons) const override;
};

class WeaponCooldownUpgrade final : public PassiveUpgrade {
public:
    WeaponCooldownUpgrade(int nextLevel, int maxLevel);

    std::string GetName() const override;
    std::string GetDescription() const override;
    UpgradeRarity GetRarity() const override { return UpgradeRarity::Uncommon; }
    void Apply(Player &player, WeaponInventory &weapons) const override;
};

class ProjectileCountUpgrade final : public PassiveUpgrade {
public:
    ProjectileCountUpgrade(int nextLevel, int maxLevel);

    std::string GetName() const override;
    std::string GetDescription() const override;
    UpgradeRarity GetRarity() const override { return UpgradeRarity::Rare; }
    void Apply(Player &player, WeaponInventory &weapons) const override;
};

class OverflowRecoveryUpgrade final : public Upgrade {
public:
    std::string GetName() const override { return "Second Wind"; }
    std::string GetDescription() const override {
        return "+4 HP when primary upgrades are capped";
    }
    void Apply(Player &player, WeaponInventory &weapons) const override;
};

class UnlockWeaponUpgrade final : public Upgrade {
public:
    explicit UnlockWeaponUpgrade(WeaponType weaponType);

    std::string GetName() const override;
    std::string GetDescription() const override;
    UpgradeRarity GetRarity() const override { return UpgradeRarity::Rare; }
    void Apply(Player &player, WeaponInventory &weapons) const override;

private:
    WeaponType m_WeaponType;
};

class WeaponLevelUpgrade final : public Upgrade {
public:
    WeaponLevelUpgrade(WeaponType weaponType, int nextLevel,
                       std::string description);

    std::string GetName() const override;
    std::string GetDescription() const override;
    UpgradeRarity GetRarity() const override;
    void Apply(Player &player, WeaponInventory &weapons) const override;

private:
    WeaponType m_WeaponType;
    int m_NextLevel = 1;
    std::string m_Description;
};

#endif
