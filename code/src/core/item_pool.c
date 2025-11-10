//
// Created by anton on 11/2/2025.
//

#include "core/item_pool.h"
#include "core/inventory_data.h"  // Player starting items
#include "core/item_data.h"       // Shop and loot items
#include <stdlib.h>
#include <time.h>
#include <string.h>

ItemPool create_item_pool(ItemPoolEntry* entries, int count) {
    ItemPool pool;
    pool.entries = entries;
    pool.entry_count = count;

    // Calculate cumulative weights for O(log n) selection
    pool.cumulative_weights = malloc(sizeof(int) * count);
    pool.total_weight = 0;

    for (int i = 0; i < count; i++) {
        pool.total_weight += entries[i].weight;
        pool.cumulative_weights[i] = pool.total_weight;
    }

    return pool;
}

Item draw_random_item(ItemPool* pool) {
    if (!pool || pool->entry_count == 0) {
        return create_item("Empty", ITEM_CONSUMABLE, NULL, 0, 0, 0, 0, 0, 0);
    }

    // Generate random number between 1 and total_weight
    int roll = (rand() % pool->total_weight) + 1;

    // Binary search for the selected item
    int left = 0;
    int right = pool->entry_count - 1;
    int selected_index = 0;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (roll <= pool->cumulative_weights[mid]) {
            selected_index = mid;
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }

    ItemPoolEntry* entry = &pool->entries[selected_index];

    // Create the item
    if (entry->is_consumable && entry->item_factory_qty) {
        int quantity = entry->min_quantity;
        if (entry->max_quantity > entry->min_quantity) {
            quantity += rand() % (entry->max_quantity - entry->min_quantity + 1);
        }
        return entry->item_factory_qty(quantity);
    } else if (entry->item_factory) {
        return entry->item_factory();
    }

    return create_item("Empty", ITEM_CONSUMABLE, NULL, 0, 0, 0, 0, 0, 0);
}

Item draw_item_by_rarity(ItemPool* pool, ItemRarity rarity) {
    if (!pool || pool->entry_count == 0) {
        return create_item("Empty", ITEM_CONSUMABLE, NULL, 0, 0, 0, 0, 0, 0);
    }

    // Count items of this rarity and calculate total weight
    int rarity_count = 0;
    int rarity_total_weight = 0;

    for (int i = 0; i < pool->entry_count; i++) {
        if (pool->entries[i].rarity == rarity) {
            rarity_count++;
            rarity_total_weight += pool->entries[i].weight;
        }
    }

    if (rarity_count == 0) {
        return create_item("Empty", ITEM_CONSUMABLE, NULL, 0, 0, 0, 0, 0, 0);
    }

    // Random selection within rarity tier
    int roll = (rand() % rarity_total_weight) + 1;
    int cumulative = 0;

    for (int i = 0; i < pool->entry_count; i++) {
        if (pool->entries[i].rarity == rarity) {
            cumulative += pool->entries[i].weight;
            if (roll <= cumulative) {
                ItemPoolEntry* entry = &pool->entries[i];

                if (entry->is_consumable && entry->item_factory_qty) {
                    int quantity = entry->min_quantity +
                                   (rand() % (entry->max_quantity - entry->min_quantity + 1));
                    return entry->item_factory_qty(quantity);
                } else if (entry->item_factory) {
                    return entry->item_factory();
                }
            }
        }
    }

    return create_item("Empty", ITEM_CONSUMABLE, NULL, 0, 0, 0, 0, 0, 0);
}

const char* get_rarity_name(ItemRarity rarity) {
    switch(rarity) {
        case RARITY_COMMON: return "Commun";
        case RARITY_UNCOMMON: return "Peu commun";
        case RARITY_RARE: return "Rare";
        case RARITY_EPIC: return "Epique";
        case RARITY_LEGENDARY: return "Legendaire";
        default: return "Inconnu";
    }
}

const char* get_rarity_color(ItemRarity rarity) {
    switch(rarity) {
        case RARITY_COMMON: return "\033[37m";      // White
        case RARITY_UNCOMMON: return "\033[32m";    // Green
        case RARITY_RARE: return "\033[34m";        // Blue
        case RARITY_EPIC: return "\033[35m";        // Purple
        case RARITY_LEGENDARY: return "\033[33m";   // Yellow/Gold
        default: return "\033[0m";                   // Reset
    }
}

void free_item_pool(ItemPool* pool) {
    if (pool) {
        free(pool->cumulative_weights);
        free(pool->entries);
    }
}

