//
// Created by Yasmine Slamani on 02/10/2025.
//

#ifndef OCEANDEPTH_ENTITY_H
#define OCEANDEPTH_ENTITY_H
#define MAX_EFFECTS 10
#define MAX_ACTIONS 4

#include <stdbool.h>

#include "action.h"
#include "effect.h"

typedef enum {
    ENTITY_PLAYER,
    ENTITY_CREATURE
} EntityType;

/** @brief Modifier types for different calculation methods **/
typedef enum {
    MOD_FLAT,        // Direct addition: +10 attack
    MOD_PERCENTAGE   // Percentage of base: +20% attack
} ModifierType;

// Allows to track which effect added which modifier
typedef struct {
    float value;         // The amount: +10 or 0.20 (for 20%)
    ModifierType type;   // How to apply it: flat addition or percentage
    Effect* source;        // Which effect created this (for removal)
} StatModifier;

// allows to calculate the effect modified stats
typedef struct {
    int base_value; // entity initial value for that stat
    int cached_value; // value after all modifiers were applied
    int current_value; // a copy of cached_value that can be modified by functions that aren't tracked by StatModifier
    bool to_calculate;
    StatModifier* modifiers;
    int modifier_count;
    int modifier_capacity;
} Stat;

typedef struct EntityBase{
    EntityType type;
    char name[30];

    // stats that are modified by effects
    Stat attack;
    Stat defense;
    // Creature-specific stats (can be used by player for movement)
    Stat speed;

    // Resources (health)
    int current_health_points;
    int max_health_points;

    // Player-specific resources (unused by creatures)
    int fatigue_level;
    int oxygen_level;
    int max_oxygen_level;

    // Effects
    int effects_number;
    Effect effects[MAX_EFFECTS];

    // Actions
    Action actions[MAX_ACTIONS]; /** Predefined actions, see creature_data */
    int action_count;

    int is_alive;
} EntityBase;

/**
 * @brief initializes a stat
 * @param stat Stat pointer to initialize
 * @param default_value default value to put the stat at
 */
void stat_init(Stat* stat, int default_value);

/**
 * @brief frees up a stat pointer
 * @param stat Stat to free
 */
void free_stat(Stat* stat);

/**
 * Recalculates the stats with modifiers
 * @param stat Stat to get value of
 * @return returns a stat with is its modifiers
 */
int stat_get_value(Stat* stat);

/**
 * @brief Adds a stat modifier from a source (effect)
 * @param stat Stat to modify
 * @param type Modifier type (flat or percentage)
 * @param source Pointer to source (usually an Effect*)
 * @param value Modifier value
 */
void stat_modifier_add(Stat* stat, ModifierType type, void* source, float value);

/**
 * @brief Removes all modifiers from a specific source
 * @param stat Stat to clean up
 * @param source Pointer to source (usually an Effect*)
 */
void stat_modifier_remove_by_source(Stat* stat, Effect* source);

/**
 * @brief Applies damage to the Entity's HP.
 * @param base Pointer to the Entity base.
 * @param hp Amount of damage to apply (must be >= 0).
 * @note HP cannot drop below 0.
 */
int entity_take_damage(EntityBase *base, int hp);

/**
 * @brief Recovers Entity HP.
 * @param base Pointer to the Entity base.
 * @param hp Amount of HP recovered (must be >= 0).
 */
int entity_recover_hp(EntityBase *base, int hp);

EntityBase create_entity_base(EntityType type, char* name, int max_hp, int base_defense, int speed);

void free_entity_base(EntityBase* base);



#endif //OCEANDEPTH_ENTITY_H
