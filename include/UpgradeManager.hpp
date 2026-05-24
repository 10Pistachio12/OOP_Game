#ifndef UPGRADE_MANAGER_HPP
#define UPGRADE_MANAGER_HPP

#include "pch.hpp"

#include <random>

#include "Upgrade.hpp"

class WeaponInventory;

class UpgradeManager {
public:
    explicit UpgradeManager(std::mt19937 &rng);

    std::vector<std::shared_ptr<Upgrade>> GenerateChoices(
        std::size_t count, const WeaponInventory &weapons);

private:
    std::mt19937 &m_Rng;
};

#endif
