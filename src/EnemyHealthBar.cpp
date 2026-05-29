#include "EnemyHealthBar.hpp"

#include <cmath>

#include <glm/common.hpp>

#include "SpriteAssets.hpp"

namespace {
constexpr int HEALTH_BAR_STEPS = 10;
constexpr int LARGE_ENEMY_HEALTH_THRESHOLD = 7;
constexpr float BAR_VERTICAL_OFFSET = 14.0F;

bool ShouldShowHealthBar(const Enemy &enemy) {
    return enemy.IsElite() ||
           enemy.GetMaxHitPoints() >= LARGE_ENEMY_HEALTH_THRESHOLD ||
           enemy.GetHitPoints() < enemy.GetMaxHitPoints();
}
} // namespace

EnemyHealthBar::EnemyHealthBar()
    : GlyphObject(SpriteAssets::Path("health_bar_10.png"), {1.0F, 1.0F},
                  16.0F, 0.0F) {}

void EnemyHealthBar::Update(const Enemy &enemy) {
    if (!enemy.IsAlive()) {
        Destroy();
        return;
    }

    SetVisible(ShouldShowHealthBar(enemy));
    SetPosition(enemy.GetPosition() +
                glm::vec2(0.0F, enemy.GetRadius() + BAR_VERTICAL_OFFSET));

    const float healthRatio =
        static_cast<float>(enemy.GetHitPoints()) /
        static_cast<float>(enemy.GetMaxHitPoints());
    const int healthStep = static_cast<int>(
        std::round(glm::clamp(healthRatio, 0.0F, 1.0F) * HEALTH_BAR_STEPS));
    SetHealthStep(healthStep);
}

void EnemyHealthBar::SetHealthStep(int step) {
    if (step == m_LastHealthStep) {
        return;
    }

    m_LastHealthStep = step;
    m_Image->SetImage(SpriteAssets::Path("health_bar_" +
                                         std::to_string(step) + ".png"));
}
