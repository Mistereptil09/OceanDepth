//
// Created by cantonio on 02/10/2025.
//

#ifndef OCEANDEPTHS_INTERFACE_TABLE_H
#define OCEANDEPTHS_INTERFACE_TABLE_H

#include "core/map.h"
#include "core/inventory.h"
#include "core/creature.h"
#include "core/player.h"
#include "core/entity.h"

typedef struct {
    void (*display_map)(Map* map);
    void (*display_combat)(Player* p, Creature* creatures, int count);
    int (*get_choice)(const char* prompt, int min, int max);
    void (*show_attack)(EntityBase* attacker, EntityBase* target, int damage);
    void (*show_inventory)(Inventory* inv);
    void (*get_input)(const char* prompt, char* result, size_t result_size);
} InterfaceVTable;

// Global interface pointer (declared here, defined in interface_table.c)
extern InterfaceVTable* current_interface;

// Function to select interface
InterfaceVTable* select_interface(void);

#endif //OCEANDEPTHS_INTERFACE_TABLE_H