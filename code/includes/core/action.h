//
// Created by Yasmine Slamani on 03/10/2025.
//
#ifndef OCEANDEPTH_ACTION_H
#define OCEANDEPTH_ACTION_H
#include "effect.h"

typedef enum {
    PHYSICAL_ATTACK, // TARGET ENNEMY
    SPECIAL_SKILL, // TARGET SELF
    // ITEM_ATTACK (unnecessary)
} ActionType;

typedef struct {
    char name[30];
    ActionType type;
    int cooldown_turns;      // Base cooldown duration
    int cooldown_remaining;  // Current cooldown remaining (0 = ready to use)
    Effect effect;           // The effect the action applies on the receiver
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

int insert_effect_in_effects(EntityBase* target, Effect effect);

#endif //OCEANDEPTH_ACTION_H
