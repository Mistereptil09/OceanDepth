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
                     // flat modifiers
                     int attack_boost_flat, int defense_boost_flat, int speed_boost_flat,
                     int oxygen_max_boost_flat, int hp_max_boost_flat,
                     // percent modifiers
                     float attack_boost_percent, float defense_boost_percent,float speed_boost_percent,
                     float oxygen_max_boost_percent, float hp_max_boost_percent,FonctionEffect on_tick, int applies_next_turn)
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

    if (applies_next_turn != 0 && applies_next_turn != 1) {
        effect.applies_next_turn = 0;
    } else {
        effect.applies_next_turn = applies_next_turn;
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
    if (effect->is_active) return;  // effect already applied

    if (effect->on_tick != NULL) {
        effect->is_active = 1;
        return;
    }

    // Apply FLAT modifiers
    if (effect->attack_boost_flat != 0) {
        printf("[DEBUG] FLAT ATK BOOST ADDED %d TO ENTITY : %s\n", effect->attack_boost_flat, base->name);
        stat_modifier_add(&base->attack, MOD_FLAT, effect, (float)effect->attack_boost_flat);
    }
    if (effect->defense_boost_flat != 0) {
        printf("[DEBUG] FLAT DEFENSE BOOST ADDED %d TO ENTITY : %s\n", effect->defense_boost_flat, base->name);
        stat_modifier_add(&base->defense, MOD_FLAT, effect, (float)effect->defense_boost_flat);
    }
    if (effect->speed_boost_flat != 0) {
        printf("[DEBUG] FLAT SPEED BOOST ADDED %d TO ENTITY : %s\n", effect->speed_boost_flat, base->name);
        stat_modifier_add(&base->speed, MOD_FLAT, effect, (float)effect->speed_boost_flat);
    }
    if (effect->hp_max_boost_flat != 0) {
        printf("[DEBUG] FLAT HP MAX BOOST ADDED %d TO ENTITY : %s\n", effect->hp_max_boost_flat, base->name);
        base->max_health_points += effect->hp_max_boost_flat;
    }
    if (effect->oxygen_max_boost_flat != 0) {
        printf("[DEBUG] FLAT OXYGEN BOOST ADDED %d TO ENTITY : %s\n", effect->oxygen_max_boost_flat, base->name);
        base->max_oxygen_level += effect->oxygen_max_boost_flat;
    }


    // Apply PERCENTAGE modifiers
    if (effect->attack_boost_percent != 0.0) {
        printf("[DEBUG] PERCENT ATTACK BOOST ADDED %lf TO ENTITY : %s\n", effect->attack_boost_percent, base->name);
        stat_modifier_add(&base->attack, MOD_PERCENTAGE, effect, effect->attack_boost_percent);
    }
    if (effect->defense_boost_percent != 0.0) {
        printf("[DEBUG] PERCENT DEFENSE BOOST ADDED %lf TO ENTITY : %s\n", effect->defense_boost_percent, base->name);
        stat_modifier_add(&base->defense, MOD_PERCENTAGE, effect, effect->defense_boost_percent);
    }
    if (effect->speed_boost_percent != 0.0) {
        printf("[DEBUG] PERCENT SPEED BOOST ADDED %lf TO ENTITY : %s\n", effect->speed_boost_percent, base->name);
        stat_modifier_add(&base->speed, MOD_PERCENTAGE, effect, effect->speed_boost_percent);
    }
    if (effect->hp_max_boost_percent != 0) {
        printf("[DEBUG] PERCENT HP BOOST ADDED %lf TO ENTITY : %s\n", effect->hp_max_boost_percent, base->name);
        base->max_health_points += (int)effect->hp_max_boost_percent * base->max_health_points;
    }
    if (effect->oxygen_max_boost_flat != 0) {
        printf("[DEBUG] PERCENT OXYGEN BOOST ADDED %lf TO ENTITY : %s\n", effect->oxygen_max_boost_percent, base->name);
        base->max_oxygen_level += (int)effect->oxygen_max_boost_percent * base->max_oxygen_level;
    }


    effect->is_active = 1;
    printf("[DEBUG] Modifier was successfully added \n");

}

void all_effects_tick(EntityBase* self, EntityBase* ennemy)
{
    // DEBUG: Show which entity's effects are ticking
    if (self->effects_number > 0) {
        printf("[DEBUG] Ticking %d effects on %s\n", self->effects_number, self->name);
    }

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
    // CRITICAL: When we move effects in the array, we must update the source pointers
    // in all stat modifiers, otherwise they'll point to stale/wrong effect addresses
    int write_index = 0;
    for (int read_index = 0; read_index < self->effects_number; read_index++) {
        if (self->effects[read_index].is_active ||
            self->effects[read_index].turns_left > 0) {
            if (write_index != read_index) {
                // We're about to move an effect from read_index to write_index
                // Save the old address before moving
                Effect* old_address = &self->effects[read_index];

                // Move the effect
                self->effects[write_index] = self->effects[read_index];

                // Update all modifier source pointers from old address to new address
                Effect* new_address = &self->effects[write_index];
                stat_modifier_update_source(&self->attack, old_address, new_address);
                stat_modifier_update_source(&self->defense, old_address, new_address);
                stat_modifier_update_source(&self->speed, old_address, new_address);
            }
            write_index++;
        } else {
            // Effect is expired - free its display_message before discarding
            // Note: modifiers should already be removed by effect_remove() in effect_tick()
            free_effect_content(&self->effects[read_index]);
        }
    }
    self->effects_number = write_index;
}

void effect_tick(EntityBase* self, EntityBase* ennemy, Effect* effect)
{
    if (!effect->is_active) return;

    if (effect->applies_next_turn == 1) {
        effect->applies_next_turn = 0;
        return;
    }

    if (effect->on_tick != NULL) {
          effect->on_tick(self, ennemy);
          printf("[DEBUG] Special on tick function was triggered by effect : %s\n", effect->name);
    } else {
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
    printf("[DEBUG] Effect now has %d turns left\n", effect->turns_left);


    // remove modifiers when effect expires
    if (effect->turns_left <= 0) {
            effect_remove(self, effect);  // This cleans up modifiers
    }
}

void effect_remove(EntityBase* base, Effect* effect)
{
    if (!effect->is_active) return;
    printf("[DEBUG] Removing %s from %s's effects\n", effect->name, base->name);

    if (effect->on_tick == NULL) {
        // Remove modifiers from this effect (using pointer)
        stat_modifier_remove_by_source(&base->attack, effect);
        stat_modifier_remove_by_source(&base->defense, effect);
        stat_modifier_remove_by_source(&base->speed, effect);
    }

    print_current_stat_modifier_list(base);

    effect->is_active = 0;
}