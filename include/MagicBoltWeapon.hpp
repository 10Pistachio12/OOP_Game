#ifndef MAGIC_BOLT_WEAPON_HPP
#define MAGIC_BOLT_WEAPON_HPP

#include "Weapon.hpp"

class MagicBoltWeapon : public Weapon {
public:
    explicit MagicBoltWeapon(const std::string &fontPath);

    std::vector<std::shared_ptr<Projectile>> UpdateAndFire(
        const Player &owner,
        const std::vector<std::shared_ptr<Enemy>> &enemies,
        float deltaTimeSeconds) override;

    void OnOwnerLevelUp() override;
    std::string GetDisplayName() const override { return "Magic Bolt"; }
    void IncreaseDamage(int amount) override;
    void MultiplyCooldown(float multiplier) override;

    int GetDamage() const { return m_Damage; }

private:
    std::string m_FontPath;
    int m_Damage = 1;
    float m_ProjectileSpeed = 420.0F;
    float m_LifetimeSeconds = 1.5F;
};

#endif
