# OOP Vampire Survivors Prototype

This project is a C++ OOP game prototype built on the
[PTSD](https://github.com/ntut-open-source-club/practical-tools-for-simple-design)
framework. The current design follows the core loop of Vampire Survivors:
move, survive enemy waves, auto-fire weapons, collect XP gems, and choose
upgrades on level up.

## Current Features

- WeaponInventory supports multiple automatic weapons.
- The player starts with Magic Bolt and can unlock Arcane Nova through level-up choices.
- Each weapon has levels and weapon-specific upgrade effects.
- Auto-targeting Magic Bolt weapon and radial Arcane Nova burst weapon.
- Slime, Bat, and Brute enemy classes derived from a shared Enemy base.
- EnemyDirector controls timed phases, spawn rate, batch size, and enemy mix.
- XP gems are magnetized toward the player within pickup range.
- Level-up choices include character stats, all-weapon bonuses, weapon unlocks, and weapon level-ups.
- HUD shows HP, level, XP, survival time, wave phase, enemy count, and weapon levels.

## Controls

- Move: WASD or arrow keys
- Choose upgrade: 1, 2, 3
- Restart after game over: R
- Exit: ESC

## Debug Controls

Debug builds enable test shortcuts for faster gameplay verification:

- T: Toggle debug HUD
- Y: Grant enough XP to trigger one level-up choice
- U: Unlock Arcane Nova
- I: Level up all owned weapons
- O: Spawn an enemy wave

## Quick Start

1. Use this template to create a new repository
   ![github screenshot](https://github.com/ntut-rick/ptsd-template/assets/126899559/ef62242f-03ed-481d-b858-12b730c09beb)

2. Clone your repository

   ```bash
   git clone YOUR_GIT_URL --recursive
   ```

3. Build your project

  > [!WARNING]
  > Please build your project in `Debug` because our `Release` path is broken D:
   
   ```sh
   cmake -DCMAKE_BUILD_TYPE=Debug -B build # -G Ninja
   cmake --build build -j 8
   ./build/VampireSurvivors
   ```
   better read [PTSD README](https://github.com/ntut-open-source-club/practical-tools-for-simple-design)
