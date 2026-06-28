#ifndef AEGIS_HALO_WEAPON_HPP
#define AEGIS_HALO_WEAPON_HPP

#include "Weapon.hpp"

class AegisHaloWeapon final : public Weapon {
public:
    explicit AegisHaloWeapon(const std::string &fontPath);

    std::vector<std::shared_ptr<Projectile>> UpdateAndFire(
        const Player &owner,
        const std::vector<std::shared_ptr<Enemy>> &enemies,
        float deltaTimeSeconds) override;

    std::string GetDisplayName() const override { return "Aegis Halo"; }
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
    int m_BaseOrbCount = 6;
    float m_InnerOrbitRadius = 48.0F;
    float m_OuterOrbitRadius = 78.0F;
    float m_AngularSpeed = 5.8F;
    float m_LifetimeSeconds = 4.2F;
    float m_AngleOffset = 0.0F;
};

#endif
