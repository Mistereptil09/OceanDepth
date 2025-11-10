//
// Created by anton on 11/2/2025.
//

#ifndef OCEANDEPTH_ITEM_POOL_H
#define OCEANDEPTH_ITEM_POOL_H

#include "item.h"

typedef enum {
    RARITY_COMMON,
    RARITY_UNCOMMON,
    RARITY_RARE,
    RARITY_EPIC,
    RARITY_LEGENDARY
} ItemRarity;

typedef struct {
    Item (*item_factory)(void);           // Function pointer to create the item
    Item (*item_factory_qty)(int);        // For consumables with quantity
    ItemRarity rarity;
    int weight;                           // Drop weight (higher = more common in same rarity)
    int is_consumable;                    // 1 if needs quantity parameter
    int min_quantity;                     // Min quantity for consumables
    int max_quantity;                     // Max quantity for consumables
} ItemPoolEntry;

typedef struct {
    ItemPoolEntry* entries;
    int entry_count;
    int* cumulative_weights;              // Precomputed for faster selection
    int total_weight;
} ItemPool;

/**
 * @brief Creates an item pool from an array of entries
 * @param entries Array of ItemPoolEntry
 * @param count Number of entries
 * @return ItemPool structure
 */
ItemPool create_item_pool(ItemPoolEntry* entries, int count);

/**
 * @brief Selects a random item from the pool based on weights
 * @param pool Pointer to ItemPool
 * @return A newly created Item (caller must free)
 */
Item draw_random_item(ItemPool* pool);

/**
 * @brief Selects a random item from a specific rarity tier
 * @param pool Pointer to ItemPool
 * @param rarity Rarity tier to draw from
 * @return A newly created Item (caller must free)
 */
Item draw_item_by_rarity(ItemPool* pool, ItemRarity rarity);

/**
 * @brief Gets the rarity name as a string
 * @param rarity ItemRarity enum value
 * @return String representation
 */
const char* get_rarity_name(ItemRarity rarity);

/**
 * @brief Gets rarity color code for display
 * @param rarity ItemRarity enum value
 * @return Color code string
 */
const char* get_rarity_color(ItemRarity rarity);

/**
 * @brief Frees the item pool resources
 * @param pool Pointer to ItemPool
 */
void free_item_pool(ItemPool* pool);

/**
 * @brief Creates a shop item pool (items available for purchase)
 * @return ItemPool for shop
 */
ItemPool create_shop_pool(void);

/**
 * @brief Creates a loot item pool (items from combat/exploration)
 * @return ItemPool for loot drops
 */
ItemPool create_loot_pool(void);

#endif //OCEANDEPTH_ITEM_POOL_H
