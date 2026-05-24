#include "UpgradeManager.hpp"

#include <algorithm>

#include "WeaponInventory.hpp"

UpgradeManager::UpgradeManager(std::mt19937 &rng)
    : m_Rng(rng) {}

std::vector<std::shared_ptr<Upgrade>> UpgradeManager::GenerateChoices(
    std::size_t count, const WeaponInventory &weapons) {
    std::vector<std::shared_ptr<Upgrade>> pool{
        std::make_shared<MoveSpeedUpgrade>(),
        std::make_shared<MaxHealthUpgrade>(),
        std::make_shared<PickupRangeUpgrade>(),
        std::make_shared<WeaponDamageUpgrade>(),
        std::make_shared<WeaponCooldownUpgrade>(),
    };

    for (WeaponType type : weapons.GetOwnedWeaponTypes()) {
        if (!weapons.CanLevelUpWeapon(type)) {
            continue;
        }

        pool.push_back(std::make_shared<WeaponLevelUpgrade>(
            type, weapons.GetWeaponLevel(type) + 1,
            weapons.GetWeaponLevelUpDescription(type)));
    }

    if (!weapons.HasWeapon(WeaponType::ArcaneNova)) {
        pool.push_back(std::make_shared<UnlockWeaponUpgrade>(
            WeaponType::ArcaneNova));
    }

    std::shuffle(pool.begin(), pool.end(), m_Rng);
    if (count < pool.size()) {
        pool.resize(count);
    }

    return pool;
}
