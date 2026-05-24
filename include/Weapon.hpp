#ifndef WEAPON_HPP
#define WEAPON_HPP

#include "pch.hpp"

class Enemy;
class Player;
class Projectile;

class Weapon {
public:
    explicit Weapon(float cooldownSeconds)
        : m_CooldownSeconds(cooldownSeconds) {}
    virtual ~Weapon() = default;

    virtual std::vector<std::shared_ptr<Projectile>> UpdateAndFire(
        const Player &owner,
        const std::vector<std::shared_ptr<Enemy>> &enemies,
        float deltaTimeSeconds) = 0;

    virtual void OnOwnerLevelUp() = 0;
    virtual std::string GetDisplayName() const = 0;
    virtual void IncreaseDamage(int amount) = 0;
    virtual void MultiplyCooldown(float multiplier) = 0;

    float GetCooldownSeconds() const { return m_CooldownSeconds; }

protected:
    float m_CooldownSeconds = 0.0F;
    float m_CooldownRemaining = 0.0F;
};

#endif
