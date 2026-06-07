#include "PlayerMarker.hpp"

#include <cmath>

#include "SpriteAssets.hpp"

namespace {
constexpr float BASE_SCALE = 1.22F;
constexpr float PULSE_SCALE = 0.035F;
constexpr float PULSE_SPEED = 7.0F;
} // namespace

PlayerMarker::PlayerMarker(const std::string &fontPath)
    : GlyphObject(SpriteAssets::Path("player_marker.png"),
                  {BASE_SCALE, BASE_SCALE}, 12.0F, 0.0F) {
    (void)fontPath;
}

void PlayerMarker::Update(const glm::vec2 &playerPosition,
                          float deltaTimeSeconds) {
    m_Timer += deltaTimeSeconds;
    const float pulse = std::sin(m_Timer * PULSE_SPEED) * PULSE_SCALE;

    SetPosition(playerPosition);
    m_Transform.scale = {BASE_SCALE + pulse, BASE_SCALE + pulse};
}
