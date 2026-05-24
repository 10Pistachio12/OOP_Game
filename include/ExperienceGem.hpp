#ifndef EXPERIENCE_GEM_HPP
#define EXPERIENCE_GEM_HPP

#include "GlyphObject.hpp"

class ExperienceGem : public GlyphObject {
public:
    ExperienceGem(const std::string &fontPath, const glm::vec2 &spawnPosition,
                  int value);

    void Update(float deltaTimeSeconds, const glm::vec2 &targetPosition,
                float attractionRadius);

    int GetValue() const { return m_Value; }

private:
    int m_Value = 1;
    float m_AttractionSpeed = 250.0F;
};

#endif
