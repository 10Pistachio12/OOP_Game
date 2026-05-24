#include "GameScene.hpp"

#include <sstream>

#include <glm/geometric.hpp>

#include "BatEnemy.hpp"
#include "MagicBoltWeapon.hpp"
#include "SlimeEnemy.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Time.hpp"
#include "config.hpp"

namespace {
constexpr float HALF_WIDTH = static_cast<float>(WINDOW_WIDTH) * 0.5F;
constexpr float HALF_HEIGHT = static_cast<float>(WINDOW_HEIGHT) * 0.5F;

float RandomRange(std::mt19937 &rng, float minValue, float maxValue) {
    return std::uniform_real_distribution<float>(minValue, maxValue)(rng);
}

bool IsColliding(const glm::vec2 &leftPosition, float leftRadius,
                 const glm::vec2 &rightPosition, float rightRadius) {
    return glm::distance(leftPosition, rightPosition) <= leftRadius + rightRadius;
}
} // namespace

GameScene::GameScene()
    : m_FontPath(std::string(RESOURCE_DIR) + "/Inter.ttf"),
      m_Hud(std::make_shared<Hud>(m_FontPath)),
      m_Player(std::make_shared<Player>(m_FontPath)),
      m_Weapon(std::make_unique<MagicBoltWeapon>(m_FontPath)),
      m_Rng(std::random_device{}()),
      m_UpgradeManager(std::make_unique<UpgradeManager>(m_Rng)) {}

void GameScene::Start() {
    Reset();
}

void GameScene::Update() {
    const float deltaTimeSeconds = Util::Time::GetDeltaTimeMs() / 1000.0F;

    if (m_Status == Status::LEVEL_UP) {
        HandleLevelUpInput();
        UpdateHud();
        m_Renderer.Update();
        return;
    }

    if (m_Status == Status::GAME_OVER) {
        if (Util::Input::IsKeyDown(Util::Keycode::R)) {
            Reset();
        }
        UpdateHud();
        m_Renderer.Update();
        return;
    }

    m_SurvivalTime += deltaTimeSeconds;
    m_EnemySpawnTimer -= deltaTimeSeconds;

    if (m_EnemySpawnTimer <= 0.0F) {
        SpawnEnemy();
        m_EnemySpawnTimer = std::max(0.35F, 1.15F - m_SurvivalTime * 0.025F);
    }

    m_Player->UpdateMovement(deltaTimeSeconds);

    for (const auto &enemy : m_Enemies) {
        if (enemy->IsAlive()) {
            enemy->Update(deltaTimeSeconds, m_Player->GetPosition());
        }
    }

    for (const auto &projectile : m_Projectiles) {
        if (projectile->IsAlive()) {
            projectile->Update(deltaTimeSeconds);
        }
    }

    SpawnProjectiles(
        m_Weapon->UpdateAndFire(*m_Player, m_Enemies, deltaTimeSeconds));
    HandleCollisions();
    CleanupDestroyed();
    UpdateHud();
    m_Renderer.Update();
}

void GameScene::Reset() {
    m_Status = Status::RUNNING;
    m_PendingLevelUps = 0;
    m_SurvivalTime = 0.0F;
    m_EnemySpawnTimer = 0.0F;

    m_Renderer = Util::Renderer();
    m_Enemies.clear();
    m_Projectiles.clear();
    m_ExperienceGems.clear();

    m_Player = std::make_shared<Player>(m_FontPath);
    m_Hud = std::make_shared<Hud>(m_FontPath);
    m_Weapon = std::make_unique<MagicBoltWeapon>(m_FontPath);
    m_UpgradeManager = std::make_unique<UpgradeManager>(m_Rng);
    m_UpgradeChoices.clear();

    m_Renderer.AddChild(m_Player);
    m_Renderer.AddChild(m_Hud);
    UpdateHud();
}

void GameScene::EnterLevelUp() {
    if (m_PendingLevelUps <= 0) {
        return;
    }

    m_Status = Status::LEVEL_UP;
    m_UpgradeChoices = m_UpgradeManager->GenerateChoices(3);
}

void GameScene::HandleLevelUpInput() {
    int selectedIndex = -1;
    if (Util::Input::IsKeyDown(Util::Keycode::NUM_1)) {
        selectedIndex = 0;
    } else if (Util::Input::IsKeyDown(Util::Keycode::NUM_2)) {
        selectedIndex = 1;
    } else if (Util::Input::IsKeyDown(Util::Keycode::NUM_3)) {
        selectedIndex = 2;
    }

    if (selectedIndex < 0 ||
        selectedIndex >= static_cast<int>(m_UpgradeChoices.size())) {
        return;
    }

    m_UpgradeChoices[static_cast<std::size_t>(selectedIndex)]->Apply(
        *m_Player, *m_Weapon);
    --m_PendingLevelUps;

    if (m_PendingLevelUps > 0) {
        m_UpgradeChoices = m_UpgradeManager->GenerateChoices(3);
        return;
    }

    m_UpgradeChoices.clear();
    m_Status = Status::RUNNING;
}

glm::vec2 GameScene::GenerateSpawnPosition() {
    glm::vec2 spawnPosition(0.0F);

    const int edge = std::uniform_int_distribution<int>(0, 3)(m_Rng);
    switch (edge) {
        case 0:
            spawnPosition = {RandomRange(m_Rng, -HALF_WIDTH + 48.0F,
                                         HALF_WIDTH - 48.0F),
                             HALF_HEIGHT - 48.0F};
            break;
        case 1:
            spawnPosition = {RandomRange(m_Rng, -HALF_WIDTH + 48.0F,
                                         HALF_WIDTH - 48.0F),
                             -HALF_HEIGHT + 48.0F};
            break;
        case 2:
            spawnPosition = {-HALF_WIDTH + 48.0F,
                             RandomRange(m_Rng, -HALF_HEIGHT + 48.0F,
                                         HALF_HEIGHT - 48.0F)};
            break;
        default:
            spawnPosition = {HALF_WIDTH - 48.0F,
                             RandomRange(m_Rng, -HALF_HEIGHT + 48.0F,
                                         HALF_HEIGHT - 48.0F)};
            break;
    }

    return spawnPosition;
}

