#ifndef FLOATING_TEXT_HPP
#define FLOATING_TEXT_HPP

#include "GlyphObject.hpp"

class FloatingText : public GlyphObject {
public:
    FloatingText(const std::string &fontPath, const std::string &content,
                 const Util::Color &color, const glm::vec2 &position,
                 float lifetimeSeconds = 0.75F);

    void Update(float deltaTimeSeconds);

private:
    float m_RemainingLifetime = 0.0F;
    float m_RiseSpeed = 42.0F;
};

#endif
