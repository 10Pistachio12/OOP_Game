#include "EnemyDirector.hpp"

#include <algorithm>

namespace {
int RollPercent(std::mt19937 &rng) {
    return std::uniform_int_distribution<int>(0, 99)(rng);
}
} // namespace

EnemyDirector::EnemyDirector(std::mt19937 &rng)
    : m_Rng(rng) {}

EnemySpawnRequest EnemyDirector::CreateSpawnRequest(float survivalTimeSeconds) {
    return EnemySpawnRequest{
        RollEnemyType(survivalTimeSeconds),
        GetBatchSize(survivalTimeSeconds),
        GetDifficultyScale(survivalTimeSeconds),
    };
}

EnemySpawnRequest EnemyDirector::CreateEliteSpawnRequest(
    float survivalTimeSeconds) {
    return EnemySpawnRequest{
        EnemyType::Elite,
        survivalTimeSeconds >= 90.0F ? 2 : 1,
        GetDifficultyScale(survivalTimeSeconds) * 1.2F,
    };
}

float EnemyDirector::GetSpawnInterval(float survivalTimeSeconds) const {
    const float interval = 1.15F - survivalTimeSeconds * 0.015F;
    return std::max(0.24F, interval);
}

float EnemyDirector::GetNextEliteSpawnTime(int eliteSpawnsCompleted) const {
    return 25.0F + static_cast<float>(eliteSpawnsCompleted) * 30.0F;
}

std::string EnemyDirector::GetPhaseName(float survivalTimeSeconds) const {
    if (survivalTimeSeconds < 12.0F) {
        return "Opening Swarm";
    }
    if (survivalTimeSeconds < 32.0F) {
        return "Bat Invasion";
    }
    if (survivalTimeSeconds < 60.0F) {
        return "Heavy Horde";
    }
    return "Endurance";
}

EnemyType EnemyDirector::RollEnemyType(float survivalTimeSeconds) {
    const int roll = RollPercent(m_Rng);

    if (survivalTimeSeconds < 12.0F) {
        return EnemyType::Slime;
    }
    if (survivalTimeSeconds < 32.0F) {
        return roll < 35 ? EnemyType::Bat : EnemyType::Slime;
    }
    if (survivalTimeSeconds < 60.0F) {
        if (roll < 12) {
            return EnemyType::Brute;
        }
        return roll < 52 ? EnemyType::Bat : EnemyType::Slime;
    }

    if (roll < 22) {
        return EnemyType::Brute;
    }
    return roll < 58 ? EnemyType::Bat : EnemyType::Slime;
}

int EnemyDirector::GetBatchSize(float survivalTimeSeconds) {
    const int roll = RollPercent(m_Rng);

    if (survivalTimeSeconds < 18.0F) {
        return 1;
    }
    if (survivalTimeSeconds < 38.0F) {
        return roll < 65 ? 1 : 2;
    }
    if (survivalTimeSeconds < 70.0F) {
        return roll < 45 ? 2 : 3;
    }

    return roll < 35 ? 3 : 4;
}

float EnemyDirector::GetDifficultyScale(float survivalTimeSeconds) const {
    return 1.0F + survivalTimeSeconds / 28.0F;
}
