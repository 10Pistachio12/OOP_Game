#include "GlyphObject.hpp"

GlyphObject::GlyphObject(const std::string &fontPath, const std::string &glyph,
                         int fontSize, const Util::Color &color, float zIndex,
                         float radius)
    : m_Text(std::make_shared<Util::Text>(fontPath, fontSize, glyph, color)),
      m_Radius(radius) {
    SetDrawable(m_Text);
    SetZIndex(zIndex);
}

GlyphObject::GlyphObject(const std::string &imagePath, const glm::vec2 &scale,
                         float zIndex, float radius)
    : m_Image(std::make_shared<Util::Image>(imagePath)),
      m_Radius(radius) {
    m_Transform.scale = scale;
    SetDrawable(m_Image);
    SetZIndex(zIndex);
}
