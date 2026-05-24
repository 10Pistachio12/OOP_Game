#include "GlyphObject.hpp"

GlyphObject::GlyphObject(const std::string &fontPath, const std::string &glyph,
                         int fontSize, const Util::Color &color, float zIndex,
                         float radius)
    : m_Text(std::make_shared<Util::Text>(fontPath, fontSize, glyph, color)),
      m_Radius(radius) {
    SetDrawable(m_Text);
    SetZIndex(zIndex);
}
