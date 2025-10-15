//
// Created by Yasmine Slamani on 03/10/2025.
//

#ifndef OCEANDEPTH_CREATURE_H
#define OCEANDEPTH_CREATURE_H
#define MAX_ACTIONS 4
#include "action.h"
#include "entity.h"
#include "effect.h"

/**
 * @brief Enum representing the different types of sea creatures.
 */
typedef enum {
    KRAKEN,  // hard
    KING_CRAB, // medium
    JELLYFISH, // easy
    SHARK, // medium
    LEVIATHAN, // boss
    ANGLERFISH, // hard
    SWORDFISH, // medium
    MEGALODON // boss
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
    char name[30];  // Display name for this creature type
    int min_hp, max_hp; // for random generation
    int min_atk, max_atk; // for random generation
    int defense;
    int speed;
    Action actions[MAX_ACTIONS]; /** Predefined actions, see creature_data */
} CreatureTemplate;

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
    // Actions now in base
} Creature;

/**
 * @brief Creates a new creature of the given type.
 * @param id
 * @param type The creature type (e.g., SHARK, KRAKEN...).
 * @param base
 * @param actions
 * @return Pointer to the allocated Creature, or NULL on failure.
 * @note This is a low-level function, logic will be handled by create_from_template()
 */
Creature *create_creature(int id, CreatureType type, EntityBase base,Action actions[MAX_ACTIONS]);

/**
 * @brief Frees memory allocated for a creature.
 * @param c Pointer to the Creature instance.
 */
void free_creature(Creature *c);


/**
 * @brief The function uses the global CREATURE_TEMPLATES array as a base to randomly select
 * creatures matching the desired power tier for the chosen difficulty level.
 * Each generated Creature will have:
 *  - Randomized stats within the min/max ranges defined in its template.
 *  - Properly initialized state (alive, empty active_effects, preset actions).
 * @param d The chosen difficulty level (EASY, MEDIUM, HARD, FINAL).
 *        - EASY  → 2 easy-tier creatures, 1 medium-tier creature
 *        - MEDIUM → 2 medium-tier creatures, 1 easy-tier creature
 *        - HARD  → 3 hard-tier creatures
 *        - FINAL → 1 boss-tier creature (Leviathan or Megalodon) + 1 easy-tier allies
 * @param count returns the size of the array
 * @return A dynamically allocated array of Creature pointers (typically size 3),
 *         each initialized and ready for battle.
 *         The array itself and its Creature elements must be freed using free_generated_creatures
 */
Creature **generate_creatures(Difficulty d, int *count);

/**
 * @param creatures The creature lineup generated for the combat by generate_creatures()
 * @param count The size of the array updated in the int count parameter of generate_creatures()
 */
void free_generated_creatures(Creature **creatures, int count);

/**
 * @brief Generates a random Creature matching the specified tier
   Randomizes stats within template ranges.
 * @param tier The CreatureTier to generate (defines the strength category).
 * @param id   Unique identifier of the creature within the current lineup.
 * @return Pointer to a newly allocated Creature, or NULL if allocation fails.
 * @note MUST seed srand time once at the entry point of the project
 */
Creature *create_from_template(CreatureTier tier, int id);

/**
 *
 * @param c Creature pointer
 * @return
 */
Action *select_action(Creature *c);

#endif //OCEANDEPTH_CREATURE_H