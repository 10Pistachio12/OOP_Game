#include "GameScene.hpp"

#include <algorithm>
#include <sstream>

#include <glm/geometric.hpp>

#include "BatEnemy.hpp"
#include "BruteEnemy.hpp"
#include "EliteEnemy.hpp"
#include "SlimeEnemy.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Time.hpp"
#include "config.hpp"

namespace {
constexpr float HALF_WIDTH = static_cast<float>(WINDOW_WIDTH) * 0.5F;
constexpr float HALF_HEIGHT = static_cast<float>(WINDOW_HEIGHT) * 0.5F;
constexpr float VICTORY_TIME_SECONDS = 180.0F;

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
      m_Weapons(std::make_unique<WeaponInventory>(m_FontPath)),
      m_Rng(std::random_device{}()),
      m_EnemyDirector(std::make_unique<EnemyDirector>(m_Rng)),
      m_UpgradeManager(std::make_unique<UpgradeManager>(m_Rng)) {}

void GameScene::Start() {
    ShowTitleScreen();
}

void GameScene::Update() {
    const float deltaTimeSeconds = Util::Time::GetDeltaTimeMs() / 1000.0F;

    if (m_Status == Status::TITLE) {
        if (Util::Input::IsKeyDown(Util::Keycode::RETURN)) {
            Reset();
        }
        UpdateHud();
        m_Renderer.Update();
        return;
    }

    if (m_Status == Status::GAME_OVER || m_Status == Status::VICTORY) {
        HandleEndScreenInput();
        UpdateHud();
        m_Renderer.Update();
        return;
    }

#ifdef DEBUG_TOOLS_ENABLED
    HandleDebugInput();
    if (m_Status == Status::VICTORY) {
        UpdateHud();
        m_Renderer.Update();
        return;
    }
#endif

    if (m_Status == Status::LEVEL_UP) {
        HandleLevelUpInput();
        UpdateHud();
        m_Renderer.Update();
        return;
    }

    if (m_Status == Status::PAUSED) {
        if (Util::Input::IsKeyDown(Util::Keycode::SPACE)) {
            m_Status = Status::RUNNING;
        }
        UpdateHud();
        m_Renderer.Update();
        return;
    }

    if (Util::Input::IsKeyDown(Util::Keycode::SPACE)) {
        m_Status = Status::PAUSED;
        UpdateHud();
        m_Renderer.Update();
        return;
    }

    m_SurvivalTime += deltaTimeSeconds;

    if (m_SurvivalTime >= VICTORY_TIME_SECONDS) {
        m_Status = Status::VICTORY;
        UpdateHud();
        m_Renderer.Update();
        return;
    }

    m_EnemySpawnTimer -= deltaTimeSeconds;

    if (m_SurvivalTime >= m_NextEliteSpawnTime) {
        SpawnEliteWave();
        ++m_EliteSpawnsCompleted;
        m_NextEliteSpawnTime =
            m_EnemyDirector->GetNextEliteSpawnTime(m_EliteSpawnsCompleted);
    }

    if (m_EnemySpawnTimer <= 0.0F) {
        SpawnEnemyWave();
        m_EnemySpawnTimer = m_EnemyDirector->GetSpawnInterval(m_SurvivalTime);
    }

    m_Player->UpdateMovement(deltaTimeSeconds);
    m_PlayerMarker->Update(m_Player->GetPosition(), deltaTimeSeconds);

    for (const auto &enemy : m_Enemies) {
        if (enemy->IsAlive()) {
            enemy->Update(deltaTimeSeconds, m_Player->GetPosition());
        }
    }

    for (const auto &display : m_EnemyHealthDisplays) {
        display.healthBar->Update(*display.enemy);
    }

    for (const auto &projectile : m_Projectiles) {
        if (projectile->IsAlive()) {
            projectile->Update(deltaTimeSeconds);
        }
    }

    for (const auto &gem : m_ExperienceGems) {
        if (gem->IsAlive()) {
            gem->Update(deltaTimeSeconds, m_Player->GetPosition(),
                        m_Player->GetPickupRadius());
        }
    }

    for (const auto &text : m_FloatingTexts) {
        if (text->IsAlive()) {
            text->Update(deltaTimeSeconds);
        }
    }

    SpawnProjectiles(
        m_Weapons->UpdateAndFire(*m_Player, m_Enemies, deltaTimeSeconds));
    HandleCollisions();
    CleanupDestroyed();
    UpdateHud();
    m_Renderer.Update();
}

void GameScene::ShowTitleScreen() {
    m_Status = Status::TITLE;
    m_PendingLevelUps = 0;
    m_KillCount = 0;
    m_EliteKills = 0;
    m_EliteSpawnsCompleted = 0;
    m_ChestsOpened = 0;
    m_SurvivalTime = 0.0F;
    m_EnemySpawnTimer = 0.0F;
    m_LastRewardText = "None";
    m_NextEliteSpawnTime =
        m_EnemyDirector->GetNextEliteSpawnTime(m_EliteSpawnsCompleted);

    m_Renderer = Util::Renderer();
    m_Enemies.clear();
    m_EnemyHealthDisplays.clear();
    m_Projectiles.clear();
    m_ExperienceGems.clear();
    m_RewardChests.clear();
    m_FloatingTexts.clear();
    m_UpgradeChoices.clear();

    m_Hud = std::make_shared<Hud>(m_FontPath);
    m_Renderer.AddChild(m_Hud);
    UpdateHud();
}

void GameScene::Reset() {
    m_Status = Status::RUNNING;
    m_PendingLevelUps = 0;
    m_KillCount = 0;
    m_EliteKills = 0;
    m_EliteSpawnsCompleted = 0;
    m_ChestsOpened = 0;
    m_SurvivalTime = 0.0F;
    m_EnemySpawnTimer = 0.0F;
    m_LastRewardText = "None";

    m_Renderer = Util::Renderer();
    m_Enemies.clear();
    m_EnemyHealthDisplays.clear();
    m_Projectiles.clear();
    m_ExperienceGems.clear();
    m_RewardChests.clear();
    m_FloatingTexts.clear();

    m_Player = std::make_shared<Player>(m_FontPath);
    m_PlayerMarker = std::make_shared<PlayerMarker>(m_FontPath);
    m_Hud = std::make_shared<Hud>(m_FontPath);
    m_Weapons = std::make_unique<WeaponInventory>(m_FontPath);
    m_Weapons->UnlockWeapon(WeaponType::MagicBolt);
    m_EnemyDirector = std::make_unique<EnemyDirector>(m_Rng);
    m_NextEliteSpawnTime =
        m_EnemyDirector->GetNextEliteSpawnTime(m_EliteSpawnsCompleted);
    m_UpgradeManager = std::make_unique<UpgradeManager>(m_Rng);
    m_UpgradeChoices.clear();

    m_Renderer.AddChild(m_Player);
    m_Renderer.AddChild(m_PlayerMarker);
    m_Renderer.AddChild(m_Hud);
    UpdateHud();
}

void GameScene::EnterLevelUp() {
    if (m_PendingLevelUps <= 0) {
        return;
    }

    m_Status = Status::LEVEL_UP;
    m_UpgradeChoices = m_UpgradeManager->GenerateChoices(3, *m_Weapons);
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

    const auto selectedChoice =
        m_UpgradeChoices[static_cast<std::size_t>(selectedIndex)];
    const std::string selectedName = selectedChoice->GetName();
    selectedChoice->Apply(*m_Player, *m_Weapons);
    --m_PendingLevelUps;

    if (m_PendingLevelUps > 0) {
        m_UpgradeChoices = m_UpgradeManager->GenerateChoices(3, *m_Weapons);
        return;
    }

    m_UpgradeChoices.clear();
    m_Status = Status::RUNNING;
    SpawnFloatingText(selectedName,
                      m_Player->GetPosition() + glm::vec2(0.0F, 34.0F),
                      Util::Color(255, 230, 110), 1.1F);
}

void GameScene::HandleEndScreenInput() {
    if (Util::Input::IsKeyDown(Util::Keycode::R)) {
        Reset();
        return;
    }

    if (Util::Input::IsKeyDown(Util::Keycode::RETURN)) {
        ShowTitleScreen();
    }
}

#ifdef DEBUG_TOOLS_ENABLED
void GameScene::HandleDebugInput() {
    if (Util::Input::IsKeyDown(Util::Keycode::T)) {
        m_ShowDebugHud = !m_ShowDebugHud;
    }

    if (Util::Input::IsKeyDown(Util::Keycode::Y)) {
        GrantDebugLevelUp();
    }

    if (Util::Input::IsKeyDown(Util::Keycode::U)) {
        m_Weapons->UnlockWeapon(WeaponType::ArcaneNova);
    }

    if (Util::Input::IsKeyDown(Util::Keycode::J)) {
        m_Weapons->UnlockWeapon(WeaponType::OrbitingShield);
    }

    if (Util::Input::IsKeyDown(Util::Keycode::I)) {
        m_Weapons->LevelUpAllWeapons();
    }

    if (Util::Input::IsKeyDown(Util::Keycode::O)) {
        SpawnEnemyWave();
    }

    if (Util::Input::IsKeyDown(Util::Keycode::P)) {
        SpawnEliteWave();
    }

    if (Util::Input::IsKeyDown(Util::Keycode::C)) {
        SpawnRewardChest(m_Player->GetPosition());
    }

    if (Util::Input::IsKeyDown(Util::Keycode::V)) {
        ForceDebugVictory();
    }
}

void GameScene::GrantDebugLevelUp() {
    const int neededExperience = std::max(
        1, m_Player->GetExperienceToNextLevel() - m_Player->GetExperience());
    const int levelUps = m_Player->GainExperience(neededExperience);
    if (levelUps <= 0) {
        return;
    }

    m_PendingLevelUps += levelUps;
    if (m_Status == Status::RUNNING) {
        EnterLevelUp();
    }
}

void GameScene::ForceDebugVictory() {
    m_SurvivalTime = VICTORY_TIME_SECONDS;
    m_Status = Status::VICTORY;
}
#endif

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

int GameScene::CountAliveEnemies() const {
    return static_cast<int>(
        std::count_if(m_Enemies.begin(), m_Enemies.end(),
                      [](const auto &enemy) { return enemy->IsAlive(); }));
}

void GameScene::SpawnEnemyWave() {
    const int maxActiveEnemies =
        m_EnemyDirector->GetMaxActiveEnemies(m_SurvivalTime);
    if (CountAliveEnemies() >= maxActiveEnemies) {
        return;
    }

    const EnemySpawnRequest request =
        m_EnemyDirector->CreateSpawnRequest(m_SurvivalTime);
    for (int i = 0; i < request.count; ++i) {
        if (CountAliveEnemies() >= maxActiveEnemies) {
            break;
        }
        SpawnEnemy(request.type, request.difficultyScale);
    }
}

void GameScene::SpawnEliteWave() {
    const EnemySpawnRequest request =
        m_EnemyDirector->CreateEliteSpawnRequest(m_SurvivalTime);
    for (int i = 0; i < request.count; ++i) {
        SpawnEnemy(request.type, request.difficultyScale);
    }
}

void GameScene::SpawnEnemy(EnemyType enemyType, float difficultyScale) {
    const glm::vec2 spawnPosition = GenerateSpawnPosition();

    std::shared_ptr<Enemy> enemy;
    switch (enemyType) {
        case EnemyType::Bat:
            enemy = std::make_shared<BatEnemy>(m_FontPath, spawnPosition,
                                               difficultyScale);
            break;
        case EnemyType::Brute:
            enemy = std::make_shared<BruteEnemy>(m_FontPath, spawnPosition,
                                                 difficultyScale);
            break;
        case EnemyType::Elite:
            enemy = std::make_shared<EliteEnemy>(m_FontPath, spawnPosition,
                                                 difficultyScale);
            break;
        case EnemyType::Slime:
        default:
            enemy = std::make_shared<SlimeEnemy>(m_FontPath, spawnPosition,
                                                 difficultyScale);
            break;
    }

    m_Enemies.push_back(enemy);
    m_Renderer.AddChild(enemy);

    auto healthBar = std::make_shared<EnemyHealthBar>();
    m_EnemyHealthDisplays.push_back({enemy, healthBar});
    m_Renderer.AddChild(healthBar);
}

void GameScene::SpawnExperienceGem(const glm::vec2 &position, int value) {
    auto gem = std::make_shared<ExperienceGem>(m_FontPath, position, value);
    m_ExperienceGems.push_back(gem);
    m_Renderer.AddChild(gem);
}

void GameScene::SpawnRewardChest(const glm::vec2 &position) {
    auto chest = std::make_shared<RewardChest>(m_FontPath, position);
    m_RewardChests.push_back(chest);
    m_Renderer.AddChild(chest);
}

void GameScene::SpawnProjectiles(
    const std::vector<std::shared_ptr<Projectile>> &projectiles) {
    for (const auto &projectile : projectiles) {
        m_Projectiles.push_back(projectile);
        m_Renderer.AddChild(projectile);
    }
}

void GameScene::SpawnFloatingText(const std::string &content,
                                  const glm::vec2 &position,
                                  const Util::Color &color,
                                  float lifetimeSeconds) {
    if (m_FloatingTexts.size() >= 32) {
        m_Renderer.RemoveChild(m_FloatingTexts.front());
        m_FloatingTexts.erase(m_FloatingTexts.begin());
    }

    auto text =
        std::make_shared<FloatingText>(m_FontPath, content, color, position,
                                       lifetimeSeconds);
    m_FloatingTexts.push_back(text);
    m_Renderer.AddChild(text);
}

void GameScene::OpenRewardChest(const std::shared_ptr<RewardChest> &chest) {
    if (!chest->IsAlive()) {
        return;
    }

    const std::string weaponReward = m_Weapons->LevelUpRandomWeapon(m_Rng);
    if (!weaponReward.empty()) {
        m_LastRewardText = "Chest Reward: " + weaponReward;
        SpawnFloatingText("Chest: " + weaponReward, chest->GetPosition(),
                          Util::Color(255, 220, 80), 1.2F);
    } else {
        m_Player->Heal(2);
        const int levelUps = m_Player->GainExperience(8);
        m_LastRewardText = "Chest Reward: +8 XP and heal 2";
        SpawnFloatingText("+8 XP  +2 HP", chest->GetPosition(),
                          Util::Color(255, 220, 80), 1.2F);
        if (levelUps > 0 && m_Status == Status::RUNNING) {
            m_PendingLevelUps += levelUps;
            EnterLevelUp();
        }
    }

    ++m_ChestsOpened;
    chest->Destroy();
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
            SpawnFloatingText("-" + std::to_string(projectile->GetDamage()),
                              enemy->GetPosition() + glm::vec2(0.0F, 18.0F),
                              Util::Color(255, 120, 95), 0.55F);

            if (!enemy->IsAlive()) {
                ++m_KillCount;
                if (enemy->IsElite()) {
                    ++m_EliteKills;
                    SpawnRewardChest(enemy->GetPosition());
                    SpawnFloatingText("Chest!", enemy->GetPosition(),
                                      Util::Color(255, 220, 80), 0.9F);
                }
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
        SpawnFloatingText("-" + std::to_string(enemy->GetContactDamage()) + " HP",
                          m_Player->GetPosition() + glm::vec2(0.0F, 28.0F),
                          Util::Color(255, 95, 95), 0.75F);

        if (!m_Player->IsAlive()) {
            m_Status = Status::GAME_OVER;
            break;
        }
    }

    for (const auto &gem : m_ExperienceGems) {
        if (!gem->IsAlive()) {
            continue;
        }

        if (!IsColliding(m_Player->GetPosition(), m_Player->GetRadius(),
                         gem->GetPosition(), gem->GetRadius())) {
            continue;
        }

        const int levelUps = m_Player->GainExperience(gem->GetValue());
        SpawnFloatingText("+" + std::to_string(gem->GetValue()) + " XP",
                          gem->GetPosition(), Util::Color(100, 225, 255),
                          0.55F);
        if (levelUps > 0 && m_Status == Status::RUNNING) {
            m_PendingLevelUps += levelUps;
            EnterLevelUp();
        }
        gem->Destroy();
    }

    for (const auto &chest : m_RewardChests) {
        if (!chest->IsAlive()) {
            continue;
        }

        if (!IsColliding(m_Player->GetPosition(), m_Player->GetRadius(),
                         chest->GetPosition(), chest->GetRadius())) {
            continue;
        }

        OpenRewardChest(chest);
    }
}

void GameScene::CleanupDestroyed() {
    m_EnemyHealthDisplays.erase(
        std::remove_if(m_EnemyHealthDisplays.begin(), m_EnemyHealthDisplays.end(),
                       [this](const auto &display) {
                           if (display.enemy->IsAlive() &&
                               display.healthBar->IsAlive()) {
                               return false;
                           }
                           display.healthBar->Destroy();
                           m_Renderer.RemoveChild(display.healthBar);
                           return true;
                       }),
        m_EnemyHealthDisplays.end());

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
    removeDestroyed(m_RewardChests);
    removeDestroyed(m_FloatingTexts);
}

void GameScene::UpdateHud() const {
    std::ostringstream stream;

    if (m_Status == Status::TITLE) {
        stream << "Vampire Survivors\n\n";
        stream << "Survive " << static_cast<int>(VICTORY_TIME_SECONDS)
               << " seconds against escalating enemy waves.\n";
        stream << "Press Enter to Start\n\n";
        stream << "Move: WASD / Arrow Keys\n";
        stream << "Choose upgrades: 1 / 2 / 3\n";
        stream << "Pause: Space  Exit: ESC";
#ifdef DEBUG_TOOLS_ENABLED
        stream << "\n\nDebug build: T shows test controls after starting.";
#endif
        m_Hud->SetContent(stream.str());
        return;
    }

    if (m_Status == Status::GAME_OVER || m_Status == Status::VICTORY) {
        stream << (m_Status == Status::VICTORY ? "Victory" : "Game Over")
               << "\n\n";
        stream << "Survival Time: " << static_cast<int>(m_SurvivalTime) << " / "
               << static_cast<int>(VICTORY_TIME_SECONDS) << "s\n";
        stream << "Level: " << m_Player->GetLevel()
               << "  Kills: " << m_KillCount
               << "  Elites: " << m_EliteKills
               << "  Chests: " << m_ChestsOpened << '\n';
        stream << "Weapons: " << m_Weapons->GetDisplayNames() << "\n\n";
        stream << "Last Reward: " << m_LastRewardText << "\n\n";
        stream << "R: Restart  Enter: Title  ESC: Exit";
        m_Hud->SetContent(stream.str());
        return;
    }

    stream << "Vampire Survivors MVP\n";
    stream << "Weapons: " << m_Weapons->GetDisplayNames() << "  "
           << "HP: " << m_Player->GetHitPoints() << "/" << m_Player->GetMaxHitPoints()
           << "  LV: " << m_Player->GetLevel() << '\n';
    stream << "XP: " << m_Player->GetExperience() << "/"
           << m_Player->GetExperienceToNextLevel() << "  "
           << "Time: " << static_cast<int>(m_SurvivalTime) << "s  "
           << "Enemies: " << m_Enemies.size() << '\n';
    stream << "Phase: " << m_EnemyDirector->GetPhaseName(m_SurvivalTime)
           << "  Magnet: " << static_cast<int>(m_Player->GetPickupRadius())
           << '\n';
    stream << "Kills: " << m_KillCount << "  Elites: " << m_EliteKills
           << "  Chests: " << m_ChestsOpened
           << "  Cap: " << m_EnemyDirector->GetMaxActiveEnemies(m_SurvivalTime)
           << "  Next Elite: "
           << std::max(0, static_cast<int>(m_NextEliteSpawnTime - m_SurvivalTime))
           << "s\n";
    stream << "Move: WASD / Arrow Keys  Space: Pause  ESC: Exit";

    if (m_Status == Status::GAME_OVER) {
        stream << "\nGame Over - Press R to Restart";
    } else if (m_Status == Status::PAUSED) {
        stream << "\nPaused - Press Space to Resume";
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
    stream << "\n" << "Last Reward: " << m_LastRewardText;

#ifdef DEBUG_TOOLS_ENABLED
    if (m_ShowDebugHud) {
        stream << "\n[Debug] T: HUD  Y: Level Up  U: Unlock Nova  "
               << "J: Unlock Shield  I: Weapon Lv  O: Wave  P: Elite  "
               << "C: Chest  V: Victory";
        stream << "\n[Debug] Projectiles: " << m_Projectiles.size()
               << "  Gems: " << m_ExperienceGems.size()
               << "  Chests: " << m_RewardChests.size()
               << "  Texts: " << m_FloatingTexts.size()
               << "  Bars: " << m_EnemyHealthDisplays.size()
               << "  Pending LevelUps: " << m_PendingLevelUps
               << "  Elite Spawns: " << m_EliteSpawnsCompleted;
    } else {
        stream << "\n[Debug] Press T for test controls";
    }
#endif

    m_Hud->SetContent(stream.str());
}
