#include "RewardSystem.hpp"

#include "Player.hpp"
#include "UpgradeManager.hpp"
#include "WeaponInventory.hpp"

ChestRewardResult RewardSystem::OpenRewardChest(Player &player,
                                                WeaponInventory &weapons,
                                                UpgradeManager &upgrades,
                                                std::mt19937 &rng) {
    if (const std::string evolutionReward =
            weapons.EvolveRandomWeapon(rng, upgrades);
        !evolutionReward.empty()) {
        return {ChestRewardKind::WeaponEvolution,
                "Chest Evolution: " + evolutionReward,
                "EVOLUTION!",
                evolutionReward,
                0};
    }

    if (const std::string levelReward = weapons.LevelUpRandomWeapon(rng);
        !levelReward.empty()) {
        return {ChestRewardKind::WeaponLevel,
                "Chest Reward: " + levelReward,
                "Chest Reward",
                levelReward,
                0};
    }

    player.Heal(2);
    const int levelUps = player.GainExperience(8);
    return {ChestRewardKind::Recovery,
            "Chest Reward: +8 XP and heal 2",
            "Chest Reward",
            "+8 XP  +2 HP",
            levelUps};
}
