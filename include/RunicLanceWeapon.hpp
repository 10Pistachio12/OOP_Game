#ifndef RUNIC_LANCE_WEAPON_HPP
#define RUNIC_LANCE_WEAPON_HPP

#include "Weapon.hpp"

class RunicLanceWeapon final : public Weapon {
public:
    explicit RunicLanceWeapon(const std::string &fontPath);

    std::vector<std::shared_ptr<Projectile>> UpdateAndFire(
        const Player &owner,
        const std::vector<std::shared_ptr<Enemy>> &enemies,
        float deltaTimeSeconds) override;

    std::string GetDisplayName() const override { return "Runic Lance"; }
    std::string GetLevelUpDescription() const override {
        return "Evolution complete";
    }
    void ApplyLevelUp() override {}
    void IncreaseDamage(int amount) override;
    void MultiplyCooldown(float multiplier) override;
    void IncreaseProjectileCount(int amount) override;
    bool IsEvolved() const override { return true; }

private:
    std::string m_FontPath;
    int m_Damage = 5;
    int m_BaseProjectileCount = 3;
    float m_ProjectileSpeed = 620.0F;
    float m_LifetimeSeconds = 1.65F;
};

#endif
