//
// Created by Yasmine Slamani on 02/10/2025.
//

#ifndef OCEANDEPTH_INVENTORY_H
#define OCEANDEPTH_INVENTORY_H
#include "item.h"

#define INVENTORY_SIZE 8

typedef struct {
    Item items[INVENTORY_SIZE];
    int count;
} Inventory;


Inventory *create_inventory(void);
void free_inventory(Inventory *inventory);

// add
int add_item_to_inventory(Inventory *inventory, Item item);
// remove
int remove_item_to_inventory(Inventory *inventory, Item* item);

int insert_into_inventory(Inventory* inventory, Item item);
// ...
#endif //OCEANDEPTH_INVENTORY_H
