#ifndef PLAYER_MARKER_HPP
#define PLAYER_MARKER_HPP

#include "GlyphObject.hpp"

class PlayerMarker : public GlyphObject {
public:
    explicit PlayerMarker(const std::string &fontPath);

    void Update(const glm::vec2 &playerPosition, float deltaTimeSeconds);

private:
    float m_Timer = 0.0F;
};

#endif
