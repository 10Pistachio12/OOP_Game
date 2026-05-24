#ifndef WEAPON_INVENTORY_HPP
#define WEAPON_INVENTORY_HPP

#include "pch.hpp"

#include "Weapon.hpp"

class Enemy;
class Player;
class Projectile;

class WeaponInventory {
public:
    explicit WeaponInventory(std::string fontPath);

    void AddWeapon(std::unique_ptr<Weapon> weapon);
    bool UnlockWeapon(WeaponType type);
    bool LevelUpWeapon(WeaponType type);
    bool HasWeapon(WeaponType type) const;
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

    std::string GetDisplayNames() const;
    std::size_t GetWeaponCount() const { return m_Weapons.size(); }

private:
    Weapon *FindWeapon(WeaponType type);
    const Weapon *FindWeapon(WeaponType type) const;
    std::unique_ptr<Weapon> CreateWeapon(WeaponType type) const;

private:
    std::string m_FontPath;
    std::vector<std::unique_ptr<Weapon>> m_Weapons;
};

#endif
