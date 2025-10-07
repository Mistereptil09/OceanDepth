//
// Created by Yasmine Slamani on 03/10/2025.
//

#include "../../includes/core/creature.h"


#include <stdlib.h>
#include <string.h>

Creature *create_creature(int id, CreatureType type, EntityBase base, Action actions[5]) {
    Creature *creature = malloc(sizeof(Creature));
    if (creature == NULL) return NULL;
    creature->id = id;
    creature->type = type;
    creature->base = base;
    if (actions != NULL) {
        for (int i = 0; i < 5; i++) {
            creature->creature_actions[i] = actions[i];
        }
    } else {
        for (int i = 0; i < 7; i++) {
            memset(&creature->creature_actions[i], 0, sizeof(Action));
        }
    }

    return creature;
}

void free_creature(Creature *c) {
    if (c == NULL) return;

    // Free the effects array if it exists
    if (c->base.effects != NULL) {
        free(c->base.effects);
        // depends on how we handle the effects
        // free_effects(c->base.effects, c->base.effects_number);
        c->base.effects = NULL;
    }

    // Free the creature itself
    free(c);
}

void creature_take_damage(Creature *c, int damage) {
    c->base.current_health_points -= damage;
    if (c->base.current_health_points < 0) {
        c->base.current_health_points = 0;
    }
}

// made dirty for now
Creature* create_from_template(CreatureTier tier, int id)
{
    CreatureType type = rand() % 8;
    EntityBase base;
    base.type = ENTITY_CREATURE;
    return create_creature(id, type, base, NULL);
}

Creature **generate_creatures(Difficulty d, int *count) {
    if (!count) return NULL;

    int size = (d == FINAL) ? 2 : 3;
    Creature **lineup = malloc(sizeof(Creature *) * size);
    if (!lineup) return NULL;

    *count = size;
    int id = 0;

    switch (d) {
        case EASY:
            lineup[0] = create_from_template(CREATURE_EASY, id++);
            lineup[1] = create_from_template(CREATURE_EASY,id++);
            lineup[2] = create_from_template(CREATURE_MEDIUM, id);
            break;

        case MEDIUM:
            lineup[0] = create_from_template(CREATURE_EASY, id++);
            lineup[1] = create_from_template(CREATURE_MEDIUM, id++);
            lineup[2] = create_from_template(CREATURE_HARD, id);
            break;

        case HARD:
            lineup[0] = create_from_template(CREATURE_HARD, id++);
            lineup[1] = create_from_template(CREATURE_HARD, id++);
            lineup[2] = create_from_template(CREATURE_MEDIUM, id);
            break;

        case FINAL:
            lineup[0] = create_from_template(CREATURE_BOSS, id++);
            lineup[1] = create_from_template(CREATURE_MEDIUM, id);
            break;
    }

    for (int i = 0; i < size; i++) {
        if (!lineup[i]) {
            for (int j = 0; j < i; j++) free_creature(lineup[j]);
            free(lineup);
            return NULL;
        }
    }

    return lineup;
}
