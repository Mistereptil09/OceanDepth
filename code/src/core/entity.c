//
// Created by Yasmine Slamani on 02/10/2025.
//

#include "core/entity.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void entity_take_damage(EntityBase *base, int hp) {
    base->current_health_points -= hp;
    if (base->current_health_points < 0) {
        base->current_health_points = 0;
    }
}

int entity_recover_hp(EntityBase *base, int hp) {
    int new_value = base->current_health_points + hp;
    if (new_value > base->max_health_points) {
        base->current_health_points = base->max_health_points;
        return -1; // saturated
    }
    base->current_health_points = new_value;
    return 0;
}

EntityBase create_entity_base(EntityType type, char* name, int max_hp, int base_defense) {
    EntityBase base = {0};

    base.type = type;
    if (name != NULL) {
        strncpy(base.name, name, sizeof(base.name) - 1);
        base.name[sizeof(base.name) - 1] = '\0';
    }

    base.max_health_points = max_hp;
    base.current_health_points = max_hp;
    base.base_attack = 0;
    base.current_attack = 0;
    base.base_defense = base_defense;
    base.current_defense = base_defense;

    base.effects_number = 0;
    base.effects = NULL;
    base.is_alive = 1;

    return base;
}