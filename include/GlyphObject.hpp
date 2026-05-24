#ifndef GLYPH_OBJECT_HPP
#define GLYPH_OBJECT_HPP

#include "pch.hpp"

#include "Util/GameObject.hpp"
#include "Util/Text.hpp"

class GlyphObject : public Util::GameObject {
public:
    GlyphObject(const std::string &fontPath, const std::string &glyph,
                int fontSize, const Util::Color &color, float zIndex,
                float radius);

    glm::vec2 GetPosition() const { return m_Transform.translation; }
    float GetRadius() const { return m_Radius; }
    bool IsAlive() const { return m_IsAlive; }

    void SetPosition(const glm::vec2 &position) { m_Transform.translation = position; }
    void Destroy() { m_IsAlive = false; }

protected:
    std::shared_ptr<Util::Text> m_Text;
    float m_Radius = 0.0F;
    bool m_IsAlive = true;
};

#endif
