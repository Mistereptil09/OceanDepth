//
// Created by Yasmine Slamani on 02/10/2025.
//

#include "core/inventory.h"
#include <stdlib.h>

Inventory *create_inventory(void) {
    Inventory *inv = malloc(sizeof(Inventory));
    if (!inv) return NULL;
    inv->count = 0;

    return inv;
}

void free_inventory(Inventory *inventory) {
    free(inventory);
}