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
    void (*display_map)(Map* map);

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
} InterfaceVTable;

// Global interface pointer (declared here, defined in interface_table.c)
extern InterfaceVTable* current_interface;

// Function to select interface
InterfaceVTable* select_interface(void);

#endif //OCEANDEPTHS_INTERFACE_TABLE_H