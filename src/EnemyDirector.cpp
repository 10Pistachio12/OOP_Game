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
        survivalTimeSeconds >= 120.0F ? 2 : 1,
        GetDifficultyScale(survivalTimeSeconds) * 1.1F,
    };
}

float EnemyDirector::GetSpawnInterval(float survivalTimeSeconds) const {
    const float interval = 1.65F - survivalTimeSeconds * 0.008F;
    return std::max(0.48F, interval);
}

float EnemyDirector::GetNextEliteSpawnTime(int eliteSpawnsCompleted) const {
    return 40.0F + static_cast<float>(eliteSpawnsCompleted) * 40.0F;
}

int EnemyDirector::GetMaxActiveEnemies(float survivalTimeSeconds) const {
    if (survivalTimeSeconds < 30.0F) {
        return 18;
    }
    if (survivalTimeSeconds < 70.0F) {
        return 28;
    }
    if (survivalTimeSeconds < 120.0F) {
        return 42;
    }

    return 56;
}

std::string EnemyDirector::GetPhaseName(float survivalTimeSeconds) const {
    if (survivalTimeSeconds < 18.0F) {
        return "Opening Swarm";
    }
    if (survivalTimeSeconds < 45.0F) {
        return "Bat Invasion";
    }
    if (survivalTimeSeconds < 85.0F) {
        return "Heavy Horde";
    }
    return "Endurance";
}

EnemyType EnemyDirector::RollEnemyType(float survivalTimeSeconds) {
    const int roll = RollPercent(m_Rng);

    if (survivalTimeSeconds < 12.0F) {
        return EnemyType::Slime;
    }
    if (survivalTimeSeconds < 35.0F) {
        return roll < 18 ? EnemyType::Bat : EnemyType::Slime;
    }
    if (survivalTimeSeconds < 75.0F) {
        if (roll < 7) {
            return EnemyType::Brute;
        }
        return roll < 38 ? EnemyType::Bat : EnemyType::Slime;
    }

    if (roll < 15) {
        return EnemyType::Brute;
    }
    return roll < 48 ? EnemyType::Bat : EnemyType::Slime;
}

int EnemyDirector::GetBatchSize(float survivalTimeSeconds) {
    const int roll = RollPercent(m_Rng);

    if (survivalTimeSeconds < 30.0F) {
        return 1;
    }
    if (survivalTimeSeconds < 70.0F) {
        return roll < 82 ? 1 : 2;
    }
    if (survivalTimeSeconds < 120.0F) {
        return roll < 58 ? 1 : 2;
    }

    return roll < 70 ? 2 : 3;
}

float EnemyDirector::GetDifficultyScale(float survivalTimeSeconds) const {
    return 1.0F + survivalTimeSeconds / 42.0F;
}
