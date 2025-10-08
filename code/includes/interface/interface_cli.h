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
void cli_display_combat(Player* player, Creature* creatures, int count);
int cli_get_choice(const char* prompt, int min, int max);
void cli_show_attack(EntityBase* attacker, EntityBase* target, int damage);
void cli_show_inventory(Inventory* inv);

#endif //OCEANDEPTHS_INTERFACE_CLI_H