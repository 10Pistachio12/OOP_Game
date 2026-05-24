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

void Enemy::Update(float deltaTimeSeconds, const glm::vec2 &targetPosition) {
    glm::vec2 direction = targetPosition - GetPosition();
    if (glm::length(direction) > 0.0F) {
        direction = glm::normalize(direction);
    }

    m_Transform.translation += direction * m_MoveSpeed * deltaTimeSeconds;
}
