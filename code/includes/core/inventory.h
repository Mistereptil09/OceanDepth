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

#endif //OCEANDEPTH_INVENTORY_H
