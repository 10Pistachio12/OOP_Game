#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Character.hpp"

class Enemy : public Character {
public:
    virtual ~Enemy() = default;

    virtual void Update(float deltaTimeSeconds, const glm::vec2 &targetPosition);
    void TakeDamage(int amount) override;

    int GetContactDamage() const { return m_ContactDamage; }
    int GetExperienceValue() const { return m_ExperienceValue; }
    const std::string &GetEnemyName() const { return m_EnemyName; }
    bool IsElite() const { return m_IsElite; }

protected:
    Enemy(const std::string &fontPath, const std::string &glyph, int fontSize,
          const Util::Color &color, const glm::vec2 &spawnPosition,
          float radius, float moveSpeed, int hitPoints, int contactDamage,
          int experienceValue, std::string enemyName, bool isElite = false);
    Enemy(const std::string &imagePath, const glm::vec2 &scale,
          const glm::vec2 &spawnPosition, float radius, float moveSpeed,
          int hitPoints, int contactDamage, int experienceValue,
          std::string enemyName, bool isElite = false);

    void UpdateHitFlash(float deltaTimeSeconds);

private:
    int m_ContactDamage = 1;
    int m_ExperienceValue = 1;
    std::string m_EnemyName;
    bool m_IsElite = false;
    float m_HitFlashTimer = 0.0F;
};

#endif
