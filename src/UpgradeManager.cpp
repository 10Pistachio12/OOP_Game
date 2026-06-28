#include "UpgradeManager.hpp"

#include <algorithm>

#include "WeaponInventory.hpp"

namespace {
struct UpgradeCandidate {
    std::shared_ptr<Upgrade> upgrade;
    int weight = 1;
};

std::size_t ToIndex(PassiveUpgradeType type) {
    return static_cast<std::size_t>(type);
}

void AddCandidate(std::vector<UpgradeCandidate> &candidates,
                  std::shared_ptr<Upgrade> upgrade, int weight) {
    if (weight <= 0) {
        return;
    }

    candidates.push_back({std::move(upgrade), weight});
}

int GetWeaponLevelWeight(const WeaponInventory &weapons, WeaponType type) {
    const int currentLevel = weapons.GetWeaponLevel(type);
    return std::max(10, 20 - currentLevel * 2);
}

std::size_t PickWeightedIndex(const std::vector<UpgradeCandidate> &candidates,
                              std::mt19937 &rng) {
    int totalWeight = 0;
    for (const auto &candidate : candidates) {
        totalWeight += candidate.weight;
    }

    std::uniform_int_distribution<int> distribution(1, totalWeight);
    int roll = distribution(rng);
    for (std::size_t i = 0; i < candidates.size(); ++i) {
        roll -= candidates[i].weight;
        if (roll <= 0) {
            return i;
        }
    }

    return candidates.size() - 1;
}
}

UpgradeManager::UpgradeManager(std::mt19937 &rng)
    : m_Rng(rng) {}

std::vector<std::shared_ptr<Upgrade>> UpgradeManager::GenerateChoices(
    std::size_t count, const WeaponInventory &weapons) {
    std::vector<UpgradeCandidate> candidates;
    const auto addPassiveCandidate =
        [&](PassiveUpgradeType type, int weight, const auto &factory) {
            const int maxLevel = GetPassiveUpgradeMaxLevel(type);
            const int currentLevel = GetPassiveUpgradeLevel(type);
            if (currentLevel >= maxLevel || !CanOfferPassiveUpgrade(type)) {
                return;
            }

            AddCandidate(candidates, factory(currentLevel + 1, maxLevel),
                         weight);
        };

    addPassiveCandidate(PassiveUpgradeType::MoveSpeed, 12,
                        [](int nextLevel, int maxLevel) {
                            return std::make_shared<MoveSpeedUpgrade>(
                                nextLevel, maxLevel);
                        });
    addPassiveCandidate(PassiveUpgradeType::MaxHealth, 14,
                        [](int nextLevel, int maxLevel) {
                            return std::make_shared<MaxHealthUpgrade>(
                                nextLevel, maxLevel);
                        });
    addPassiveCandidate(PassiveUpgradeType::PickupRange, 10,
                        [](int nextLevel, int maxLevel) {
                            return std::make_shared<PickupRangeUpgrade>(
                                nextLevel, maxLevel);
                        });
    addPassiveCandidate(PassiveUpgradeType::WeaponDamage, 15,
                        [](int nextLevel, int maxLevel) {
                            return std::make_shared<WeaponDamageUpgrade>(
                                nextLevel, maxLevel);
                        });
    addPassiveCandidate(PassiveUpgradeType::WeaponCooldown, 14,
                        [](int nextLevel, int maxLevel) {
                            return std::make_shared<WeaponCooldownUpgrade>(
                                nextLevel, maxLevel);
                        });
    addPassiveCandidate(PassiveUpgradeType::ProjectileCount, 7,
                        [](int nextLevel, int maxLevel) {
                            return std::make_shared<ProjectileCountUpgrade>(
                                nextLevel, maxLevel);
                        });

    for (WeaponType type : weapons.GetOwnedWeaponTypes()) {
        if (!weapons.CanLevelUpWeapon(type)) {
            continue;
        }

        AddCandidate(candidates,
                     std::make_shared<WeaponLevelUpgrade>(
                         type, weapons.GetWeaponLevel(type) + 1,
                         weapons.GetWeaponLevelUpDescription(type)),
                     GetWeaponLevelWeight(weapons, type));
    }

    if (!weapons.HasWeapon(WeaponType::ArcaneNova) &&
        weapons.HasOpenWeaponSlot()) {
        AddCandidate(candidates,
                     std::make_shared<UnlockWeaponUpgrade>(
                         WeaponType::ArcaneNova),
                     11);
    }

    if (!weapons.HasWeapon(WeaponType::OrbitingShield) &&
        weapons.HasOpenWeaponSlot()) {
        AddCandidate(candidates,
                     std::make_shared<UnlockWeaponUpgrade>(
                         WeaponType::OrbitingShield),
                     11);
    }

    if (candidates.empty()) {
        AddCandidate(candidates, std::make_shared<OverflowRecoveryUpgrade>(),
                     1);
    }

    std::vector<std::shared_ptr<Upgrade>> choices;
    while (choices.size() < count && !candidates.empty()) {
        const std::size_t selectedIndex = PickWeightedIndex(candidates, m_Rng);
        choices.push_back(candidates[selectedIndex].upgrade);
        candidates.erase(candidates.begin() +
                         static_cast<std::ptrdiff_t>(selectedIndex));
    }

    return choices;
}

void UpgradeManager::RecordApplied(const Upgrade &upgrade) {
    const PassiveUpgradeType type = upgrade.GetPassiveType();
    if (type == PassiveUpgradeType::None || type == PassiveUpgradeType::Count) {
        return;
    }

    const int maxLevel = GetPassiveUpgradeMaxLevel(type);
    int &currentLevel = m_PassiveUpgradeLevels[ToIndex(type)];
    currentLevel = std::min(maxLevel, currentLevel + 1);
}

int UpgradeManager::GetPassiveUpgradeLevel(PassiveUpgradeType type) const {
    if (type == PassiveUpgradeType::None || type == PassiveUpgradeType::Count) {
        return 0;
    }

    return m_PassiveUpgradeLevels[ToIndex(type)];
}

int UpgradeManager::GetPassiveSlotCount() const {
    int count = 0;
    for (std::size_t i = ToIndex(PassiveUpgradeType::MoveSpeed);
         i < ToIndex(PassiveUpgradeType::Count); ++i) {
        if (m_PassiveUpgradeLevels[i] > 0) {
            ++count;
        }
    }

    return count;
}

bool UpgradeManager::HasPassiveUpgradeLevel(PassiveUpgradeType type,
                                            int requiredLevel) const {
    return GetPassiveUpgradeLevel(type) >= requiredLevel;
}

bool UpgradeManager::CanOfferPassiveUpgrade(PassiveUpgradeType type) const {
    const int currentLevel = GetPassiveUpgradeLevel(type);
    return currentLevel > 0 || GetPassiveSlotCount() < MAX_PASSIVE_SLOTS;
}
