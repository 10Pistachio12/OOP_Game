# OOP Vampire Survivors Prototype

This project is a C++ OOP game prototype built on the
[PTSD](https://github.com/ntut-open-source-club/practical-tools-for-simple-design)
framework. The current design follows the core loop of Vampire Survivors:
move, survive enemy waves, auto-fire weapons, collect XP gems, and choose
upgrades on level up.

## Current Features

- WeaponInventory supports multiple automatic weapons.
- Auto-targeting Magic Bolt weapon and radial Arcane Nova burst weapon.
- Slime, Bat, and Brute enemy classes derived from a shared Enemy base.
- EnemyDirector controls timed phases, spawn rate, batch size, and enemy mix.
- XP gems are magnetized toward the player within pickup range.
- Level-up upgrade choices for movement, health, pickup range, all-weapon damage, and all-weapon cooldown.
- HUD shows HP, level, XP, survival time, wave phase, and enemy count.

## Controls

- Move: WASD or arrow keys
- Choose upgrade: 1, 2, 3
- Restart after game over: R
- Exit: ESC

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
