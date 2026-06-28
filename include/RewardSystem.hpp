#ifndef REWARD_SYSTEM_HPP
#define REWARD_SYSTEM_HPP

#include <random>
#include <string>

class Player;
class UpgradeManager;
class WeaponInventory;

enum class ChestRewardKind {
    WeaponEvolution,
    WeaponLevel,
    Recovery,
};

struct ChestRewardResult {
    ChestRewardKind kind = ChestRewardKind::Recovery;
    std::string hudText;
    std::string titleText;
    std::string detailText;
    int levelUps = 0;
};

class RewardSystem final {
public:
    static ChestRewardResult OpenRewardChest(Player &player,
                                             WeaponInventory &weapons,
                                             UpgradeManager &upgrades,
                                             std::mt19937 &rng);
};

#endif
