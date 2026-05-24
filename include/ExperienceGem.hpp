#ifndef EXPERIENCE_GEM_HPP
#define EXPERIENCE_GEM_HPP

#include "GlyphObject.hpp"

class ExperienceGem : public GlyphObject {
public:
    ExperienceGem(const std::string &fontPath, const glm::vec2 &spawnPosition,
                  int value);

    int GetValue() const { return m_Value; }

private:
    int m_Value = 1;
};

#endif
