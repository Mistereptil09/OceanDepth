//
// Created by Yasmine Slamani on 03/10/2025.
//

#ifndef OCEANDEPTH_CREATURE_H
#define OCEANDEPTH_CREATURE_H
#define CREATURE_TEMPLATE_COUNT (sizeof(CREATURE_TEMPLATES) / sizeof(CreatureTemplate))
#include "action.h"
#include "entity.h"
#include "effect.h"

/**
 * @brief Enum representing the different types of sea creatures.
 */
typedef enum {
    KRAKEN,
    KING_CRAB,
    JELLYFISH,
    SHARK,
    LEVIATHAN,
    ANGLERFISH,
    SWORDFISH,
    MEGALODON
} CreatureType;

/**
 * @brief Enum representing the tier of each sea creature type.
 */
typedef enum {
    CREATURE_EASY,
    CREATURE_MEDIUM,
    CREATURE_HARD,
    CREATURE_BOSS
} CreatureTier;


/**
 * @brief Power system template structure
 */
typedef struct {
    CreatureType type;
    CreatureTier tier;
    int min_hp, max_hp;
    int min_atk, max_atk;
    int defense;
    int speed;
} CreatureTemplate;

/**
 *  @brief Power system template
*   Kraken : HIGH HP (120-180), STRONG ATK (25-40), MODERATE DEF (10), MODERATE SPEED (2/3)
    Shark : MODERATE HP (60-100 PV), MODERATE ATK (15-25 ATK),  HIGH DEF (20), HIGH SPEED (1/3)
    Jellyfish : LOW HP (20-40 PV), LOW ATK (8-15 ATK),  LOW DEF (5), LOW SPEED (3/3)
    Swordfish : MODERATE HP (70-90 PV), LOW TO MODERATE ATK (18-28 ATK), MODERATE DEF (15), HIGH SPEED (1/3)
    King Crab : HIGH HP (80-120 PV), LOW ATK (12-20 ATK),  MODERATE DEF (10), MODERATE SPEED (2/3)
    Anglerfish : HIGH HP (100-180 PV), LOW ATK (12-20 ATK),  MODERATE DEF (15), LOW SPEED (3/3)
    Leviathan & Megalodon : HIGH HP (210-230 PV), HIGH ATK (35-45 ATK),  MODERATE DEF (15), HIGH SPEED (1/3)
 */
static const CreatureTemplate CREATURE_TEMPLATES[] = {
    {KRAKEN, CREATURE_HARD, 120, 180, 25, 40, 10, 2},
    {KING_CRAB, CREATURE_MEDIUM, 80, 120, 12, 20, 10, 2},
    {JELLYFISH, CREATURE_EASY, 20, 40, 8, 15, 5, 3},
    {SHARK, CREATURE_MEDIUM, 60, 100, 15, 25, 20, 1},
    {LEVIATHAN, CREATURE_BOSS, 210, 230, 45, 45, 15, 1},
    {ANGLERFISH, CREATURE_HARD, 180, 180, 12, 20, 15, 3},
    {SWORDFISH, CREATURE_MEDIUM, 70, 90, 18, 28, 15, 1},
    {MEGALODON, CREATURE_BOSS, 210, 230, 45, 45, 15, 1},
};


/**
 * @brief Enum representing the different difficulty levels of a set of creatures
 */
typedef enum {
    EASY,
    MEDIUM,
    HARD,
    FINAL,
} Difficulty;


/**
 * @brief Represents an enemy sea creature in the game.
 */
typedef struct {
    int id;
    CreatureType type;
    EntityBase base;
    Action creature_actions[5]; // available actions (unlike Player, doesn't have to attack through items)
} Creature;


/**
 * @brief Creates a new creature of the given type.
 * @param id
 * @param type The creature type (e.g., SHARK, KRAKEN...).
 * @param base
 * @param actions
 * @return Pointer to the allocated Creature, or NULL on failure.
 * @note This is a low-level function, logic will be handled by generate_creatures()
 */
Creature *create_creature(int id, CreatureType type, EntityBase base,Action actions[5]);

/**
 * @brief Frees memory allocated for a creature.
 * @param c Pointer to the Creature instance.
 */
void free_creature(Creature *c);

/**
 * @brief Applies damage to the creature.
 * @param c Pointer to the Creature instance.
 * @param damage Amount of damage to apply (must be >= 0).
 * @note Sets is_alive to 0 if HP reaches 0.
 */
void creature_take_damage(Creature *c, int damage);

/**
 * @brief Generates a lineup of creatures for a combat based on the specified difficulty level.
 *
 * The function uses the global CREATURE_TEMPLATES array as a base to randomly select
 * creatures matching the desired power tier(s) for the chosen difficulty level.
 *
 * Each generated Creature will have:
 *  - Randomized stats within the min/max ranges defined in its template.
 *  - Properly initialized state (alive, empty active_effects, preset actions).
 *
 * @param d The chosen difficulty level (EASY, MEDIUM, HARD, FINAL).
 *        - EASY  → 2 easy-tier creatures, 1 medium-tier creature
 *        - MEDIUM → 2 medium-tier creatures, 1 easy-tier creature
 *        - HARD  → 3 hard-tier creatures
 *        - FINAL → 1 boss-tier creature (Leviathan or Megalodon) + 1 easy-tier allies
 * @param count returns the size of the array
 *
 * @return A dynamically allocated array of Creature pointers (typically size 3),
 *         each initialized and ready for battle.
 *         The array itself and its Creature elements must be freed by the caller.
 *
 * @note This function abstracts the encounter generation logic.
 *       Serves as the entry point for initializing combat scenarios.
 */
Creature **generate_creatures(Difficulty d, int *count);

/**
 * @param creatures The creature lineup generated for the combat by generate_creatures()
 * @param count The size of the array updated in the int count parameter of generate_creatures()
 */
void free_generated_creatures(Creature **creatures, int count);

/**
 * @brief Creates a new Creature instance based on the given power tier.
 *
 * Generates a random Creature matching the specified tier
   Randomizes stats within template ranges.
 *
 * @param tier The CreatureTier to generate (defines the strength category).
 * @param id   Unique identifier of the creature within the current lineup.
 * @return Pointer to a newly allocated Creature, or NULL if allocation fails.
 */
Creature *create_from_template(CreatureTier tier, int id);

#endif //OCEANDEPTH_CREATURE_H

