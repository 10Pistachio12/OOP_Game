#ifndef ORBITING_SHIELD_WEAPON_HPP
#define ORBITING_SHIELD_WEAPON_HPP

#include "Weapon.hpp"

class OrbitingShieldWeapon final : public Weapon {
public:
    explicit OrbitingShieldWeapon(const std::string &fontPath);

    std::vector<std::shared_ptr<Projectile>> UpdateAndFire(
        const Player &owner,
        const std::vector<std::shared_ptr<Enemy>> &enemies,
        float deltaTimeSeconds) override;

    std::string GetDisplayName() const override { return "Orbiting Shield"; }
    std::string GetLevelUpDescription() const override;
    void ApplyLevelUp() override;
    void IncreaseDamage(int amount) override;
    void MultiplyCooldown(float multiplier) override;

private:
    bool HasLiveTarget(const std::vector<std::shared_ptr<Enemy>> &enemies) const;

private:
    std::string m_FontPath;
    int m_Damage = 1;
    int m_OrbCount = 2;
    float m_OrbitRadius = 54.0F;
    float m_AngularSpeed = 4.6F;
    float m_LifetimeSeconds = 2.6F;
    float m_AngleOffset = 0.0F;
};

#endif
