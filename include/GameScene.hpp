#ifndef GAME_SCENE_HPP
#define GAME_SCENE_HPP

#include "pch.hpp"

#include <random>

#include "Enemy.hpp"
#include "ExperienceGem.hpp"
#include "Hud.hpp"
#include "Player.hpp"
#include "Projectile.hpp"
#include "UpgradeManager.hpp"
#include "Util/Renderer.hpp"
#include "Weapon.hpp"

class GameScene {
public:
    GameScene();

    void Start();
    void Update();

private:
    enum class Status {
        RUNNING,
        LEVEL_UP,
        GAME_OVER,
    };

    void Reset();
    void EnterLevelUp();
    void HandleLevelUpInput();
    glm::vec2 GenerateSpawnPosition();
    void SpawnEnemy();
    void SpawnExperienceGem(const glm::vec2 &position, int value);
    void SpawnProjectiles(
        const std::vector<std::shared_ptr<Projectile>> &projectiles);
    void HandleCollisions();
    void CleanupDestroyed();
    void UpdateHud() const;

private:
    std::string m_FontPath;
    mutable std::shared_ptr<Hud> m_Hud;
    std::shared_ptr<Player> m_Player;
    std::unique_ptr<Weapon> m_Weapon;
    std::mt19937 m_Rng;
    std::unique_ptr<UpgradeManager> m_UpgradeManager;
    std::vector<std::shared_ptr<Upgrade>> m_UpgradeChoices;
    std::vector<std::shared_ptr<Enemy>> m_Enemies;
    std::vector<std::shared_ptr<Projectile>> m_Projectiles;
    std::vector<std::shared_ptr<ExperienceGem>> m_ExperienceGems;
    Util::Renderer m_Renderer;
    Status m_Status = Status::RUNNING;
    int m_PendingLevelUps = 0;
    float m_SurvivalTime = 0.0F;
    float m_EnemySpawnTimer = 0.0F;
};

#endif
