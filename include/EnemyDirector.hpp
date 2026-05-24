#ifndef ENEMY_DIRECTOR_HPP
#define ENEMY_DIRECTOR_HPP

#include "pch.hpp"

#include <random>

enum class EnemyType {
    Slime,
    Bat,
    Brute,
};

struct EnemySpawnRequest {
    EnemyType type = EnemyType::Slime;
    int count = 1;
    float difficultyScale = 1.0F;
};

class EnemyDirector {
public:
    explicit EnemyDirector(std::mt19937 &rng);

    EnemySpawnRequest CreateSpawnRequest(float survivalTimeSeconds);
    float GetSpawnInterval(float survivalTimeSeconds) const;
    std::string GetPhaseName(float survivalTimeSeconds) const;

private:
    EnemyType RollEnemyType(float survivalTimeSeconds);
    int GetBatchSize(float survivalTimeSeconds);
    float GetDifficultyScale(float survivalTimeSeconds) const;

private:
    std::mt19937 &m_Rng;
};

#endif
