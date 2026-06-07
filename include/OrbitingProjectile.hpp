#ifndef ORBITING_PROJECTILE_HPP
#define ORBITING_PROJECTILE_HPP

#include "Projectile.hpp"

class Player;

class OrbitingProjectile final : public Projectile {
public:
    OrbitingProjectile(const std::string &fontPath, const Player &owner,
                       float initialAngle, float orbitRadius,
                       float angularSpeed, int damage,
                       float lifetimeSeconds);

    void Update(float deltaTimeSeconds) override;
    void OnHitEnemy(Enemy &enemy) override;
    bool CanDamageEnemy(const Enemy &enemy) const override;

private:
    const Player *m_Owner = nullptr;
    std::unordered_map<const Enemy *, float> m_EnemyHitCooldowns;
    float m_Angle = 0.0F;
    float m_OrbitRadius = 0.0F;
    float m_AngularSpeed = 0.0F;
    float m_RemainingLifetime = 0.0F;
};

#endif
