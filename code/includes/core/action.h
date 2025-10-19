//
// Created by Yasmine Slamani on 03/10/2025.
//
#ifndef OCEANDEPTH_ACTION_H
#define OCEANDEPTH_ACTION_H
#include "effect.h"

typedef enum {
    PHYSICAL_ATTACK,
    SPECIAL_SKILL,
} ActionType;

typedef struct {
    char name[30];
    ActionType type;
    int cooldown_turns;      // Base cooldown duration
    int cooldown_remaining;  // Current cooldown remaining (0 = ready to use)
    Effect effect;           // The effect the action applies on the receiver
} Action;

/**
* @brief Add an Action's effect to the target's Effect effects array
*   Checks if it's already active, non-active in the array, or simply not there.
* @param target Entity base pointer
 * @param effect
 * @return ERROR CODES
 */
Effect *apply_effect_to_target(EntityBase *target, Effect effect);

Effect *apply_action_to_target(EntityBase *target, Action *action);

/**
* @brief If array is full, tries to replace an inactive effect with the new one
*   Otherwise gets rid of the oldest effect (needs to remove effect before doing so)
* @param target Entity base pointer
* @param effect
 * @return ERROR CODES
 */
Effect *insert_effect_in_effects(EntityBase* target, Effect effect);

#endif //OCEANDEPTH_ACTION_H
