#include "Upgrade.hpp"

#include "Player.hpp"
#include "Weapon.hpp"

void MoveSpeedUpgrade::Apply(Player &player, Weapon &) const {
    player.IncreaseMoveSpeed(35.0F);
}

void MaxHealthUpgrade::Apply(Player &player, Weapon &) const {
    player.IncreaseMaxHitPoints(2);
    player.Heal(2);
}

void PickupRangeUpgrade::Apply(Player &player, Weapon &) const {
    player.IncreasePickupRadius(28.0F);
}

void WeaponDamageUpgrade::Apply(Player &, Weapon &weapon) const {
    weapon.IncreaseDamage(1);
}

void WeaponCooldownUpgrade::Apply(Player &, Weapon &weapon) const {
    weapon.MultiplyCooldown(0.86F);
}
