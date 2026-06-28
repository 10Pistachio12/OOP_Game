#ifndef UPGRADE_MANAGER_HPP
#define UPGRADE_MANAGER_HPP

#include "pch.hpp"

#include <array>
#include <random>

#include "Upgrade.hpp"

class WeaponInventory;

class UpgradeManager {
public:
    explicit UpgradeManager(std::mt19937 &rng);

    std::vector<std::shared_ptr<Upgrade>> GenerateChoices(
        std::size_t count, const WeaponInventory &weapons);
    void RecordApplied(const Upgrade &upgrade);
    int GetPassiveUpgradeLevel(PassiveUpgradeType type) const;
    int GetPassiveSlotCount() const;
    int GetMaxPassiveSlots() const { return MAX_PASSIVE_SLOTS; }
    bool HasPassiveUpgradeLevel(PassiveUpgradeType type,
                                int requiredLevel) const;

private:
    static constexpr int MAX_PASSIVE_SLOTS = 6;

    bool CanOfferPassiveUpgrade(PassiveUpgradeType type) const;

    std::array<int, static_cast<std::size_t>(PassiveUpgradeType::Count)>
        m_PassiveUpgradeLevels{};
    std::mt19937 &m_Rng;
};

#endif
