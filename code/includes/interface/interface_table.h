//
// Created by cantonio on 02/10/2025.
//

#ifndef OCEANDEPTHS_INTERFACE_TABLE_H
#define OCEANDEPTHS_INTERFACE_TABLE_H

#include <stddef.h>
#include "core/map.h"
#include "core/inventory.h"
#include "core/creature.h"
#include "core/player.h"
#include "core/entity.h"

/**
 * @brief Interface VTable - defines functions that each interface must implement
 *
 * Interfaces should query data using interface_api.h functions rather than
 * receiving all data as parameters. This allows cleaner separation.
 */
typedef struct {
    void (*display_map)(Map* map, Player* player);

    /**
     * @brief Display current combat state
     * Interface should query combat state using get_combat_state() from interface_api.h
     */
    void (*display_combat_state)(void);

    /**
     * @brief Display combat introduction (enemies at start)
     * @param creatures Array of creatures
     * @param count Number of creatures
     */
    void (*display_combat_intro)(Creature** creatures, int count);

    /**
     * @brief Display round header
     * @param round_number Current round number
     */
    void (*display_round_header)(int round_number);

    /**
     * @brief Display victory message
     */
    void (*display_victory)(void);

    /**
     * @brief Display defeat message
     */
    void (*display_defeat)(void);

    /**
     * @brief Display battle start message
     */
    void (*display_battle_start)(void);

    /**
     * @brief Wait for user to press Enter
     */
    void (*wait_for_enter)(const char* prompt);

    int (*get_choice)(const char* prompt, int min, int max);
    void (*show_attack)(EntityBase* attacker, EntityBase* target, int damage);
    void (*show_inventory)(Inventory* inv);
    void (*get_input)(const char* prompt, char* result, size_t result_size);

    /**
     * @brief Show a generic information message
     * @param message The message to display (may include newlines)
     */
    void (*show_information)(const char* message);

    /**
     * @brief Show an action message (creature uses skill, etc.)
     * @param entity_name Name of entity performing action
     * @param action_name Name of action
     */
    void (*show_action)(const char* entity_name, const char* action_name);

    /**
     * @brief Show defeat message from specific enemy
     * @param enemy_name Name of enemy that defeated player
     */
    void (*show_defeat_by)(const char* enemy_name);

    // ========== COMBAT FEEDBACK MESSAGES ==========

    /**
     * @brief Show oxygen consumption message
     * @param amount Amount consumed
     * @param current Current oxygen level
     * @param max Maximum oxygen level
     */
    void (*show_oxygen_consumed)(int amount, int current, int max);

    /**
     * @brief Show oxygen critical warning
     * @param current Current oxygen level
     */
    void (*show_oxygen_critical)(int current);

    /**
     * @brief Show oxygen death message
     * @param damage Damage taken from suffocation
     * @param hp Current HP
     * @param max_hp Max HP
     */
    void (*show_oxygen_death)(int damage, int hp, int max_hp);

    /**
     * @brief Show fatigue level and available actions
     * @param fatigue Current fatigue level
     * @param max_actions Number of actions available this turn
     */
    void (*show_fatigue_status)(int fatigue, int max_actions);

    /**
     * @brief Show fatigue increase
     * @param new_fatigue New fatigue level
     */
    void (*show_fatigue_increased)(int new_fatigue);

    /**
     * @brief Show fatigue recovery
     * @param new_fatigue New fatigue level after recovery
     */
    void (*show_fatigue_recovered)(int new_fatigue);

    /**
     * @brief Show passive oxygen consumption at end of round
     * @param amount Amount consumed
     * @param current Current oxygen level
     * @param max Maximum oxygen level
     */
    void (*show_passive_oxygen)(int amount, int current, int max);

    /**
     * @brief Show damage dealt message
     * @param attacker_name Name of attacker
     * @param target_name Name of target
     * @param damage Damage dealt
     * @param target_hp Current HP of target
     * @param target_max_hp Max HP of target
     */
    void (*show_damage_dealt)(const char* attacker_name, const char* target_name,
                              int damage, int target_hp, int target_max_hp);

    /**
     * @brief Show attack blocked message
     * @param defender_name Name of defender
     */
    void (*show_attack_blocked)(const char* defender_name);

    /**
     * @brief Show creature defeated message
     * @param creature_name Name of defeated creature
     */
    void (*show_creature_defeated)(const char* creature_name);

    /**
     * @brief Show number of actions taken this turn
     * @param actions_taken Number of actions taken
     */
    void (*show_actions_taken)(int actions_taken);

    /**
     * @brief Show player death from afflictions
     */
    void (*show_death_from_afflictions)(void);

    /**
     * @brief Show player death from suffocation
     */
    void (*show_death_from_suffocation)(void);

    /**
     * @brief Show enemy turn header
     */
    void (*show_enemy_turn)(void);

    /**
     * @brief Show your turn header
     */
    void (*show_your_turn)(void);

    /**
     * @brief Show ending turn message
     */
    void (*show_ending_turn)(void);

    /**
     * @brief Show action effect message
     * @param message The effect message to display
     */
    void (*show_action_effect)(const char* message);

    /**
     * @brief Show error message for applying effect
     */
    void (*show_effect_error)(void);

    /**
     * @brief Show action cooldown error
     * @param action_name Name of action on cooldown
     */
    void (*show_action_on_cooldown)(const char* action_name);

    /**
     * @brief Show item on cooldown error
     * @param item_name Name of item on cooldown
     */
    void (*show_item_on_cooldown)(const char* item_name);

    /**
     * @brief Show no actions available error
     * @param creature_name Name of creature
     */
    void (*show_no_actions_available)(const char* creature_name);

    /**
     * @brief Show creature died from effects
     * @param creature_name Name of creature
     */
    void (*show_creature_died_from_effects)(const char* creature_name);

    /**
     * @brief Ask player to choose an item as reward
     * @param item_names Array of item names
     * @param item_count Number of items
     * @return Index of chosen item
     */
     Item* (*ask_item_choice_reward)(int max_item_pool_size, Item* drawn_item);

    /**
     * @brief Show inventory full message
     */
     void (*show_inventory_full)(void);

    // ========== REWARD SYSTEM INTERFACE ==========

    /**
     * @brief Show pearl reward message
     * @param amount Amount of pearls gained
     * @param total Total pearls after reward
     */
    void (*show_pearl_reward)(int amount, int total);

    /**
     * @brief Show reward obtained message
     * @param item_name Name of item obtained
     */
    void (*show_reward_obtained)(const char* item_name);

    /**
     * @brief Show inventory replacement prompt
     * @param new_item_name Name of new item
     */
    void (*show_inventory_replacement_prompt)(const char* new_item_name);

    // ========== SHOP INTERFACE FUNCTIONS ==========

    /**
     * @brief Display shop with inventory
     */
    void (*display_shop)(const char* shop_name, int player_gold, int refresh_cost,
                         const char** items, const int* prices, const int* stocks,
                         const int* rarities, const int* can_afford, int item_count);

    /**
     * @brief Show purchase success message
     */
    void (*show_purchase_success)(const char* item_name, int price, int quantity);

    /**
     * @brief Show purchase failure message
     */
    void (*show_purchase_failed)(const char* reason);

    /**
     * @brief Show sell success message
     */
    void (*show_sell_success)(const char* item_name, int gold_received);

    /**
     * @brief Show sell failure message
     */
    void (*show_sell_failed)(const char* reason);

    /**
     * @brief Show shop refresh success
     */
    void (*show_shop_refreshed)(void);

    /**
     * @brief Show shop refresh failure
     */
    void (*show_refresh_failed)(int cost, int player_gold);

    /**
     * @brief Show discount applied
     */
    void (*show_discount_applied)(int discount_percent);

    /**
     * @brief Show shop restocked
     */
    void (*show_shop_restocked)(void);

} InterfaceVTable;

// Global interface pointer (declared here, defined in interface_table.c)
extern InterfaceVTable* current_interface;

// Function to select interface
InterfaceVTable* select_interface(void);

#endif //OCEANDEPTHS_INTERFACE_TABLE_H

