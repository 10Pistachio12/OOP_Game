#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "GlyphObject.hpp"

class Character : public GlyphObject {
public:
    Character(const std::string &fontPath, const std::string &glyph,
              int fontSize, const Util::Color &color, float zIndex,
              float radius, float moveSpeed, int hitPoints)
        : GlyphObject(fontPath, glyph, fontSize, color, zIndex, radius),
          m_MoveSpeed(moveSpeed),
          m_HitPoints(hitPoints),
          m_MaxHitPoints(hitPoints) {}

    Character(const std::string &imagePath, const glm::vec2 &scale,
              float zIndex, float radius, float moveSpeed, int hitPoints)
        : GlyphObject(imagePath, scale, zIndex, radius),
          m_MoveSpeed(moveSpeed),
          m_HitPoints(hitPoints),
          m_MaxHitPoints(hitPoints) {}

    float GetMoveSpeed() const { return m_MoveSpeed; }
    int GetHitPoints() const { return m_HitPoints; }
    int GetMaxHitPoints() const { return m_MaxHitPoints; }

    void Heal(int amount) {
        m_HitPoints = std::min(m_MaxHitPoints, m_HitPoints + amount);
    }

    void IncreaseMaxHitPoints(int amount) {
        m_MaxHitPoints += amount;
        m_HitPoints = std::min(m_MaxHitPoints, m_HitPoints + amount);
    }

    void IncreaseMoveSpeed(float amount) { m_MoveSpeed += amount; }

    virtual void TakeDamage(int amount) {
        m_HitPoints = std::max(0, m_HitPoints - amount);
        if (m_HitPoints == 0) {
            Destroy();
        }
    }

protected:
    float m_MoveSpeed = 0.0F;
    int m_HitPoints = 0;
    int m_MaxHitPoints = 0;
};

#endif
