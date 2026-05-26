#ifndef UPGRADE_HPP
#define UPGRADE_HPP

#include "pch.hpp"

#include "Weapon.hpp"

class Player;
class WeaponInventory;

class Upgrade {
public:
    virtual ~Upgrade() = default;

    virtual std::string GetName() const = 0;
    virtual std::string GetDescription() const = 0;
    virtual void Apply(Player &player, WeaponInventory &weapons) const = 0;
};

class MoveSpeedUpgrade final : public Upgrade {
public:
    std::string GetName() const override { return "Swift Feet"; }
    std::string GetDescription() const override { return "+35 move speed"; }
    void Apply(Player &player, WeaponInventory &weapons) const override;
};

class MaxHealthUpgrade final : public Upgrade {
public:
    std::string GetName() const override { return "Vital Core"; }
    std::string GetDescription() const override { return "+2 max HP and heal 2"; }
    void Apply(Player &player, WeaponInventory &weapons) const override;
};

class PickupRangeUpgrade final : public Upgrade {
public:
    std::string GetName() const override { return "Magnet Pulse"; }
    std::string GetDescription() const override { return "+28 pickup range"; }
    void Apply(Player &player, WeaponInventory &weapons) const override;
};

class WeaponDamageUpgrade final : public Upgrade {
public:
    std::string GetName() const override { return "Sharper Arsenal"; }
    std::string GetDescription() const override { return "+1 damage to all weapons"; }
    void Apply(Player &player, WeaponInventory &weapons) const override;
};

class WeaponCooldownUpgrade final : public Upgrade {
public:
    std::string GetName() const override { return "Quick Cast"; }
    std::string GetDescription() const override { return "-14% cooldown on all weapons"; }
    void Apply(Player &player, WeaponInventory &weapons) const override;
};

class ProjectileCountUpgrade final : public Upgrade {
public:
    std::string GetName() const override { return "Twin Casting"; }
    std::string GetDescription() const override { return "+1 projectile pattern to all weapons"; }
    void Apply(Player &player, WeaponInventory &weapons) const override;
};

class UnlockWeaponUpgrade final : public Upgrade {
public:
    explicit UnlockWeaponUpgrade(WeaponType weaponType);

    std::string GetName() const override;
    std::string GetDescription() const override;
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
    void Apply(Player &player, WeaponInventory &weapons) const override;

private:
    WeaponType m_WeaponType;
    int m_NextLevel = 1;
    std::string m_Description;
};

#endif