void GameScene::SpawnEnemy() {
    const float difficultyScale = 1.0F + m_SurvivalTime / 25.0F;
    const glm::vec2 spawnPosition = GenerateSpawnPosition();

    std::shared_ptr<Enemy> enemy;
    const int enemyRoll = std::uniform_int_distribution<int>(0, 99)(m_Rng);
    const bool canSpawnBats = m_SurvivalTime >= 10.0F;
    if (canSpawnBats && enemyRoll < 35) {
        enemy = std::make_shared<BatEnemy>(m_FontPath, spawnPosition,
                                           difficultyScale);
    } else {
        enemy = std::make_shared<SlimeEnemy>(m_FontPath, spawnPosition,
                                             difficultyScale);
    }

    m_Enemies.push_back(enemy);
    m_Renderer.AddChild(enemy);
}

void GameScene::SpawnExperienceGem(const glm::vec2 &position, int value) {
    auto gem = std::make_shared<ExperienceGem>(m_FontPath, position, value);
    m_ExperienceGems.push_back(gem);
    m_Renderer.AddChild(gem);
}

void GameScene::SpawnProjectiles(
    const std::vector<std::shared_ptr<Projectile>> &projectiles) {
    for (const auto &projectile : projectiles) {
        m_Projectiles.push_back(projectile);
        m_Renderer.AddChild(projectile);
    }
}

void GameScene::HandleCollisions() {
    for (const auto &projectile : m_Projectiles) {
        if (!projectile->IsAlive()) {
            continue;
        }

        for (const auto &enemy : m_Enemies) {
            if (!enemy->IsAlive()) {
                continue;
            }

            if (!IsColliding(projectile->GetPosition(), projectile->GetRadius(),
                             enemy->GetPosition(), enemy->GetRadius())) {
                continue;
            }

            enemy->TakeDamage(projectile->GetDamage());
            projectile->Destroy();

            if (!enemy->IsAlive()) {
                SpawnExperienceGem(enemy->GetPosition(), enemy->GetExperienceValue());
            }
            break;
        }
    }

    for (const auto &enemy : m_Enemies) {
        if (!enemy->IsAlive()) {
            continue;
        }

        if (!IsColliding(m_Player->GetPosition(), m_Player->GetRadius(),
                         enemy->GetPosition(), enemy->GetRadius())) {
            continue;
        }

        m_Player->TakeDamage(enemy->GetContactDamage());
        enemy->Destroy();

        if (!m_Player->IsAlive()) {
            m_Status = Status::GAME_OVER;
            break;
        }
    }

    for (const auto &gem : m_ExperienceGems) {
        if (!gem->IsAlive()) {
            continue;
        }

        if (!IsColliding(m_Player->GetPosition(), m_Player->GetPickupRadius(),
                         gem->GetPosition(), gem->GetRadius())) {
            continue;
        }

        const int levelUps = m_Player->GainExperience(gem->GetValue());
        if (levelUps > 0 && m_Status == Status::RUNNING) {
            m_PendingLevelUps += levelUps;
            EnterLevelUp();
        }
        gem->Destroy();
    }
}

void GameScene::CleanupDestroyed() {
    auto removeDestroyed = [this](auto &objects) {
        objects.erase(
            std::remove_if(objects.begin(), objects.end(),
                           [this](const auto &object) {
                               if (object->IsAlive()) {
                                   return false;
                               }
                               m_Renderer.RemoveChild(object);
                               return true;
                           }),
            objects.end());
    };

    removeDestroyed(m_Projectiles);
    removeDestroyed(m_Enemies);
    removeDestroyed(m_ExperienceGems);
}

void GameScene::UpdateHud() const {
    std::ostringstream stream;
    stream << "Vampire Survivors MVP\n";
    stream << "Weapon: " << m_Weapon->GetDisplayName() << "  "
           << "HP: " << m_Player->GetHitPoints() << "/" << m_Player->GetMaxHitPoints()
           << "  LV: " << m_Player->GetLevel() << '\n';
    stream << "XP: " << m_Player->GetExperience() << "/"
           << m_Player->GetExperienceToNextLevel() << "  "
           << "Time: " << static_cast<int>(m_SurvivalTime) << "s  "
           << "Enemies: " << m_Enemies.size() << '\n';
    stream << "Move: WASD / Arrow Keys  ESC: Exit";

    if (m_Status == Status::GAME_OVER) {
        stream << "\nGame Over - Press R to Restart";
    } else if (m_Status == Status::LEVEL_UP) {
        stream << "\nChoose an upgrade";
        if (m_PendingLevelUps > 1) {
            stream << " (" << m_PendingLevelUps << " choices pending)";
        }
        stream << ":";
        for (std::size_t i = 0; i < m_UpgradeChoices.size(); ++i) {
            stream << "\n" << (i + 1) << ". " << m_UpgradeChoices[i]->GetName()
                   << " - " << m_UpgradeChoices[i]->GetDescription();
        }
    } else {
        stream << "\nAuto-fire is active. Collect XP gems to choose upgrades.";
    }

    m_Hud->SetContent(stream.str());
}
