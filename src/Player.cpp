#include "Player.hpp"

#include <glm/common.hpp>
#include <glm/geometric.hpp>

#include "SpriteAssets.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "config.hpp"

namespace {
constexpr float PLAYER_MARGIN = 36.0F;
constexpr float DAMAGE_INVULNERABILITY_SECONDS = 0.75F;
}

Player::Player(const std::string &fontPath)
    : Character(SpriteAssets::Path("player.png"), {1.1F, 1.1F}, 10.0F, 18.0F,
                260.0F, 8) {
    (void)fontPath;
}

void Player::Update(float deltaTimeSeconds) {
    UpdateMovement(deltaTimeSeconds);
    UpdateInvulnerability(deltaTimeSeconds);
}

void Player::UpdateMovement(float deltaTimeSeconds) {
    glm::vec2 direction(0.0F);

    if (Util::Input::IsKeyPressed(Util::Keycode::W) ||
        Util::Input::IsKeyPressed(Util::Keycode::UP)) {
        direction.y += 1.0F;
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::S) ||
        Util::Input::IsKeyPressed(Util::Keycode::DOWN)) {
        direction.y -= 1.0F;
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::A) ||
        Util::Input::IsKeyPressed(Util::Keycode::LEFT)) {
        direction.x -= 1.0F;
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::D) ||
        Util::Input::IsKeyPressed(Util::Keycode::RIGHT)) {
        direction.x += 1.0F;
    }

    if (glm::length(direction) > 0.0F) {
        direction = glm::normalize(direction);
    }

    m_Transform.translation += direction * m_MoveSpeed * deltaTimeSeconds;

    const float maxX = static_cast<float>(WINDOW_WIDTH) * 0.5F - PLAYER_MARGIN;
    const float maxY = static_cast<float>(WINDOW_HEIGHT) * 0.5F - PLAYER_MARGIN;
    m_Transform.translation.x = std::clamp(m_Transform.translation.x, -maxX, maxX);
    m_Transform.translation.y = std::clamp(m_Transform.translation.y, -maxY, maxY);
}

void Player::TakeDamage(int amount) {
    if (!CanTakeDamage()) {
        return;
    }

    Character::TakeDamage(amount);
    if (IsAlive()) {
        m_InvulnerabilityTimer = DAMAGE_INVULNERABILITY_SECONDS;
    }
}

int Player::GainExperience(int amount) {
    m_Experience += amount;

    int levelUps = 0;
    while (m_Experience >= m_ExperienceToNextLevel) {
        m_Experience -= m_ExperienceToNextLevel;
        LevelUp();
        ++levelUps;
    }

    return levelUps;
}

void Player::LevelUp() {
    ++m_Level;
    m_ExperienceToNextLevel += 3;
}

void Player::UpdateInvulnerability(float deltaTimeSeconds) {
    if (m_InvulnerabilityTimer <= 0.0F) {
        SetVisible(true);
        return;
    }

    m_InvulnerabilityTimer -= deltaTimeSeconds;
    if (m_InvulnerabilityTimer <= 0.0F) {
        SetVisible(true);
        return;
    }

    const int flashFrame = static_cast<int>(m_InvulnerabilityTimer * 50.0F);
    SetVisible(flashFrame % 2 == 0);
}
