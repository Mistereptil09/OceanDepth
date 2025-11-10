//
// Created by anton on 11/2/2025.
//

#ifndef OCEANDEPTH_SHOP_H
#define OCEANDEPTH_SHOP_H

#include "item.h"
#include "item_pool.h"

#define MAX_SHOP_SLOTS 12
#define SHOP_REFRESH_COST 50

typedef struct {
    Item item;
    int stock;                    // -1 for infinite stock
    int original_price;           // Base price
    int current_price;            // Can be modified by discounts/events
    ItemRarity rarity;
} ShopSlot;

typedef struct {
    char name[50];
    ShopSlot slots[MAX_SHOP_SLOTS];
    int slot_count;
    int refresh_cost;
    int times_refreshed;
    ItemPool* item_pool;          // Pool to draw items from
} Shop;

/**
 * @brief Creates a new shop with random inventory from item pool
 * @param name Shop name
 * @param pool Item pool to draw from
 * @return Initialized Shop structure
 */
Shop create_shop(const char* name, ItemPool* pool);

/**
 * @brief Displays the shop's current inventory with prices and rarity
 * @param shop Pointer to Shop
 * @param player_gold Player's current gold amount
 */
void display_shop(Shop* shop, int player_gold);

/**
 * @brief Attempts to buy an item from the shop
 * @param shop Pointer to Shop
 * @param slot_index Index of the item to buy
 * @param player_gold Pointer to player's gold (will be modified)
 * @param player_inventory Array to add item to (optional, can be NULL)
 * @return 1 on success, 0 on failure (not enough gold, out of stock, etc.)
 */
int shop_buy_item(Shop* shop, int slot_index, int* player_gold, Item* player_inventory);

/**
 * @brief Sells an item to the shop (player sells to merchant)
 * @param shop Pointer to Shop
 * @param item_to_sell Item being sold
 * @param player_gold Pointer to player's gold (will be modified)
 * @return Gold received for the item
 */
int shop_sell_item(Shop* shop, Item* item_to_sell, int* player_gold);

/**
 * @brief Refreshes shop inventory with new random items
 * @param shop Pointer to Shop
 * @param player_gold Pointer to player's gold (will be deducted)
 * @return 1 on success, 0 if player can't afford refresh
 */
int shop_refresh_inventory(Shop* shop, int* player_gold);

/**
 * @brief Applies a discount to all items in the shop
 * @param shop Pointer to Shop
 * @param discount_percent Discount percentage (0.0 - 1.0)
 */
void shop_apply_discount(Shop* shop, float discount_percent);

/**
 * @brief Restocks all items to their original quantities
 * @param shop Pointer to Shop
 */
void shop_restock(Shop* shop);

/**
 * @brief Gets the sell price for an item (usually 50% of buy price)
 * @param item Pointer to Item
 * @return Sell price in gold
 */
int get_item_sell_price(Item* item);

/**
 * @brief Frees shop resources
 * @param shop Pointer to Shop
 */
void free_shop(Shop* shop);

#endif //OCEANDEPTH_SHOP_H
