#ifndef WEAPON_INVENTORY_HPP
#define WEAPON_INVENTORY_HPP

#include "pch.hpp"

#include "Weapon.hpp"

class Enemy;
class Player;
class Projectile;

class WeaponInventory {
public:
    void AddWeapon(std::unique_ptr<Weapon> weapon);

    std::vector<std::shared_ptr<Projectile>> UpdateAndFire(
        const Player &owner,
        const std::vector<std::shared_ptr<Enemy>> &enemies,
        float deltaTimeSeconds);

    void IncreaseDamage(int amount);
    void MultiplyCooldown(float multiplier);
    void OnOwnerLevelUp();

    std::string GetDisplayNames() const;
    std::size_t GetWeaponCount() const { return m_Weapons.size(); }

private:
    std::vector<std::unique_ptr<Weapon>> m_Weapons;
};

#endif
