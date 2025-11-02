//
// Created by Yasmine Slamani on 02/10/2025.
//

#include "core/inventory.h"
#include "core/error_codes.h"
#include <stdlib.h>
#include <string.h>

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
        free_item(&inventory->items[i]);
    }
}

int add_item_to_inventory(Inventory* inventory, Item* item) {
    if (!inventory || !item) return POINTER_NULL;
    if (inventory->count >= INVENTORY_SIZE) {
        return INVENTORY_FULL;
    }

    inventory->items[inventory->count] = *item;
    inventory->count++;
    return SUCCESS;
}

int add_item_to_inventory_with_callback(Inventory* inventory, Item* item,
                                        int (*replace_callback)(Inventory* inv, Item* new_item)) {
    if (!inventory || !item) return POINTER_NULL;

    // Try simple add first
    int result = add_item_to_inventory(inventory, item);

    // If not full, we're done
    if (result == SUCCESS) {
        return SUCCESS;
    }

    // Inventory is full - invoke callback if provided
    if (result == INVENTORY_FULL && replace_callback) {
        int replace_index = replace_callback(inventory, item);

        // User cancelled or invalid index
        if (replace_index < 0 || replace_index >= inventory->count) {
            return INVENTORY_FULL;
        }

        // Replace item at chosen index
        free_item(&inventory->items[replace_index]);
        inventory->items[replace_index] = *item;
        return SUCCESS;
    }

    // No callback provided or other error
    return result;
}

int remove_item_by_index(Inventory* inventory, int index) {
    if (!inventory) return POINTER_NULL;
    if (index < 0 || index >= inventory->count) return 1; // Invalid index (no specific code defined)

    free_item(&inventory->items[index]);

    // Shift items down
    for (int i = index; i < inventory->count - 1; i++) {
        inventory->items[i] = inventory->items[i + 1];
    }

    inventory->count--;
    return SUCCESS;
}

int remove_item_to_inventory(Inventory* inventory, Item* item) {
    if (!inventory || !item) return POINTER_NULL;

    int index = -1;
    for (int i = 0; i < inventory->count; i++) {
        if (inventory->items[i], item) {
            index = i;
            break;
        }
    }

    if (index == -1) return UNPROCESSABLE_REQUEST_ERROR;

    free_item(item);

    for (int i = index; i < inventory->count - 1; i++) {
        inventory->items[i] = inventory->items[i + 1];
    }

    inventory->count--;
    return SUCCESS;
}

int inventory_has_space(Inventory* inventory) {
    if (!inventory) return 0;
    return inventory->count < INVENTORY_SIZE;
}

int inventory_get_free_slots(Inventory* inventory) {
    if (!inventory) return 0;
    return INVENTORY_SIZE - inventory->count;
}