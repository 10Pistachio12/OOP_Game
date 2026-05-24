#include "BatEnemy.hpp"

#include <cmath>

#include <glm/geometric.hpp>

BatEnemy::BatEnemy(const std::string &fontPath, const glm::vec2 &spawnPosition,
                   float difficultyScale)
    : Enemy(fontPath, "b", 24, Util::Color(255, 190, 85), spawnPosition, 13.0F,
            135.0F + difficultyScale * 24.0F,
            1 + static_cast<int>(difficultyScale * 0.45F), 1,
            1 + static_cast<int>(difficultyScale * 0.25F), "Bat") {}

void BatEnemy::Update(float deltaTimeSeconds, const glm::vec2 &targetPosition) {
    m_DriftTimer += deltaTimeSeconds;

    glm::vec2 direction = targetPosition - GetPosition();
    if (glm::length(direction) == 0.0F) {
        return;
    }

    direction = glm::normalize(direction);
    const glm::vec2 perpendicular{-direction.y, direction.x};
    const float drift = std::sin(m_DriftTimer * 5.5F) * 55.0F;

    m_Transform.translation +=
        (direction * m_MoveSpeed + perpendicular * drift) * deltaTimeSeconds;
}
