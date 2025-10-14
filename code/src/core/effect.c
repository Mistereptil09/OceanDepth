//
// Created by Yasmine Slamani on 04/10/2025.
//

#include "core/effect.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core/entity.h"

// ========== EFFECT HANDLING ==========
Effect create_effect(const char* name, const char *display_message, int turns,
                     // ressources
                     int hp_cost,  int oxygen_cost,
                     // flat modifiers
                     int attack_boost_flat, int defense_boost_flat, int speed_boost_flat,
                     int oxygen_max_boost_flat, int hp_max_boost_flat,
                     // percent modifiers
                     float attack_boost_percent, float defense_boost_percent,float speed_boost_percent,
                     float oxygen_max_boost_percent, float hp_max_boost_percent,FonctionEffect on_tick)
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

    // ressources
    effect.hp_cost = hp_cost;
    effect.oxygen_cost = oxygen_cost;

    // flat modifiers
    effect.attack_boost_flat = attack_boost_flat;
    effect.defense_boost_flat = defense_boost_flat;
    effect.speed_boost_flat = speed_boost_flat;
    effect.oxygen_max_boost_flat = oxygen_max_boost_flat;
    effect.hp_max_boost_flat = hp_max_boost_flat;

    // percent modifiers
    effect.attack_boost_percent = attack_boost_percent;
    effect.defense_boost_percent = defense_boost_percent;
    effect.speed_boost_percent = speed_boost_percent;
    effect.oxygen_max_boost_percent = oxygen_max_boost_percent;
    effect.hp_max_boost_percent = hp_max_boost_percent;

    effect.is_active = 0;

    effect.on_tick = NULL;

    if (on_tick != NULL) {
        effect.on_tick = on_tick;
    }

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

// ========== EFFECT LOGIC ==========

void effect_apply(EntityBase* base, Effect* effect)
{
    if (effect->is_active) return;  // simple effect already applied

    if (effect->on_tick != NULL) {
        effect->is_active = 1;
        return;
    }

    // Apply FLAT modifiers
    if (effect->attack_boost_flat != 0) {
        stat_modifier_add(&base->attack, MOD_FLAT, effect, (float)effect->attack_boost_flat);
    }
    if (effect->defense_boost_flat != 0) {
        stat_modifier_add(&base->defense, MOD_FLAT, effect, (float)effect->defense_boost_flat);
    }
    if (effect->speed_boost_flat != 0) {
        stat_modifier_add(&base->speed, MOD_FLAT, effect, (float)effect->speed_boost_flat);
    }

    // Apply PERCENTAGE modifiers
    if (effect->attack_boost_percent != 0.0) {
        stat_modifier_add(&base->attack, MOD_PERCENTAGE, effect, effect->attack_boost_percent);
    }
    if (effect->defense_boost_percent != 0.0) {
        stat_modifier_add(&base->defense, MOD_PERCENTAGE, effect, effect->defense_boost_percent);
    }
    if (effect->speed_boost_percent != 0.0) {
        stat_modifier_add(&base->speed, MOD_PERCENTAGE, effect, effect->speed_boost_percent);
    }

    effect->is_active = 1;
}

void all_effects_tick(EntityBase* self, EntityBase* ennemy)
{
    // Apply and tick active effects
    for (int i = 0; i < self->effects_number; i++) {
        Effect* effect = &self->effects[i];
        if (!effect->is_active && effect->turns_left > 0) {
            effect_apply(self, effect);
        }
        if (effect->is_active) {
            effect_tick(self, ennemy, effect);
        }
    }

    // Compact array: remove expired effects and free their memory
    int write_index = 0;
    for (int read_index = 0; read_index < self->effects_number; read_index++) {
        if (self->effects[read_index].is_active ||
            self->effects[read_index].turns_left > 0) {
            if (write_index != read_index) {
                self->effects[write_index] = self->effects[read_index];
            }
            write_index++;
        } else {
            // Effect is expired - free its display_message before discarding
            free_effect_content(&self->effects[read_index]);
        }
    }
    self->effects_number = write_index;
}

void effect_tick(EntityBase* self, EntityBase* ennemy, Effect* effect)
{
    if (!effect->is_active) return;

    if (effect->on_tick != NULL) {
          effect->on_tick(self, ennemy);
    } else {
        // Display message
        if (effect->display_message) {
            printf("%s\n", effect->display_message);
        }

        // Apply per-turn costs
        self->current_health_points -= effect->hp_cost;
        self->oxygen_level -= effect->oxygen_cost;

        // Clamp resources
        int max_hp = self->max_health_points;
        if (self->current_health_points > max_hp) {
            self->current_health_points = max_hp;
        }
        if (self->current_health_points <= 0) {
            self->current_health_points = 0;
            self->is_alive = 0;
        }

        int max_oxygen = self->max_oxygen_level;
        if (self->oxygen_level > max_oxygen) {
            self->oxygen_level = max_oxygen;
        }
        if (self->oxygen_level < 0) {
            self->oxygen_level = 0;
        }
    }
    effect->turns_left--;

    // remove modifiers when effect expires
    if (effect->turns_left <= 0) {
            effect_remove(self, effect);  // This cleans up modifiers
    }
}

void effect_remove(EntityBase* base, Effect* effect)
{
    if (!effect->is_active) return;

    // Remove all stat modifiers associated with this effect
    stat_modifier_remove_by_source(&base->attack, effect);
    stat_modifier_remove_by_source(&base->defense, effect);
    stat_modifier_remove_by_source(&base->speed, effect);

    effect->is_active = 0;
}