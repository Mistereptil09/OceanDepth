//
// Created by Yasmine Slamani on 02/10/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core/player.h"

/*
 * player.c = Player stats, actions, resources (HP, oxygen, fatigue, pearls, inventory)
 */

Player *create_player(char *name) {
    Player *p = malloc(sizeof(Player));
    if (p == NULL) return NULL;

    p->base = create_entity_base(ENTITY_PLAYER, name, 100, 5);

    // Resources
    p->oxygen_level = 50;
    p->max_oxygen_level = 50;
    p->fatigue_level = 0;
    p->pearls = 10;

    // Inventory mockup function
    p->inventory = *create_inventory();
    return p;
}


void free_player(Player *p) {
    if (p == NULL) return;
    if (p->base.effects != NULL) {
        free(p->base.effects);
        p->base.effects = NULL;
    }
    free(p);
}

void consume_oxygen(Player *p, int amount) {
    p->oxygen_level -= amount;
    if (p->oxygen_level < 0) {
        p->oxygen_level = 0;
    }
}

int increase_fatigue(Player *p, int amount) {
    int new_value = p->fatigue_level + amount;
    if (new_value > MAX_FATIGUE) {
        p->fatigue_level = MAX_FATIGUE;
        return -1;
    }
    p->fatigue_level = new_value;
    return 0;
}


int recover_oxygen(Player *p, int oxygen) {
    int new_value = p->oxygen_level + oxygen;
    if (new_value > p->max_oxygen_level) {
        p->oxygen_level = p->max_oxygen_level;
        return -1;
    }
    p->oxygen_level = new_value;
    return 0;
}

int recover_fatigue(Player *p, int fatigue) {
    int new_value = p->fatigue_level - fatigue;
    if (new_value < 0) {
        p->fatigue_level = 0;
        return -1;
    }
    p->fatigue_level = new_value;
    return 0;
}

void increase_pearls(Player *p, int amount) {
    p->pearls += amount;
}

int decrease_pearls(Player *p, int amount) {
    if (p->pearls < amount) {
        p->pearls = 0;
        return -1;
    }
    p->pearls -= amount;
    return 0;
}
