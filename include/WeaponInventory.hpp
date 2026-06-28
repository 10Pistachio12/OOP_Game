#ifndef WEAPON_INVENTORY_HPP
#define WEAPON_INVENTORY_HPP

#include "pch.hpp"

#include <random>

#include "Weapon.hpp"

class Enemy;
class Player;
class Projectile;
class UpgradeManager;

class WeaponInventory {
public:
    explicit WeaponInventory(std::string fontPath);

    void AddWeapon(std::unique_ptr<Weapon> weapon);
    bool UnlockWeapon(WeaponType type);
    bool LevelUpWeapon(WeaponType type);
    std::string LevelUpRandomWeapon(std::mt19937 &rng);
    std::string EvolveRandomWeapon(std::mt19937 &rng,
                                   const UpgradeManager &upgrades);
    bool LevelUpAllWeapons();
    bool HasWeapon(WeaponType type) const;
    bool HasOpenWeaponSlot() const;
    bool CanLevelUpWeapon(WeaponType type) const;
    int GetWeaponLevel(WeaponType type) const;
    int GetWeaponMaxLevel(WeaponType type) const;
    std::string GetWeaponLevelUpDescription(WeaponType type) const;
    std::vector<WeaponType> GetOwnedWeaponTypes() const;

    std::vector<std::shared_ptr<Projectile>> UpdateAndFire(
        const Player &owner,
        const std::vector<std::shared_ptr<Enemy>> &enemies,
        float deltaTimeSeconds);

    void IncreaseDamage(int amount);
    void MultiplyCooldown(float multiplier);
    void IncreaseProjectileCount(int amount);

    std::string GetDisplayNames() const;
    std::string GetEvolutionStatus(const UpgradeManager &upgrades) const;
    std::size_t GetWeaponCount() const { return m_Weapons.size(); }
    std::size_t GetMaxWeaponSlots() const { return MAX_WEAPON_SLOTS; }

private:
    static constexpr std::size_t MAX_WEAPON_SLOTS = 6;

    void ApplyWeaponPassiveHistory(Weapon &weapon,
                                   const UpgradeManager &upgrades) const;
    bool CanEvolveArcaneNova(const UpgradeManager &upgrades) const;
    bool CanEvolveMagicBolt(const UpgradeManager &upgrades) const;
    bool CanEvolveOrbitingShield(const UpgradeManager &upgrades) const;
    bool ReplaceWeapon(WeaponType originalType,
                       std::unique_ptr<Weapon> evolvedWeapon);
    Weapon *FindWeapon(WeaponType type);
    const Weapon *FindWeapon(WeaponType type) const;
    std::unique_ptr<Weapon> CreateWeapon(WeaponType type) const;

private:
    std::string m_FontPath;
    std::vector<std::unique_ptr<Weapon>> m_Weapons;
};

#endif
