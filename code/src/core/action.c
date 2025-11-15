//
//
//

#include "../../includes/core/action.h"

#include <stdio.h>
#include <string.h>
#include "core/entity.h"
#include "core/error_codes.h"
#include "core/effect.h"

static int last_replaced = -1;

Effect *apply_action_to_target(EntityBase *target, Action *action) {
    if (target == NULL) return NULL;
    Effect effect = action->effect;
    return apply_effect_to_target(target, effect);
}

Effect *apply_effect_to_target(EntityBase *target, Effect effect) {
    Effect *p = NULL;
    if (target == NULL) return p;


        // add new effect to list
    if (target->effects_number >= MAX_EFFECTS) {
        p = insert_effect_in_effects(target, effect);
    }

    target->effects[target->effects_number] = effect_copy(&effect);
    target->effects_number++;

    effect_apply(target, &target->effects[target->effects_number - 1]);

    p = &target->effects[target->effects_number - 1];

    return p;
}

Effect *insert_effect_in_effects(EntityBase* target, Effect effect) {
    Effect *p = NULL;
    if (target == NULL) return p;

    // look for an inactive slot
    for (int i = 0; i < target->effects_number; i++) {
        if (!target->effects[i].is_active) {
            effect_remove(target, &target->effects[i]);
            free_effect_content(&target->effects[i]);

            // insert new effect
            target->effects[i] = effect_copy(&effect);
            effect_apply(target, &target->effects[i]);
            p = &target->effects[i];
            return p;
        }
    }

    // replace oldest
    last_replaced = (last_replaced + 1) % MAX_EFFECTS;

    effect_remove(target, &target->effects[last_replaced]);
    free_effect_content(&target->effects[last_replaced]);

    target->effects[last_replaced] = effect_copy(&effect);
    effect_apply(target, &target->effects[last_replaced]);

    p = &target->effects[last_replaced];
    return p;
}
