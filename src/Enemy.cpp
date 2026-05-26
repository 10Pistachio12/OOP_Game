#include "Enemy.hpp"

#include <glm/geometric.hpp>

Enemy::Enemy(const std::string &fontPath, const std::string &glyph, int fontSize,
             const Util::Color &color, const glm::vec2 &spawnPosition,
             float radius, float moveSpeed, int hitPoints, int contactDamage,
             int experienceValue, std::string enemyName, bool isElite)
    : Character(fontPath, glyph, fontSize, color, 5.0F, radius, moveSpeed,
                hitPoints),
      m_ContactDamage(contactDamage),
      m_ExperienceValue(experienceValue),
      m_EnemyName(std::move(enemyName)),
      m_IsElite(isElite) {
    SetPosition(spawnPosition);
}

Enemy::Enemy(const std::string &imagePath, const glm::vec2 &scale,
             const glm::vec2 &spawnPosition, float radius, float moveSpeed,
             int hitPoints, int contactDamage, int experienceValue,
             std::string enemyName, bool isElite)
    : Character(imagePath, scale, 5.0F, radius, moveSpeed, hitPoints),
      m_ContactDamage(contactDamage),
      m_ExperienceValue(experienceValue),
      m_EnemyName(std::move(enemyName)),
      m_IsElite(isElite) {
    SetPosition(spawnPosition);
}

void Enemy::Update(float deltaTimeSeconds, const glm::vec2 &targetPosition) {
    UpdateHitFlash(deltaTimeSeconds);

    glm::vec2 direction = targetPosition - GetPosition();
    if (glm::length(direction) > 0.0F) {
        direction = glm::normalize(direction);
    }

    m_Transform.translation += direction * m_MoveSpeed * deltaTimeSeconds;
}

void Enemy::TakeDamage(int amount) {
    Character::TakeDamage(amount);

    if (IsAlive()) {
        m_HitFlashTimer = 0.14F;
    }
}

void Enemy::UpdateHitFlash(float deltaTimeSeconds) {
    if (m_HitFlashTimer <= 0.0F) {
        SetVisible(true);
        return;
    }

    m_HitFlashTimer -= deltaTimeSeconds;
    if (m_HitFlashTimer <= 0.0F) {
        SetVisible(true);
        return;
    }

    const int flashFrame = static_cast<int>(m_HitFlashTimer * 60.0F);
    SetVisible(flashFrame % 2 == 0);
}
