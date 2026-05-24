#include "UpgradeManager.hpp"

#include <algorithm>

UpgradeManager::UpgradeManager(std::mt19937 &rng)
    : m_Rng(rng) {}

std::vector<std::shared_ptr<Upgrade>> UpgradeManager::GenerateChoices(
    std::size_t count) {
    std::vector<std::shared_ptr<Upgrade>> pool{
        std::make_shared<MoveSpeedUpgrade>(),
        std::make_shared<MaxHealthUpgrade>(),
        std::make_shared<PickupRangeUpgrade>(),
        std::make_shared<WeaponDamageUpgrade>(),
        std::make_shared<WeaponCooldownUpgrade>(),
    };

    std::shuffle(pool.begin(), pool.end(), m_Rng);
    if (count < pool.size()) {
        pool.resize(count);
    }

    return pool;
}
