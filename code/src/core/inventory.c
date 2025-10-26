//
// Created by Yasmine Slamani on 02/10/2025.
//

#include "core/inventory.h"
#include <stdlib.h>

#include "core/inventory_data.h"

Inventory *create_inventory(void) {
    Inventory *inventory = calloc(1, sizeof(Inventory));
    if (!inventory) return NULL;
    inventory->items[0] = create_epee();
    inventory->items[1] = create_harpon();
    inventory->items[2] = create_combinaison();
    inventory->items[3] = create_fatigue_stimulant(1);
    inventory->items[4] = create_oxygen_capsule(2);
    inventory->items[5] = create_health_kit(1);

    inventory->count = 6;
    return inventory;
}

void free_inventory(Inventory *inventory) {
    if (inventory == NULL) return;
    for (int i = 0; i < inventory->count; i++) {
        free_item(&inventory->items[0]);
    }
    free(inventory);
}