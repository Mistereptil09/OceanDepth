//
// Created by cantonio on 02/10/2025.
//

#ifndef OCEANDEPTHS_INTERFACE_CLI_H
#define OCEANDEPTHS_INTERFACE_CLI_H


#include "interface_table.h"

// Declare the CLI vtable instance
extern InterfaceVTable cli_interface;

// You can also declare individual functions if you want to call them directly
void cli_display_map(Map* map);
void cli_display_combat_state(void);
void cli_display_combat_intro(Creature** creatures, int count);
void cli_display_round_header(int round_number);
void cli_display_victory(void);
void cli_display_defeat(void);
void cli_display_battle_start(void);
void cli_wait_for_enter(const char* prompt);
int cli_get_choice(const char* prompt, int min, int max);
void cli_show_attack(EntityBase* attacker, EntityBase* target, int damage);
void cli_show_inventory(Inventory* inv);
void cli_get_input(const char* prompt, char* result, size_t result_size);
void cli_show_action(const char* entity_name, const char* action_name);
void cli_show_defeat_by(const char* enemy_name);
#endif //OCEANDEPTHS_INTERFACE_CLI_H