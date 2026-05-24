#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Character.hpp"

class Player : public Character {
public:
    explicit Player(const std::string &fontPath);

    int GainExperience(int amount);
    void UpdateMovement(float deltaTimeSeconds);

    int GetLevel() const { return m_Level; }
    int GetExperience() const { return m_Experience; }
    int GetExperienceToNextLevel() const { return m_ExperienceToNextLevel; }
    float GetPickupRadius() const { return m_PickupRadius; }
    void IncreasePickupRadius(float amount) { m_PickupRadius += amount; }

private:
    void LevelUp();

private:
    int m_Level = 1;
    int m_Experience = 0;
    int m_ExperienceToNextLevel = 5;
    float m_PickupRadius = 42.0F;
};

#endif
