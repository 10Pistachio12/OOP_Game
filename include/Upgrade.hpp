#ifndef UPGRADE_HPP
#define UPGRADE_HPP

#include "pch.hpp"

class Player;
class Weapon;

class Upgrade {
public:
    virtual ~Upgrade() = default;

    virtual std::string GetName() const = 0;
    virtual std::string GetDescription() const = 0;
    virtual void Apply(Player &player, Weapon &weapon) const = 0;
};

class MoveSpeedUpgrade final : public Upgrade {
public:
    std::string GetName() const override { return "Swift Feet"; }
    std::string GetDescription() const override { return "+35 move speed"; }
    void Apply(Player &player, Weapon &weapon) const override;
};

class MaxHealthUpgrade final : public Upgrade {
public:
    std::string GetName() const override { return "Vital Core"; }
    std::string GetDescription() const override { return "+2 max HP and heal 2"; }
    void Apply(Player &player, Weapon &weapon) const override;
};

class PickupRangeUpgrade final : public Upgrade {
public:
    std::string GetName() const override { return "Magnet Pulse"; }
    std::string GetDescription() const override { return "+28 pickup range"; }
    void Apply(Player &player, Weapon &weapon) const override;
};

class WeaponDamageUpgrade final : public Upgrade {
public:
    std::string GetName() const override { return "Sharper Bolt"; }
    std::string GetDescription() const override { return "+1 weapon damage"; }
    void Apply(Player &player, Weapon &weapon) const override;
};

class WeaponCooldownUpgrade final : public Upgrade {
public:
    std::string GetName() const override { return "Quick Cast"; }
    std::string GetDescription() const override { return "-14% weapon cooldown"; }
    void Apply(Player &player, Weapon &weapon) const override;
};

#endif
