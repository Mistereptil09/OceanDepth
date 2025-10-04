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

/**
 * @brief Initializes default values of a Player instance (used internally by create_player).
 * @param p Pointer to a Player (must not be NULL).
 */
void init_player(Player *p) {
    // Stats
    p->stats.max_health_points = 100;
    p->stats.current_health_points = 100;

    p->stats.base_attack = 10;
    p->stats.current_attack = 10;

    p->stats.base_defense = 5;
    p->stats.current_defense = 5;

    p->stats.speed = 5;

    // Resources
    p->oxygen_level = 50;
    p->max_oxygen_level = 50;
    p->fatigue_level = 0;
    p->pearls = 10;

    // Inventory
    p->inventory = *create_inventory();
}

/**
 * @brief Allocates and initializes a new Player instance.
 * @param name Name of the player (string is duplicated).
 * @return Pointer to the new Player, or NULL if allocation failed.
 * @note Caller must free the returned Player with free_player().
 */
Player *create_player(char *name) {
    Player *p = malloc(sizeof(Player));
    if (p == NULL) return NULL;

    if (name != NULL) {
        p->name = strdup(name);
        if (p->name == NULL) {
            free(p);
            return NULL;
        }
    } else {
        p->name = strdup("Player");
    }

    init_player(p);
    return p;
}

/**
 * @brief Frees allocated memory for a Player instance.
 * @param p Pointer to the Player instance.
 * @note Safe to call with NULL (no effect).
 */
void free_player(Player *p) {
    if (p != NULL) {
        // Potentially other things to free first, depending on the structure of Items
        free(p->name);
        free(p);
    }
}

/**
 * @brief Applies damage to the Player's HP.
 * @param p Pointer to the Player.
 * @param damage Amount of damage to apply (must be >= 0).
 * @note If damage reduces HP below 0, HP is set at 0.
 */
void take_damage(Player *p, int damage) {
    p->stats.current_health_points -= damage;
    if (p->stats.current_health_points < 0) {
        p->stats.current_health_points = 0;
    }
}

/**
 * @brief Consumes oxygen from the Player.
 * @param p Pointer to the Player.
 * @param amount Oxygen consumed (must be >= 0).
 * @note Oxygen cannot go below 0. If already at 0, player will lose HP in combat loop.
 */
void consume_oxygen(Player *p, int amount) {
    p->oxygen_level -= amount;
    if (p->oxygen_level < 0) {
        p->oxygen_level = 0;
    }
}

/**
 * @brief Increases Player fatigue.
 * @param p Pointer to the Player.
 * @param amount Fatigue increase (must be >= 0).
 * @return 0 if increase applied normally, -1 if saturated at MAX_FATIGUE.
 * @note Fatigue is capped at MAX_FATIGUE.
 */
int increase_fatigue(Player *p, int amount) {
    int new_value = p->fatigue_level + amount;
    if (new_value > MAX_FATIGUE) {
        p->fatigue_level = MAX_FATIGUE;
        return -1;
    }
    p->fatigue_level = new_value;
    return 0;
}

/**
 * @brief Recovers Player HP.
 * @param p Pointer to the Player.
 * @param hp Amount of HP recovered (must be >= 0).
 * @return 0 if recovered normally, -1 if already at max and saturated.
 * @note Health points cannot exceed max_health_points.
 */
int recover_hp(Player *p, int hp) {
    int new_value = p->stats.current_health_points + hp;
    if (new_value > p->stats.max_health_points) {
        p->stats.current_health_points = p->stats.max_health_points;
        return -1; // saturated
    }
    p->stats.current_health_points = new_value;
    return 0;
}

/**
 * @brief Recovers oxygen for the Player.
 * @param p Pointer to the Player.
 * @param oxygen Amount to recover (must be >= 0).
 * @return 0 if recovered normally, -1 if saturated at max.
 */
int recover_oxygen(Player *p, int oxygen) {
    int new_value = p->oxygen_level + oxygen;
    if (new_value > p->max_oxygen_level) {
        p->oxygen_level = p->max_oxygen_level;
        return -1;
    }
    p->oxygen_level = new_value;
    return 0;
}

/**
 * @brief Recovers fatigue (reduces fatigue level).
 * @param p Pointer to the Player.
 * @param fatigue Amount of fatigue to remove (must be >= 0).
 * @return 0 if recovered normally, -1 if already at 0.
 * @note Fatigue cannot go below 0.
 */
int recover_fatigue(Player *p, int fatigue) {
    int new_value = p->fatigue_level - fatigue;
    if (new_value < 0) {
        p->fatigue_level = 0;
        return -1;
    }
    p->fatigue_level = new_value;
    return 0;
}

/**
 * @brief Increases pearls owned by the Player.
 * @param p Pointer to the Player.
 * @param amount Number of pearls to add (must be >= 0).
 */
void increase_pearls(Player *p, int amount) {
    p->pearls += amount;
}

/**
 * @brief Decreases pearls owned by the Player.
 * @param p Pointer to the Player.
 * @param amount Number of pearls to remove (must be >= 0).
 * @return 0 if removed normally, -1 if pearls were insufficient (saturated at 0).
 * @note Pearls cannot go below 0.
 */
int decrease_pearls(Player *p, int amount) {
    if (p->pearls < amount) {
        p->pearls = 0;
        return -1;
    }
    p->pearls -= amount;
    return 0;
}
