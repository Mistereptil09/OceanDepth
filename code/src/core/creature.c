//
// Created by Yasmine Slamani on 03/10/2025.
//

#include "../../includes/core/creature.h"


#include <stdlib.h>
#include <string.h>

#include "core/creature_data.h"

Creature *create_creature(int id, CreatureType type, EntityBase base, Action actions[MAX_ACTIONS]) {
    Creature *creature = malloc(sizeof(Creature));
    if (creature == NULL) return NULL;
    creature->id = id;
    creature->type = type;
    creature->base = base;
    if (actions != NULL) {
        for (int i = 0; i < MAX_ACTIONS; i++) {
            creature->base.actions[i] = actions[i];
        }
    } else {
        for (int i = 0; i < MAX_ACTIONS; i++) {
            memset(&creature->base.actions[i], 0, sizeof(Action));
        }
    }

    return creature;
}

void free_creature(Creature *c) {
    if (c == NULL) return;

    // Free all effect display_messages in actions
    for (int i = 0; i < MAX_ACTIONS; i++) {
        free_effect_content(&c->base.actions[i].effect);
    }

    // Free the entity base (this also frees active effects and stats)
    free_entity_base(&c->base);

    free(c);
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

void free_generated_creatures(Creature **creatures, int count) {
    if (!creatures) return;
    for (int i = 0; i < count; i++) {
        if (creatures[i] == NULL) continue;
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

    int hp = t->min_hp + rand() % (t->max_hp - t->min_hp + 1);
    int defense = t->defense;
    int speed = t->speed;

    // Use the template's name instead of generic "Creature"
    EntityBase base = create_entity_base(ENTITY_CREATURE, t->name, hp, defense, speed, 10);

    Action template_actions[MAX_ACTIONS];
    for (int i = 0; i < MAX_ACTIONS; i++) {
        template_actions[i] = t->actions[i];
        template_actions[i].effect = effect_copy(&t->actions[i].effect);
        /**if (template_actions[i].type == PHYSICAL_ATTACK) {
            template_actions[i].effect.hp_cost = random_range(t->min_atk, t->max_atk);
        } not all attacks have hp cost, some just use modifiers, hp costs are an "extra" and should be predetermined in the template */
    }

    return create_creature(id, t->type, base, template_actions);
}

Action* select_action(Creature *c) {
    if (c == NULL) return NULL;

    Action* available_actions[MAX_ACTIONS];
    int available_count = 0;

    for (int i = 0; i < c->base.action_count; i++) {
        Action* action = &c->base.actions[i];

        // Skip actions that are on cooldown
        if (action->cooldown_remaining > 0) {
            continue;
        }

        if (action->type == PHYSICAL_ATTACK) {
            available_actions[available_count++] = action;
        }
        else if (action->type == SPECIAL_SKILL) {
            // Don't reapply SPECIAL_SKILL if the effect is already active
            int effect_active = 0;
            for (int j = 0; j < c->base.effects_number; j++) {
                if (strcmp(c->base.effects[j].name, action->effect.name) == 0 &&
                    c->base.effects[j].is_active) {
                    effect_active = 1;
                    break;
                    }
            }

            if (!effect_active) {
                available_actions[available_count++] = action;
            }
        }
    }

    if (available_count == 0) return NULL;

    int choice = rand() % available_count;
    return available_actions[choice];
}