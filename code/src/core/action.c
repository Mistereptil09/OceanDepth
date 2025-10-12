//
// Created by Yasmine Slamani on 03/10/2025.
//

#include "../../includes/core/action.h"

#include <string.h>

#include "core/entity.h"
#include "core/error_codes.h"

int apply_effect_to_target(EntityBase *target, Action action) {
    if (target == NULL) return POINTER_NULL;
    Effect effect = action.effect;

    // si c'est special skill
    if (action.type == SPECIAL_SKILL) {
        Effect *active_effects = target->effects;
        for (int i = 0; i < target->effects_number; i++) {
            if (strcmp(active_effects[i].name, effect.name) == 0) {
                if (active_effects[i].is_active) {
                    return ALREADY_ACTIVE; // already active
                }
               // if not active, reactivate it
                active_effects[i].is_active = 1;
                active_effects[i].turns_left = effect.turns_left;
                return SUCCESS;
            }
        }
        // effect not found
        if (target->effects_number >= MAX_EFFECTS) {
            // handle adding to the array even when max is reached (looking for !is_active to replace, otherwise
            // replace or keep untouched)
        }
        target->effects[target->effects_number] = effect_copy(&effect);
        target->effects_number++;
        return SUCCESS;
    }
    // si c'est physical attack
    effect_tick(target, &effect);
    return SUCCESS;
}
