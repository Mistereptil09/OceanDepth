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

/**
 * @brief Adds an item to inventory with automatic overflow handling
 * @param inventory Pointer to inventory
 * @param item Item to add
 * @param replace_callback Function to call when inventory is full (can be NULL)
 *        Returns index to replace, or -1 to cancel
 * @return SUCCESS, POINTER_NULL, or INVENTORY_FULL if callback returns -1
 */
int add_item_to_inventory_with_callback(Inventory* inventory, Item* item, int (*replace_callback)(Inventory* inv, Item* new_item));

/**
 * @brief Simple add without overflow handling (original behavior)
 * @param inventory Pointer to inventory
 * @param item Item to add
 * @return SUCCESS, POINTER_NULL, or INVENTORY_FULL
 */
int add_item_to_inventory(Inventory* inventory, Item* item);

/**
 * @brief Removes item at specific index
 * @param inventory Pointer to inventory
 * @param index Index to remove (0-based)
 * @return SUCCESS or error code
 */
int remove_item_by_index(Inventory* inventory, int index);

/**
 * @brief Removes specific item from inventory
 * @param inventory Pointer to inventory
 * @param item Item to remove
 * @return SUCCESS, POINTER_NULL, or UNPROCESSABLE_REQUEST_ERROR if item not found
 */
int remove_item_to_inventory(Inventory* inventory, Item* item);

/**
 * @brief Checks if inventory has space
 * @param inventory Pointer to inventory
 * @return 1 if space available, 0 otherwise
 */
int inventory_has_space(Inventory* inventory);

/**
 * @brief Gets current item count in inventory
 * @param inventory Pointer to inventory
 * @return Number of items, or -1 if inventory is NULL
 */
int inventory_get_free_slots(Inventory* inventory);

#endif //OCEANDEPTH_INVENTORY_H
