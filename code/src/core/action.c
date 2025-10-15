//
// Ta fonction adaptée pour le système hybride
//

#include "../../includes/core/action.h"

#include <stdio.h>
#include <string.h>
#include "core/entity.h"
#include "core/error_codes.h"
#include "core/effect.h"

static int last_replaced = -1;

int apply_action_to_target(EntityBase *target, Action action) {
    if (target == NULL) return POINTER_NULL;
    Effect effect = action.effect;
    return apply_effect_to_target(target, effect);
}

int apply_effect_to_target(EntityBase *target, Effect effect) {
    if (target == NULL) return POINTER_NULL;

    // DEBUG: Print who is getting the effect
    printf("[DEBUG] Applying effect '%s' (hp_cost=%d) to %s\n",
           effect.name, effect.hp_cost, target->name);

    for (int i = 0; i < target->effects_number; i++) {
            if (strcmp(target->effects[i].name, effect.name) == 0) {
                if (target->effects[i].is_active) {
                    return ALREADY_ACTIVE;
                }
                // reactivate old effect
                target->effects[i].is_active = 1;
                target->effects[i].turns_left = effect.turns_left;

                // apply effect if simple effect
                effect_apply(target, &target->effects[i]);
                return SUCCESS;
            }
        }

        // else, add new effect to list
        if (target->effects_number >= MAX_EFFECTS) {
            return insert_effect_in_effects(target, effect);
        }

        target->effects[target->effects_number] = effect_copy(&effect);
        target->effects_number++;

        effect_apply(target, &target->effects[target->effects_number - 1]);

        return SUCCESS;
}

int insert_effect_in_effects(EntityBase* target, Effect effect) {
    if (target == NULL) return POINTER_NULL;

    // look for an inactive slot
    for (int i = 0; i < target->effects_number; i++) {
        if (!target->effects[i].is_active) {
            effect_remove(target, &target->effects[i]);
            free_effect_content(&target->effects[i]);

            // insert new effect
            target->effects[i] = effect_copy(&effect);
            effect_apply(target, &target->effects[i]);
            return SUCCESS;
        }
    }

    // replace oldest
    last_replaced = (last_replaced + 1) % MAX_EFFECTS;

    effect_remove(target, &target->effects[last_replaced]); // REMOVE EFFECT
    free_effect_content(&target->effects[last_replaced]);

    target->effects[last_replaced] = effect_copy(&effect);
    effect_apply(target, &target->effects[last_replaced]);

    return SUCCESS;
}