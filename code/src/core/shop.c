//
// Created by Yasmine Slamani on 02/11/2025.
//

#include "core/shop.h"
#include "core/item_pool.h"
#include "interface/interface_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Helper function to determine stock based on rarity
static int get_stock_by_rarity(ItemRarity rarity) {
    switch(rarity) {
        case RARITY_COMMON: return 5 + (rand() % 6);
        case RARITY_UNCOMMON: return 3 + (rand() % 4);
        case RARITY_RARE: return 2 + (rand() % 3);
        case RARITY_EPIC: return 1 + (rand() % 2);
        case RARITY_LEGENDARY: return 1;
        default: return 3;
    }
}

// Helper function to populate shop inventory
static void populate_shop_inventory(Shop* shop) {
    shop->slot_count = 0;

    int num_slots = 6 + (shop->times_refreshed / 3);
    if (num_slots > MAX_SHOP_SLOTS) num_slots = MAX_SHOP_SLOTS;

    int common_slots = num_slots / 2;
    int uncommon_slots = num_slots / 4;
    int rare_slots = num_slots / 6;
    int epic_slots = 1;
    int legendary_chance = (rand() % 100) < 5;

    // Fill common items
    for (int i = 0; i < common_slots && shop->slot_count < num_slots; i++) {
        Item item = draw_item_by_rarity(shop->item_pool, RARITY_COMMON);
        shop->slots[shop->slot_count].item = item;
        shop->slots[shop->slot_count].stock = get_stock_by_rarity(RARITY_COMMON);
        shop->slots[shop->slot_count].original_price = item.price;
        shop->slots[shop->slot_count].current_price = item.price;
        shop->slots[shop->slot_count].rarity = RARITY_COMMON;
        shop->slot_count++;
    }

    // Fill uncommon items
    for (int i = 0; i < uncommon_slots && shop->slot_count < num_slots; i++) {
        Item item = draw_item_by_rarity(shop->item_pool, RARITY_UNCOMMON);
        shop->slots[shop->slot_count].item = item;
        shop->slots[shop->slot_count].stock = get_stock_by_rarity(RARITY_UNCOMMON);
        shop->slots[shop->slot_count].original_price = item.price;
        shop->slots[shop->slot_count].current_price = item.price;
        shop->slots[shop->slot_count].rarity = RARITY_UNCOMMON;
        shop->slot_count++;
    }

    // Fill rare items
    for (int i = 0; i < rare_slots && shop->slot_count < num_slots; i++) {
        Item item = draw_item_by_rarity(shop->item_pool, RARITY_RARE);
        shop->slots[shop->slot_count].item = item;
        shop->slots[shop->slot_count].stock = get_stock_by_rarity(RARITY_RARE);
        shop->slots[shop->slot_count].original_price = item.price;
        shop->slots[shop->slot_count].current_price = item.price;
        shop->slots[shop->slot_count].rarity = RARITY_RARE;
        shop->slot_count++;
    }

    // Fill epic items
    for (int i = 0; i < epic_slots && shop->slot_count < num_slots; i++) {
        Item item = draw_item_by_rarity(shop->item_pool, RARITY_EPIC);
        shop->slots[shop->slot_count].item = item;
        shop->slots[shop->slot_count].stock = get_stock_by_rarity(RARITY_EPIC);
        shop->slots[shop->slot_count].original_price = item.price;
        shop->slots[shop->slot_count].current_price = item.price;
        shop->slots[shop->slot_count].rarity = RARITY_EPIC;
        shop->slot_count++;
    }

    // Maybe add legendary
    if (legendary_chance && shop->slot_count < num_slots) {
        Item item = draw_item_by_rarity(shop->item_pool, RARITY_LEGENDARY);
        shop->slots[shop->slot_count].item = item;
        shop->slots[shop->slot_count].stock = 1;
        shop->slots[shop->slot_count].original_price = item.price;
        shop->slots[shop->slot_count].current_price = item.price;
        shop->slots[shop->slot_count].rarity = RARITY_LEGENDARY;
        shop->slot_count++;
    }
}

Shop create_shop(const char* name, ItemPool* pool) {
    Shop shop = {0};
    strncpy(shop.name, name, sizeof(shop.name) - 1);
    shop.name[sizeof(shop.name) - 1] = '\0';

    shop.item_pool = pool;
    shop.refresh_cost = SHOP_REFRESH_COST;
    shop.times_refreshed = 0;

    populate_shop_inventory(&shop);

    return shop;
}

