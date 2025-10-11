//
// Created by Yasmine Slamani on 03/10/2025.
//
#ifndef OCEANDEPTH_ACTION_H
#define OCEANDEPTH_ACTION_H
#include "effect.h"

typedef enum {
    PHYSICAL_ATTACK,
    SPECIAL_SKILL,
    // ITEM_ATTACK (unnecessary)
} ActionType;

typedef struct {
    char name[30];
    ActionType type;
    int cooldown; // should we do base vs current cooldown ?
    Effect effect; // The effect the action applies on the receiver
} Action;

/**
* @brief Applies the effect by :
    Either calling on_tick once if it's a one time action (initial turns_left = 1, after = 0)
     Or essentially creating a copy of it in the receivers active effects
* @param target Entity base pointer
 * @param action Action
 * @return 0 if applied,
 */
int apply_effect_to_target(EntityBase *target, Action action);

#endif //OCEANDEPTH_ACTION_H
