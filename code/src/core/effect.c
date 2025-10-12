//
// Created by Yasmine Slamani on 04/10/2025.
//

#include "core/effect.h"

#include <stdlib.h>
#include <string.h>


// todo include effect triggering here



Effect create_effect(const char* name, const char *display_message, const int turns, const int hp_cost,
    const int attack_boost, const int defense_boost, const int oxygen_boost, const int speed_boost)
{
    Effect effect = {0};

    if (name != NULL) {
        strncpy(effect.name, name, sizeof(effect.name) - 1);
        effect.name[sizeof(effect.name) - 1] = '\0';  // Ensure null termination
    }

    if (display_message != NULL) {
        effect.display_message = malloc(strlen(display_message) + 1);
        if (effect.display_message != NULL) {
            strcpy(effect.display_message, display_message);
        }
    } else {
        effect.display_message = NULL;
    }

    effect.turns_left = turns;
    effect.hp_cost = hp_cost;
    effect.attack_boost = attack_boost;
    effect.defense_boost = defense_boost;
    effect.oxygen_boost = oxygen_boost;
    effect.speed_boost = speed_boost;
    effect.is_active = 1;

    return effect;
}

Effect effect_copy(const Effect* src) {
    if (src == NULL) {
        Effect empty = {0};
        return empty;
    }

    Effect copy = *src;  // Copy all fields

    // Duplicate the display_message
    if (src->display_message != NULL) {
        copy.display_message = malloc(strlen(src->display_message) + 1);
        if (copy.display_message != NULL) {
            strcpy(copy.display_message, src->display_message);
        }
    } else {
        copy.display_message = NULL;
    }

    return copy;
}

void free_effect_content(Effect *effect) {
    if (effect == NULL) return;
    if (effect->display_message != NULL) {
        free(effect->display_message);
        effect->display_message = NULL;
    }
}