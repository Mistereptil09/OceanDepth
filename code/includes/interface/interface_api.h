
#ifndef OCEANDEPTHS_INTERFACE_API_H
#define OCEANDEPTHS_INTERFACE_API_H

#include "core/player.h"
#include "core/creature.h"

/**
 * @brief Combat state information that can be queried by interfaces
 */
typedef struct {
    Player* player;
    Creature** creatures;
    int creature_count;
    int round_number;
    int alive_count;
} CombatState;

// ========== COMBAT STATE QUERIES ==========

/**
 * @brief Set the current combat state
 * @param player Pointer to player
 * @param creatures Array of creature pointers
 * @param creature_count Number of creatures in array
 * @param round Current round number
 */
void set_combat_state(Player* player, Creature** creatures, int creature_count, int round);

/**
 * @brief Get the current combat state
 * @return Pointer to current combat state (read-only)
 */
const CombatState* get_combat_state(void);

/**
 * @brief Get count of alive creatures in current combat
 * @return Number of alive creatures
 */
int get_alive_creature_count(void);

/**
 * @brief Get a specific alive creature by index (1-based for UI)
 * @param alive_index The index among alive creatures (1 = first alive creature)
 * @return Pointer to creature or NULL if index invalid
 */
Creature* get_alive_creature_at(int alive_index);

// ========== PLAYER QUERIES ==========

/**
 * @brief Get player name
 * @param player Player pointer
 * @return Player name string
 */
const char* get_player_name(Player* player);

/**
 * @brief Get player current HP
 */
int get_player_hp(Player* player);

/**
 * @brief Get player max HP
 */
int get_player_max_hp(Player* player);

/**
 * @brief Get player current oxygen
 */
int get_player_oxygen(Player* player);

/**
 * @brief Get player max oxygen
 */
int get_player_max_oxygen(Player* player);

/**
 * @brief Get player attack stat (including modifiers)
 */
int get_player_attack(Player* player);

/**
 * @brief Get player defense stat (including modifiers)
 */
int get_player_defense(Player* player);

/**
 * @brief Get player pearls
 */
int get_player_pearls(Player* player);

/**
 * @brief Get player fatigue level
 */
int get_player_fatigue(Player* player);

// ========== CREATURE QUERIES ==========

/**
 * @brief Get creature name
 */
const char* get_creature_name(Creature* creature);

/**
 * @brief Get creature current HP
 */
int get_creature_hp(Creature* creature);

/**
 * @brief Get creature max HP
 */
int get_creature_max_hp(Creature* creature);

/**
 * @brief Get creature attack stat (including modifiers)
 */
int get_creature_attack(Creature* creature);

/**
 * @brief Get creature defense stat (including modifiers)
 */
int get_creature_defense(Creature* creature);

/**
 * @brief Get creature speed stat (including modifiers)
 */
int get_creature_speed(Creature* creature);

/**
 * @brief Get creature ID
 */
int get_creature_id(Creature* creature);

/**
 * @brief Check if creature is alive
 */
int is_creature_alive(Creature* creature);

/**
 * @brief Get creature action name
 * @param creature Creature pointer
 * @param action_index Action index (0-1)
 * @return Action name or NULL if invalid index
 */
const char* get_creature_action_name(Creature* creature, int action_index);

/**
 * @brief Get number of active effects on creature
 * @param creature Creature pointer
 * @return Number of active effects
 */
int get_creature_effect_count(Creature* creature);

/**
 * @brief Get effect name by index
 * @param creature Creature pointer
 * @param effect_index Effect index
 * @return Effect name or NULL if invalid index
 */
const char* get_creature_effect_name(Creature* creature, int effect_index);

/**
 * @brief Get effect turns remaining by index
 * @param creature Creature pointer
 * @param effect_index Effect index
 * @return Turns remaining or -1 if invalid index
 */
int get_creature_effect_turns(Creature* creature, int effect_index);

#endif //OCEANDEPTHS_INTERFACE_API_H

