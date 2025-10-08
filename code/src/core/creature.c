//
// Created by Yasmine Slamani on 03/10/2025.
//

#include "../../includes/core/creature.h"


#include <stdlib.h>
#include <string.h>

#include "core/creature_data.h"

Creature *create_creature(int id, CreatureType type, EntityBase base, Action actions[2], int speed) {
    Creature *creature = malloc(sizeof(Creature));
    if (creature == NULL) return NULL;
    creature->id = id;
    creature->type = type;
    creature->base = base;
    creature->speed = speed;
    if (actions != NULL) {
        for (int i = 0; i < 2; i++) { // might want to have an action_count in creature_data's template
            creature->creature_actions[i] = actions[i];
        }
    } else {
        for (int i = 0; i < 2; i++) {
            memset(&creature->creature_actions[i], 0, sizeof(Action));
        }
    }

    return creature;
}

void free_creature(Creature *c) {
    if (c == NULL) return;

    if (c->base.effects != NULL) {
        free(c->base.effects);
        c->base.effects = NULL;
    }

    free(c);
}


// made dirty for now
/*Creature* create_from_template(CreatureTier tier, int id)
{
    CreatureType type = rand() % 8;
    EntityBase base;
    base.type = ENTITY_CREATURE;
    return create_creature(id, type, base, NULL);
}*/

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

void free_generated_creatures(Creature **creatures, int count) {
    if (!creatures) return;
    for (int i = 0; i < count; i++) {
        free_creature(creatures[i]);
    }
    free(creatures);
}

Creature *create_from_template(CreatureTier tier, int id) {
    const CreatureTemplate *templates = get_creature_templates();
    int template_count = get_creature_template_count();

    int count = 0;
    int indexes[template_count];

    // get all indexes where Creatures match CreatureTier
    for (int i = 0; i < template_count; i++) {
        if (tier == templates[i].tier) {
            indexes[count++] = i;
        }
    }
    if (count == 0) return NULL;

    // random select of one of these indexes (seed srand time once at the beginning of the program)
    int random_index = indexes[rand() % count];
    const CreatureTemplate *t = &templates[random_index];

    // Random select of stats within range
    int hp = t->min_hp + rand() % (t->max_hp - t->min_hp + 1);
    int defense = t->defense;
    int speed = t->speed;

    // Create EntityBase using the new function
    EntityBase base = create_entity_base(ENTITY_CREATURE, "Creature", hp, defense);


    // Copy template actions
    Action template_actions[2];
    for (int i = 0; i < 2; i++) {
        template_actions[i] = t->actions[i];
    }

    // Create creature with template actions
    return create_creature(id, t->type, base, template_actions, speed);
}