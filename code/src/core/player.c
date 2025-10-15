//
// Created by Yasmine Slamani on 02/10/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core/player.h"
#include "core/error_codes.h"
#include "core/entity.h"
/*
 * player.c = Player stats, actions, resources (HP, oxygen, fatigue, pearls, inventory)
 */

Player *create_player(char *name, int max_hp, int base_defense, int max_oxygen) {
    Player *p = malloc(sizeof(Player));
    if (p == NULL) return NULL;

    p->base = create_entity_base(ENTITY_PLAYER, name, max_hp, base_defense, 0);

    // entity base values (operate directly on p->base)
    p->base.oxygen_level = max_oxygen;
    p->base.max_oxygen_level = max_oxygen;
    p->base.fatigue_level = 0;

    // Initialize player actions
    p->base.actions[0] = (Action){
        "Coup d'épée", PHYSICAL_ATTACK, 0, 0,  // cooldown_turns, cooldown_remaining
        create_effect("Saignement", "Transpercé par la lame !",
            1,
            0, 0, 0, 0, 0,  // flat modifiers
            0, 0, 0, 0, 0,  // percentage modifiers
            NULL)
    };

    p->base.actions[1] = (Action){
        "Boost d'attaque", SPECIAL_SKILL, 2, 0,  // cooldown_turns, cooldown_remaining
        create_effect("Boost d'attaque", "Montée de puissance !",
            2,
            10, 0, 0, 0, 0,  // flat modifiers (10 attack boost)
            0, 0, 0, 0, 0,  // percentage modifiers
            NULL)
    };

    p->base.actions[2] = (Action){
        "Boost de défense", SPECIAL_SKILL, 2, 0,  // cooldown_turns, cooldown_remaining
        create_effect("Boost de défense", "Carapace renforcée !",
            2,
            0, 10, 0, 0, 0,  // flat modifiers (10 defense boost)
            0, 0, 0, 0, 0,  // percentage modifiers
            NULL)
    };

    p->base.actions[3] = (Action){
        "Boost de vitesse", SPECIAL_SKILL, 2, 0,  // cooldown_turns, cooldown_remaining
        create_effect("Boost de vitesse", "Rapide comme l'éclair !",
            2,
            0, 0, 10, 0, 0,  // flat modifiers (10 speed boost)
            0, 0, 0, 0, 0,  // percentage modifiers
            NULL)
    };

    p->base.action_count = 4;

    // player unique values
    p->pearls = 10;
    // Inventory initialization - properly handle memory
    Inventory* inv = create_inventory();
    if (inv == NULL) {
        free(p);
        return NULL;
    }
    p->inventory = *inv;
    free(inv);  // Free the temporary inventory pointer after copying
    return p;
}


void free_player(Player *p) {
    if (p == NULL) return;

    // Free all effect display_messages
    for (int i = 0; i < p->base.effects_number; i++) {
        free_effect_content(&p->base.effects[i]);
    }

    free(p);
}

int consume_oxygen(Player *p, int amount) {
    if (p == NULL) return POINTER_NULL;
    p->base.oxygen_level -= amount;
    if (p->base.oxygen_level < 0) {
        p->base.oxygen_level = 0;
    }
    return SUCCESS;
}

int increase_fatigue(Player *p, int amount) {
    if (p == NULL) return POINTER_NULL;
    int new_value = p->base.fatigue_level + amount;
    if (new_value > MAX_FATIGUE) {
        p->base.fatigue_level = MAX_FATIGUE;
        return SUCCESS_SATURATED;
    }
    p->base.fatigue_level = new_value;
    return SUCCESS;
}


int recover_oxygen(Player *p, int oxygen) {
    if (p == NULL) return POINTER_NULL;
    int new_value = p->base.oxygen_level + oxygen;
    int max_oxygen = p->base.max_oxygen_level;
    if (new_value > max_oxygen) {
        p->base.oxygen_level = max_oxygen;
        return SUCCESS_SATURATED;
    }
    p->base.oxygen_level = new_value;
    return SUCCESS;
}

int recover_fatigue(Player *p, int fatigue) {
    if (p == NULL) return POINTER_NULL;
    int new_value = p->base.fatigue_level - fatigue;
    if (new_value < 0) {
        p->base.fatigue_level = 0;
        return SUCCESS_SATURATED;
    }
    p->base.fatigue_level = new_value;
    return SUCCESS;
}

int increase_pearls(Player *p, int amount) {
    if (p == NULL) return POINTER_NULL;
    p->pearls += amount;
    return SUCCESS;
}

int decrease_pearls(Player *p, int amount) {
    if (p == NULL) return POINTER_NULL;
    if (p->pearls < amount) {
        p->pearls = 0;
        return SUCCESS_SATURATED;
    }
    p->pearls -= amount;
    return SUCCESS;
}
