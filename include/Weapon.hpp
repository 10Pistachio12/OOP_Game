#ifndef WEAPON_HPP
#define WEAPON_HPP

#include "pch.hpp"

class Enemy;
class Player;
class Projectile;

enum class WeaponType {
    MagicBolt,
    ArcaneNova,
    OrbitingShield,
    RunicLance,
    StarfallNova,
};

inline std::string GetWeaponTypeDisplayName(WeaponType type) {
    switch (type) {
        case WeaponType::MagicBolt:
            return "Magic Bolt";
        case WeaponType::ArcaneNova:
            return "Arcane Nova";
        case WeaponType::OrbitingShield:
            return "Orbiting Shield";
        case WeaponType::RunicLance:
            return "Runic Lance";
        case WeaponType::StarfallNova:
            return "Starfall Nova";
        default:
            return "Unknown Weapon";
    }
}

class Weapon {
public:
    Weapon(WeaponType type, float cooldownSeconds, int maxLevel)
        : m_Type(type),
          m_CooldownSeconds(cooldownSeconds),
          m_MaxLevel(maxLevel) {}
    virtual ~Weapon() = default;

    virtual std::vector<std::shared_ptr<Projectile>> UpdateAndFire(
        const Player &owner,
        const std::vector<std::shared_ptr<Enemy>> &enemies,
        float deltaTimeSeconds) = 0;

    virtual std::string GetDisplayName() const = 0;
    virtual std::string GetLevelUpDescription() const = 0;
    virtual void ApplyLevelUp() = 0;
    virtual void IncreaseDamage(int amount) = 0;
    virtual void MultiplyCooldown(float multiplier) = 0;
    virtual void IncreaseProjectileCount(int amount) {
        m_BonusProjectileCount += amount;
    }
    virtual bool IsEvolved() const { return false; }

    bool LevelUp() {
        if (!CanLevelUp()) {
            return false;
        }

        ++m_Level;
        ApplyLevelUp();
        return true;
    }

    WeaponType GetType() const { return m_Type; }
    int GetLevel() const { return m_Level; }
    int GetMaxLevel() const { return m_MaxLevel; }
    bool CanLevelUp() const { return m_Level < m_MaxLevel; }
    float GetCooldownSeconds() const { return m_CooldownSeconds; }

protected:
    WeaponType m_Type;
    float m_CooldownSeconds = 0.0F;
    float m_CooldownRemaining = 0.0F;
    int m_Level = 1;
    int m_MaxLevel = 1;
    int m_BonusProjectileCount = 0;
};

#endif
