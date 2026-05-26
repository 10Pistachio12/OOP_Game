#include "FloatingText.hpp"

FloatingText::FloatingText(const std::string &fontPath,
                           const std::string &content,
                           const Util::Color &color,
                           const glm::vec2 &position,
                           float lifetimeSeconds)
    : GlyphObject(fontPath, content, 18, color, 20.0F, 0.0F),
      m_RemainingLifetime(lifetimeSeconds) {
    SetPosition(position);
}

void FloatingText::Update(float deltaTimeSeconds) {
    m_RemainingLifetime -= deltaTimeSeconds;
    m_Transform.translation.y += m_RiseSpeed * deltaTimeSeconds;

    if (m_RemainingLifetime <= 0.0F) {
        Destroy();
    }
}
