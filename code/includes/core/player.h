//
// Created by Yasmine Slamani on 02/10/2025.
//
#ifndef OCEANDEPTH_PLAYER_H
#define OCEANDEPTH_PLAYER_H
#include "inventory.h"
#include "entity.h"
#include "effect.h"
#include "map.h"

#define MAX_FATIGUE 5

/**
 * @struct Player
 * @brief Represents the player's state, statistics, and resources.
 */
typedef struct Player {
    EntityBase base;
    int pearls;
    Inventory inventory;

    // Current cell visited by the player
    Position current_position;

    // The last cell unlocked
    Position max_position;

    // Heal center usage tracking (session-wide limit)
    int heal_uses_left;

    // Cave usage tracking (one-time only)
    int has_used_cave;  // 0 = not used, 1 = used
} Player;

/**
 * @brief Allocates and initializes a new Player instance.
 * @param name Name of the player (string copied into fixed buffer).
 * @param max_hp Maximum hp of the player
 * @param base_defense Base defense of the player
 * @param max_oxygen Maximum oxygen of the player
 * @param row Starting row position (0 for new game)
 * @param col Starting col position (0 for new game)
 * @return Pointer to the new Player, or NULL if allocation failed.
 * @note Caller must free the returned Player with free_player().
 */
Player *create_player(char *name, int max_hp, int base_defense, int max_oxygen, Position current, Position max);

/**
 * @brief Frees allocated memory for a Player instance.
 * @param p Pointer to the Player instance.
 * @note Safe to call with NULL (no effect).
 */
void free_player(Player *p);

/**
 * @brief Consumes oxygen from the Player.
 * @param p Pointer to the Player.
 * @param amount Oxygen consumed (must be >= 0).
 * @note Oxygen cannot go below 0. If at 0, the player will lose HP each turn.
 */
int consume_oxygen(Player *p, int amount);

/**
 * @brief Increases Player fatigue.
 * @param p Pointer to the Player.
 * @param amount Fatigue increase (must be >= 0).
 * @return 0 if applied normally, -1 if saturated at MAX_FATIGUE.
 */
int increase_fatigue(Player *p, int amount);

/**
 * @brief Recovers oxygen for the Player.
 * @param p Pointer to the Player.
 * @param oxygen Amount to recover (must be >= 0).
 * @return 0 if recovered normally, -1 if already at max.
 */
int recover_oxygen(Player *p, int oxygen);

/**
 * @brief Recovers fatigue (reduces fatigue level).
 * @param p Pointer to the Player.
 * @param fatigue Amount of fatigue to remove (must be >= 0).
 * @return 0 if recovered normally, -1 if already at 0.
 */
int recover_fatigue(Player *p, int fatigue);

/**
 * @brief Increases pearls owned by the Player.
 * @param p Pointer to the Player.
 * @param amount Number of pearls to add (must be >= 0).
 */
int increase_pearls(Player *p, int amount);

/**
 * @brief Decreases pearls owned by the Player.
 * @param p Pointer to the Player.
 * @param amount Number of pearls to remove (must be >= 0).
 * @return 0 if removed normally, -1 if insufficient (saturated at 0).
 */
int decrease_pearls(Player *p, int amount);

void use_consumable(Player *p, Item* item);

int unlock_new_position(Player *player);

int player_move_to(Player* player, Position new_position);

#endif //OCEANDEPTH_PLAYER_H