void display_shop(Shop* shop, int player_gold) {
    if (!current_interface) return;

    // Prepare arrays for interface
    const char* items[MAX_SHOP_SLOTS];
    int prices[MAX_SHOP_SLOTS];
    int stocks[MAX_SHOP_SLOTS];
    int rarities[MAX_SHOP_SLOTS];
    int can_afford[MAX_SHOP_SLOTS];

    for (int i = 0; i < shop->slot_count; i++) {
        items[i] = shop->slots[i].item.name;
        prices[i] = shop->slots[i].current_price;
        stocks[i] = shop->slots[i].stock;
        rarities[i] = shop->slots[i].rarity;
        can_afford[i] = (player_gold >= shop->slots[i].current_price);
    }

    current_interface->display_shop(
            shop->name,
            player_gold,
            shop->refresh_cost,
            items,
            prices,
            stocks,
            rarities,
            can_afford,
            shop->slot_count
    );
}

int shop_buy_item(Shop* shop, int slot_index, int* player_gold, Item* player_inventory) {
    if (!current_interface) return 0;

    if (slot_index < 0 || slot_index >= shop->slot_count) {
        current_interface->show_purchase_failed("Article invalide!");
        return 0;
    }

    ShopSlot* slot = &shop->slots[slot_index];

    // Check stock
    if (slot->stock <= 0) {
        current_interface->show_purchase_failed("Rupture de stock!");
        return 0;
    }

    // Check if player can afford
    if (*player_gold < slot->current_price) {
        current_interface->show_purchase_failed("Pas assez de perles!");
        return 0;
    }

    // Process purchase
    *player_gold -= slot->current_price;
    slot->stock--;

    // Show success
    int quantity = (slot->item.type == ITEM_CONSUMABLE) ? slot->item.quantity : 0;
    current_interface->show_purchase_success(
            slot->item.name,
            slot->current_price,
            quantity
    );

    return 1;
}

int shop_sell_item(Shop* shop, Item* item_to_sell, int* player_gold) {
    if (!current_interface) return 0;

    if (!item_to_sell) {
        current_interface->show_sell_failed("Article invalide!");
        return 0;
    }

    int sell_price = get_item_sell_price(item_to_sell);

    if (sell_price <= 0) {
        current_interface->show_sell_failed("Cet article ne peut pas etre vendu!");
        return 0;
    }

    *player_gold += sell_price;
    current_interface->show_sell_success(item_to_sell->name, sell_price);

    return sell_price;
}

int shop_refresh_inventory(Shop* shop, int* player_gold) {
    if (!current_interface) return 0;

    if (*player_gold < shop->refresh_cost) {
        current_interface->show_refresh_failed(shop->refresh_cost, *player_gold);
        return 0;
    }

    *player_gold -= shop->refresh_cost;
    shop->times_refreshed++;
    shop->refresh_cost = SHOP_REFRESH_COST + (shop->times_refreshed * 10);

    // Free old items
    for (int i = 0; i < shop->slot_count; i++) {
        free_item(&shop->slots[i].item);
    }

    // Generate new inventory
    populate_shop_inventory(shop);

    current_interface->show_shop_refreshed();

    return 1;
}

void shop_apply_discount(Shop* shop, float discount_percent) {
    if (!current_interface) return;

    if (discount_percent < 0.0f) discount_percent = 0.0f;
    if (discount_percent > 1.0f) discount_percent = 1.0f;

    for (int i = 0; i < shop->slot_count; i++) {
        shop->slots[i].current_price =
                (int)(shop->slots[i].original_price * (1.0f - discount_percent));

        if (shop->slots[i].current_price < 1) {
            shop->slots[i].current_price = 1;
        }
    }

    int discount_display = (int)(discount_percent * 100);
    current_interface->show_discount_applied(discount_display);
}

void shop_restock(Shop* shop) {
    if (!current_interface) return;

    for (int i = 0; i < shop->slot_count; i++) {
        shop->slots[i].stock = get_stock_by_rarity(shop->slots[i].rarity);
    }

    current_interface->show_shop_restocked();
}

int get_item_sell_price(Item* item) {
    if (!item || item->price <= 0) return 0;
    int sell_price = item->price / 2;
    return sell_price > 0 ? sell_price : 1;
}

void free_shop(Shop* shop) {
    if (!shop) return;

    for (int i = 0; i < shop->slot_count; i++) {
        free_item(&shop->slots[i].item);
    }
}
