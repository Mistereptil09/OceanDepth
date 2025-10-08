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

void init_player(Player *p) {
    // Stats
    p->base.max_health_points = 100;
    p->base.current_health_points = 100;

    p->base.base_attack = 10;
    p->base.current_attack = 10;

    p->base.base_defense = 5;
    p->base.current_defense = 5;

    p->base.speed = 0;

    // Resources
    p->oxygen_level = 50;
    p->max_oxygen_level = 50;
    p->fatigue_level = 0;
    p->pearls = 10;

    // Inventory
    p->inventory = *create_inventory();
}

Player *create_player(const char *name) {
    Player *p = malloc(sizeof(Player));
    if (p == NULL) return NULL;

    if (name != NULL)
        strncpy(p->base.name, name, sizeof(p->base.name) - 1);
    else
        strncpy(p->base.name, "Player", sizeof(p->base.name) - 1);

    p->base.name[sizeof(p->base.name) - 1] = '\0';
    init_player(p);
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

void player_take_damage(Player *p, const int damage) {
    p->base.current_health_points -= damage;
    if (p->base.current_health_points < 0) {
        p->base.current_health_points = 0;
    }
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

int recover_hp(Player *p, int hp) {
    int new_value = p->base.current_health_points + hp;
    if (new_value > p->base.max_health_points) {
        p->base.current_health_points = p->base.max_health_points;
        return -1; // saturated
    }
    p->base.current_health_points = new_value;
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
