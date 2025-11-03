//
// Created by Yasmine Slamani on 02/10/2025.
//

#include "core/entity.h"
#include "core/error_codes.h"
#include "core/effect.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// ========== STAT FUNCTIONS ==========

void stat_init(Stat *stat, int default_value) {
    stat->base_value = default_value;
    stat->cached_value = default_value;
    stat->to_calculate = false;
    stat->modifier_count = 0;
    stat->modifier_capacity = 10;

    stat->modifiers = calloc(stat->modifier_capacity, sizeof(StatModifier));

    if (stat->modifiers == NULL) {
        stat->modifier_capacity = 0;
    }
}

void free_stat(Stat *stat) {
    if (stat == NULL) return;

    if (stat->modifiers != NULL) {
        free(stat->modifiers);
        stat->modifiers = NULL;
    }

    stat->modifier_count = 0;
    stat->modifier_capacity = 0;
}

int stat_get_value(Stat *stat) {
    if (stat == NULL) return 0;

    // Use cached value if it's still fresh
    if (!stat->to_calculate) {
        return stat->cached_value;
    }

    // Cache is stale we recalculate
    float result = (float) stat->base_value;

    // Apply all FLAT based modifiers
    for (int i = 0; i < stat->modifier_count; i++) {
        if (stat->modifiers[i].type == MOD_FLAT) {
            result += stat->modifiers[i].value;
        }
    }

    // Apply all PERCENTAGE based modifiers (based on BASE value)
    float percentage_sum = 0.0f;
    for (int i = 0; i < stat->modifier_count; i++) {
        if (stat->modifiers[i].type == MOD_PERCENTAGE) {
            percentage_sum += stat->modifiers[i].value;
        }
    }
    result += (float) stat->base_value * percentage_sum;

    // Step 3: Update cache and mark clean
    stat->cached_value = (int) result;
    stat->to_calculate = false;

    return stat->cached_value;
}

void stat_modifier_add(Stat *stat, ModifierType type, void *source, float value) {
    // Check if array is full and needs expansion
    if (stat->modifier_count >= stat->modifier_capacity) {
        int new_capacity = stat->modifier_capacity == 0 ? 4 : stat->modifier_capacity * 2;
        StatModifier *new_modifiers = realloc(stat->modifiers,
                                              new_capacity * sizeof(StatModifier));

        if (new_modifiers == NULL) {
            // Allocation failed
            // throw error
            fprintf(stderr, "Avertissement: Impossible d'etendre le tableau de modificateurs\n");
            return; // don't right into stat the null pointer
        }
        stat->modifiers = new_modifiers;
        stat->modifier_capacity = new_capacity;
    }

    stat->modifiers[stat->modifier_count].value = value;
    stat->modifiers[stat->modifier_count].type = type;
    stat->modifiers[stat->modifier_count].source = source;
    stat->modifier_count++;

    stat->to_calculate = true;
}


void stat_modifier_remove_by_source(Stat *stat, Effect *source) {
    if (stat == NULL || stat->modifiers == NULL) return;

    int write_index = 0;
    for (int read_index = 0; read_index < stat->modifier_count; read_index++) {
        // we compare pointers directly
        if (stat->modifiers[read_index].source != source) {
            // overwrites precedent value, so erases the one we don't want
            if (write_index != read_index) {
                stat->modifiers[write_index] = stat->modifiers[read_index];
            }
            // increment only on values we want to keep
            write_index++;
        }
    }

    // we flag to recalculate the stat if we removed a modifier
    if (write_index != stat->modifier_count) {
        stat->modifier_count = write_index;
        stat->to_calculate = true;
    }
}

void stat_modifier_update_source(Stat *stat, Effect *old_source, Effect *new_source) {
    if (stat == NULL || stat->modifiers == NULL) return;

    // Iterate through all modifiers and update pointers
    for (int i = 0; i < stat->modifier_count; i++) {
        if (stat->modifiers[i].source == old_source) {
            stat->modifiers[i].source = new_source;
        }
    }
}

// ========== ENTITY FUNCTIONS ==========

EntityBase create_entity_base(EntityType type, char *name, int max_hp, int base_defense, int speed, int attack) {
    EntityBase base = {0}; // Zero-initialize

    base.type = type;
    if (name != NULL) {
        strncpy(base.name, name, sizeof(base.name) - 1);
        base.name[sizeof(base.name) - 1] = '\0';
    }

    int oxygen_level = 100;

    // Initialize stats
    stat_init(&base.attack, attack);
    stat_init(&base.defense, base_defense);
    stat_init(&base.speed, speed);

    base.max_health_points = max_hp;
    base.max_oxygen_level = oxygen_level;

    // Initialize resources
    base.current_health_points = max_hp;
    base.oxygen_level = oxygen_level;
    base.fatigue_level = 0;

    base.effects_number = 0;
    base.is_alive = 1;

    return base;
}

void free_entity_base(EntityBase *base) {
    if (base == NULL) return;

    // Clean up all effects
    for (int i = 0; i < base->effects_number; i++) {
        Effect *effect = &base->effects[i];

        // Remove modifiers if still active
        if (effect->is_active) {
            stat_modifier_remove_by_source(&base->attack, effect);
            stat_modifier_remove_by_source(&base->defense, effect);
            stat_modifier_remove_by_source(&base->speed, effect);
        }

        // Free display message
        free_effect_content(effect);
    }
    base->effects_number = 0;

    // Clean up stats
    free_stat(&base->attack);
    free_stat(&base->defense);
    free_stat(&base->speed);
}

int entity_take_damage(EntityBase *base, int hp) {
    if (base == NULL) return POINTER_NULL;
    base->current_health_points -= hp;
    if (base->current_health_points <= 0) {
        base->current_health_points = 0;
        base->is_alive = 0;
    }
    return SUCCESS;
}

int entity_recover_hp(EntityBase *base, int hp) {
    if (base == NULL) return POINTER_NULL;
    if (base->is_alive == 0) return UNPROCESSABLE_REQUEST_ERROR;

    int new_value = base->current_health_points + hp;
    int max_hp = base->max_health_points;
    if (new_value > max_hp) {
        base->current_health_points = max_hp;
        return SUCCESS_SATURATED; // saturated
    }
    base->current_health_points = new_value;
    return SUCCESS;
}

void print_current_effect_list(EntityBase *target) {
    printf("Liste d'effets de %s :\n", target->name);
    for (int i = 0; i < target->effects_number; i++) {
        printf(" %s avec %d tours restants\n", target->effects[i].name, target->effects[i].turns_left);
    }
}

void print_current_stat_modifier_list(EntityBase* base) {
    printf("Attaque de %s composee de ces modificateurs:\n", base->name);
    for (int i = 0; i < base->attack.modifier_count; i++) {
        printf("valeur : %.2lf, type : %s\n", base->attack.modifiers[i].value, base->attack.modifiers[i].type == MOD_FLAT ? "PLAT" : "POURCENT");
    }

    printf("\nDefense de %s composee de ces modificateurs:\n", base->name);
    for (int i = 0; i < base->defense.modifier_count; i++) {
        printf("valeur : %.2lf, type : %s\n", base->defense.modifiers[i].value, base->defense.modifiers[i].type == MOD_FLAT ? "PLAT" : "POURCENT");
    }
}