ItemPool create_shop_pool(void) {
    static ItemPoolEntry shop_entries[] = {
            // COMMON - Basic consumables from both inventories (weight: 60-100)
            {NULL, create_oxygen_capsule, RARITY_COMMON, 100, 1, 1, 3},
            {NULL, create_health_kit, RARITY_COMMON, 100, 1, 1, 2},
            {NULL, create_fatigue_stimulant, RARITY_COMMON, 80, 1, 1, 3},
            {NULL, create_energy_drink, RARITY_COMMON, 70, 1, 1, 2},
            {NULL, create_antidote, RARITY_COMMON, 60, 1, 1, 2},

            // UNCOMMON - Enhanced consumables and starter weapons (weight: 30-50)
            {NULL, create_mega_oxygen, RARITY_UNCOMMON, 50, 1, 1, 2},
            {NULL, create_adrenaline_shot, RARITY_UNCOMMON, 45, 1, 1, 2},
            {NULL, create_emergency_medkit, RARITY_UNCOMMON, 40, 1, 1, 1},
            {create_epee, NULL, RARITY_UNCOMMON, 35, 0, 0, 0},
            {create_combinaison, NULL, RARITY_UNCOMMON, 30, 0, 0, 0},

            // RARE - Advanced items (weight: 15-25)
            {NULL, create_nanobots, RARITY_RARE, 25, 1, 1, 2},
            {create_harpon, NULL, RARITY_RARE, 20, 0, 0, 0},
            {create_reinforced_suit, NULL, RARITY_RARE, 18, 0, 0, 0},
            {create_net_launcher, NULL, RARITY_RARE, 15, 0, 0, 0},

            // EPIC - Premium equipment (weight: 7-12)
            {NULL, create_full_restore, RARITY_EPIC, 12, 1, 1, 1},
            {create_trident, NULL, RARITY_EPIC, 10, 0, 0, 0},
            {create_coral_staff, NULL, RARITY_EPIC, 8, 0, 0, 0},
            {create_power_armor, NULL, RARITY_EPIC, 7, 0, 0, 0},

            // LEGENDARY - Rare elite gear (weight: 2-5)
            {create_sonic_blade, NULL, RARITY_LEGENDARY, 5, 0, 0, 0},
            {create_abyssal_hammer, NULL, RARITY_LEGENDARY, 3, 0, 0, 0},
            {create_stealth_suit, NULL, RARITY_LEGENDARY, 2, 0, 0, 0},
    };

    int count = sizeof(shop_entries) / sizeof(ItemPoolEntry);
    ItemPoolEntry* entries = malloc(sizeof(ItemPoolEntry) * count);
    memcpy(entries, shop_entries, sizeof(shop_entries));

    return create_item_pool(entries, count);
}

ItemPool create_loot_pool(void) {
    static ItemPoolEntry loot_entries[] = {
            // COMMON - High drop rate basics (weight: 90-150)
            {NULL, create_oxygen_capsule, RARITY_COMMON, 150, 1, 1, 5},
            {NULL, create_health_kit, RARITY_COMMON, 120, 1, 1, 3},
            {NULL, create_fatigue_stimulant, RARITY_COMMON, 100, 1, 1, 4},
            {NULL, create_energy_drink, RARITY_COMMON, 90, 1, 2, 4},

            // UNCOMMON - Medium drop rate (weight: 45-60)
            {NULL, create_mega_oxygen, RARITY_UNCOMMON, 60, 1, 1, 3},
            {NULL, create_adrenaline_shot, RARITY_UNCOMMON, 55, 1, 1, 2},
            {NULL, create_emergency_medkit, RARITY_UNCOMMON, 50, 1, 1, 2},
            {NULL, create_antidote, RARITY_UNCOMMON, 45, 1, 1, 3},
            {create_epee, NULL, RARITY_UNCOMMON, 30, 0, 0, 0},

            // RARE - Lower drop rate (weight: 20-35)
            {NULL, create_nanobots, RARITY_RARE, 35, 1, 1, 2},
            {create_harpon, NULL, RARITY_RARE, 25, 0, 0, 0},
            {create_combinaison, NULL, RARITY_RARE, 22, 0, 0, 0},
            {create_reinforced_suit, NULL, RARITY_RARE, 20, 0, 0, 0},

            // EPIC - Rare combat drops (weight: 8-15)
            {NULL, create_full_restore, RARITY_EPIC, 15, 1, 1, 1},
            {create_trident, NULL, RARITY_EPIC, 12, 0, 0, 0},
            {create_net_launcher, NULL, RARITY_EPIC, 10, 0, 0, 0},
            {create_coral_staff, NULL, RARITY_EPIC, 8, 0, 0, 0},

            // LEGENDARY - Extremely rare (weight: 1-6)
            {create_power_armor, NULL, RARITY_LEGENDARY, 6, 0, 0, 0},
            {create_sonic_blade, NULL, RARITY_LEGENDARY, 4, 0, 0, 0},
            {create_abyssal_hammer, NULL, RARITY_LEGENDARY, 2, 0, 0, 0},
            {create_stealth_suit, NULL, RARITY_LEGENDARY, 1, 0, 0, 0},
    };

    int count = sizeof(loot_entries) / sizeof(ItemPoolEntry);
    ItemPoolEntry* entries = malloc(sizeof(ItemPoolEntry) * count);
    memcpy(entries, loot_entries, sizeof(loot_entries));

    return create_item_pool(entries, count);
}
