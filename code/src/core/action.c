//
// Created by Yasmine Slamani on 03/10/2025.
//

#include "../../includes/core/action.h"

#include <string.h>

#include "core/entity.h"
#include "core/error_codes.h"

static int last_replaced = -1;

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
            // handle adding to the array even when max is reached
            return insert_effect_in_effects(target, effect);
        }
        target->effects[target->effects_number] = effect_copy(&effect);
        target->effects_number++;
        return SUCCESS;
    }
    if (effect.on_tick != NULL) {
        return effect.on_tick(target);
    } else {
        return UNPROCESSABLE_REQUEST_ERROR;  // fallback
    }
}

int insert_effect_in_effects(EntityBase* target, Effect effect) {
    if (target == NULL) return POINTER_NULL;
    // looking for an inactive slot to replace
    for (int i = 0; i < target->effects_number; i++) {
        if (!target->effects[i].is_active) {
            free_effect_content(&target->effects[i]);
            target->effects[i] = effect_copy(&effect);
            return SUCCESS;
        }
    }

    // replace oldest effect
    last_replaced = (last_replaced + 1) % MAX_EFFECTS;

    if (target->effects[last_replaced].on_remove != NULL) {
        target->effects[last_replaced].on_remove(target);
    }

    free_effect_content(&target->effects[last_replaced]);
    target->effects[last_replaced] = effect_copy(&effect);
    return SUCCESS;
}