#ifndef STARFALL_NOVA_WEAPON_HPP
#define STARFALL_NOVA_WEAPON_HPP

#include "Weapon.hpp"

class StarfallNovaWeapon final : public Weapon {
public:
    explicit StarfallNovaWeapon(const std::string &fontPath);

    std::vector<std::shared_ptr<Projectile>> UpdateAndFire(
        const Player &owner,
        const std::vector<std::shared_ptr<Enemy>> &enemies,
        float deltaTimeSeconds) override;

    std::string GetDisplayName() const override { return "Starfall Nova"; }
    std::string GetLevelUpDescription() const override {
        return "Evolution complete";
    }
    void ApplyLevelUp() override {}
    void IncreaseDamage(int amount) override;
    void MultiplyCooldown(float multiplier) override;
    void IncreaseProjectileCount(int amount) override;
    bool IsEvolved() const override { return true; }

private:
    bool HasLiveTarget(const std::vector<std::shared_ptr<Enemy>> &enemies) const;

private:
    std::string m_FontPath;
    int m_Damage = 4;
    int m_BaseProjectilesPerBurst = 16;
    float m_ProjectileSpeed = 340.0F;
    float m_LifetimeSeconds = 1.55F;
    float m_BurstAngleOffset = 0.0F;
};

#endif
