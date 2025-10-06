//
// Created by Yasmine Slamani on 03/10/2025.
//

#include "../../includes/core/creature.h"


#include <stdlib.h>
#include <string.h>

Creature *create_creature(int id, CreatureType type, Stats stats,Action actions[5]) {
    Creature *creature = malloc(sizeof(Creature));
    if (creature == NULL) return NULL;
    creature->id = id;
    creature->type = type;
    creature->stats = stats;
    creature->is_alive = 1;
    creature->active_effects = NULL;
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
    if (c->active_effects != NULL) {
        free(c->active_effects);
    }
    free(c);
}

void take_damage(Creature *c, int damage) {
    c->stats.current_health_points -= damage;
    if (c->stats.current_health_points < 0) {
        c->stats.current_health_points = 0;
    }
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

Creature *create_from_template(CreatureTier tier, int id) {
    int count = 0;
    int indexes[CREATURE_TEMPLATE_COUNT];
    // get all indexes where Creatures match CreatureTier
    for (int i = 0; i < CREATURE_TEMPLATE_COUNT; i++) {
        if (tier == CREATURE_TEMPLATES[i].tier) {
            indexes[count++] = i;
        }
    }
    if (count == 0) return NULL;
    // random select of one of these indexes (seed srand time once at the beginning of the program)
    int random_index = indexes[rand() % count];
    const CreatureTemplate *t = &CREATURE_TEMPLATES[random_index];
    // Random select of stats within range
    Stats s;
    s.max_health_points = t->min_hp + rand() % (t->max_hp - t->min_hp + 1);
    s.current_health_points = s.max_health_points;
    s.base_attack = t->min_atk + rand() % (t->max_atk - t->min_atk + 1);
    s.current_attack = s.base_attack;
    s.base_defense = t->defense;
    s.current_defense = t->defense;
    s.speed = t->speed;

    // Create creature
    Action actions
    return create_creature(id, t->type, s, );
}