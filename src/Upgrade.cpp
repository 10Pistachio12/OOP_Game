#include "Upgrade.hpp"

#include "Player.hpp"
#include "WeaponInventory.hpp"

void MoveSpeedUpgrade::Apply(Player &player, WeaponInventory &) const {
    player.IncreaseMoveSpeed(35.0F);
}

void MaxHealthUpgrade::Apply(Player &player, WeaponInventory &) const {
    player.IncreaseMaxHitPoints(2);
    player.Heal(2);
}

void PickupRangeUpgrade::Apply(Player &player, WeaponInventory &) const {
    player.IncreasePickupRadius(28.0F);
}

void WeaponDamageUpgrade::Apply(Player &, WeaponInventory &weapons) const {
    weapons.IncreaseDamage(1);
}

void WeaponCooldownUpgrade::Apply(Player &, WeaponInventory &weapons) const {
    weapons.MultiplyCooldown(0.86F);
}
