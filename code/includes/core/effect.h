//
// Created by Yasmine Slamani on 04/10/2025.
//

#ifndef OCEANDEPTH_EFFECT_H
#define OCEANDEPTH_EFFECT_H
#include "entity.h"

typedef struct Effect Effect;

typedef struct Effect {
    char name[30];
    int turns_left;

    // DAMAGE
    int hp_cost;
    int oxygen_cost;
    int defense_cost;

    // HEALTH BOOST
    int attack_boost;
    int defense_boost;
    int oxygen_boost;
    int speed_boost;

    int is_active;
} Effect;

/**
 * @brief Applies and remove effect on target
 * @param target Entity base pointer
 * @param effect Effect pointer
 */
void generic_tick(EntityBase* target, Effect* effect);
void generic_remove(EntityBase* target, Effect* effect);

#endif //OCEANDEPTH_EFFECT_H