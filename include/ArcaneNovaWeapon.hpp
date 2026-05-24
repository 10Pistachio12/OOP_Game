#ifndef ARCANE_NOVA_WEAPON_HPP
#define ARCANE_NOVA_WEAPON_HPP

#include "Weapon.hpp"

class ArcaneNovaWeapon : public Weapon {
public:
    explicit ArcaneNovaWeapon(const std::string &fontPath);

    std::vector<std::shared_ptr<Projectile>> UpdateAndFire(
        const Player &owner,
        const std::vector<std::shared_ptr<Enemy>> &enemies,
        float deltaTimeSeconds) override;

    void OnOwnerLevelUp() override;
    std::string GetDisplayName() const override { return "Arcane Nova"; }
    void IncreaseDamage(int amount) override;
    void MultiplyCooldown(float multiplier) override;

private:
    bool HasLiveTarget(const std::vector<std::shared_ptr<Enemy>> &enemies) const;

private:
    std::string m_FontPath;
    int m_Damage = 1;
    int m_ProjectilesPerBurst = 8;
    float m_ProjectileSpeed = 260.0F;
    float m_LifetimeSeconds = 1.1F;
    float m_BurstAngleOffset = 0.0F;
};

#endif
