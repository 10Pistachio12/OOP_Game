#include "ExperienceGem.hpp"

ExperienceGem::ExperienceGem(const std::string &fontPath,
                             const glm::vec2 &spawnPosition, int value)
    : GlyphObject(fontPath, "+", 18, Util::Color(80, 215, 255), 4.0F, 12.0F),
      m_Value(value) {
    SetPosition(spawnPosition);
}
