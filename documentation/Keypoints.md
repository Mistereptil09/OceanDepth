# 1. Project Scope

Underwater adventure game in C, fully text-based, modular.

Core mechanics: exploration, combat, resource management (HP, oxygen, fatigue), treasure collection, and progression.

Focus on memory management, input validation, and modular architecture.

---
# 2. Core Components / Modules
```
   Module	Responsibility
   main.c	Entry point; game loop management
   creatures.c/h	Creature creation, stats, attacks, special effects
   joueur.c/h	Player stats, actions, resources (HP, oxygen, fatigue)
   combat.c/h	Combat mechanics (player & creature turns)
   inventaire.c/h	Inventory, equipment, consumables
   carte.c/h	Map navigation, zone interactions
   sauvegarde.c/h	Save/load player state, inventory, creatures
   Makefile	Build management
```
---
# 3. Data Structures
   Player:
```c++
typedef struct {
    int points_de_vie;
    int points_de_vie_max;
    int niveau_oxygene;
    int niveau_oxygene_max;
    int niveau_fatigue; // 0-5
    int perles; // currency
} Plongeur;
```

Creature
```c++
typedef struct {
    int id;
    char nom[30];
    int points_de_vie_max;
    int points_de_vie_actuels;
    int attaque_minimale;
    int attaque_maximale;
    int defense;
    int vitesse;
    char effet_special[20]; // "paralysie", "poison", "aucun"
    int est_vivant;
} CreatureMarine;
```

*Inventory / Items*

Maximum 8 items

Types: equipment (harpon, combi), consumables (capsule O₂, soins, antidotes)

Store quantities and equipped status

---
# 4. Combat System

Turn-based, player vs 1-4 creatures.

Player actions:

- Attack (limited by fatigue)

- Use skill (with oxygen cost and cooldown)

- Use item

- End turn

- Fatigue effect: 0-1 → 3 attacks; 2-3 → 2 attacks; 4-5 → 1 attack

- Oxygen consumption: varies per action; critical alert at ≤10, damage at 0

- Creature attacks: based on speed; special effects may persist

---
# 5. Creature Generation

- Generate 1-4 creatures per depth

- Stats scale with depth

- Special effects: paralysis, poison, multiple attacks, defense reduction

- Manage empty slots in fixed-size arrays

---
# 6. Rewards System

- Loot: weapons, armor, consumables, currency (perles)

- Item stats: attack, defense, oxygen cost, special effects

- Interface: textual inventory with equip/unequip and usage options

- Constraints: Max 8 items, update equipped status, integrate with combat

---
# 7. Exploration / Map

- Text-based ocean map with zones (surface, reefs, wrecks, caves, trenches)

- Zones mechanics: depth affects oxygen consumption; some require equipment

- Actions: move, explore, return to surface, view detailed map

---
# 8. Skills System

- Skills consume oxygen, may have cooldowns

- Examples: Apnée Prolongée (+oxygen), Décharge Électrique (AOE damage), Communication Marine (pacify), Tourbillon Aquatique (debuff)

- Recover oxygen naturally or via consumables

---
# 9. Save/Load System

- Save full game state: player stats, inventory, position, creatures

- Only at surface or in caves

- File format: structured text/JSON/XML

- Single save per player

- Robust error handling required

---
# 10. Technical Constraints

- Memory management: no leaks, all allocations freed, check malloc/calloc return

- Input validation: robust, buffer overflows prevented, clear French messages

- C99 standard, modular architecture

- Compilation: via Makefile; run valgrind to verify

---
# 11. Testing & Validation

- Unit test each module

- Validate input consistently

- Test combat mechanics with edge cases

- Verify memory with valgrind --leak-check=full ./oceandepths

- Test save/load consistency