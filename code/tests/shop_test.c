//
// Created by anton on 11/2/2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "core/shop.h"
#include "core/item_pool.h"
#include "core/inventory_data.h"
#include "core/item_data.h"
#include "interface/interface_table.h"
#include "interface/interface_test.h"

/**
 * Compile command:
 * gcc -fsanitize=address -g -O1 -Icode/includes \
 * code/tests/shop_test.c \
 * code/src/core/shop.c \
 * code/src/core/item_pool.c \
 * code/src/core/inventory_data.c \
 * code/src/core/item_data.c \
 * code/src/core/item.c \
 * code/src/core/effect.c \
 * code/tests/interface_test.c \
 * -o shop_test
 */

// Test helper to display shop slot
void display_shop_slot(ShopSlot* slot, int index) {
    const char* rarity_names[] = {"COMMON", "UNCOMMON", "RARE", "EPIC", "LEGENDARY"};
    printf("  [%d] %-25s | Price: %3d | Stock: %2d | Rarity: %s\n",
           index,
           slot->item.name,
           slot->current_price,
           slot->stock,
           rarity_names[slot->rarity]);
}

int main(void) {
    srand(time(NULL));

    // Set up test interface
    current_interface = &test_interface;

    printf("=== SHOP SYSTEM TESTS ===\n\n");

    // Test 1: Create item pools
    printf("1. Creating item pools...\n");
    ItemPool shop_pool = create_shop_pool();
    ItemPool loot_pool = create_loot_pool();
    printf("   Shop pool: %d items, total weight: %d\n",
           shop_pool.entry_count, shop_pool.total_weight);
    printf("   Loot pool: %d items, total weight: %d\n",
           loot_pool.entry_count, loot_pool.total_weight);

    // Test 2: Create shop
    printf("\n2. Creating shop...\n");
    Shop merchant = create_shop("Boutique des Profondeurs", &shop_pool);
    printf("   Shop '%s' created with %d items\n", merchant.name, merchant.slot_count);
    printf("   Refresh cost: %d\n", merchant.refresh_cost);

    // Display initial inventory
    printf("\n   Initial Inventory:\n");
    for (int i = 0; i < merchant.slot_count; i++) {
        display_shop_slot(&merchant.slots[i], i + 1);
    }

    // Test 3: Item pool drawing
    printf("\n3. Testing item pool random drawing (10 draws)...\n");
    int rarity_counts[5] = {0};
    for (int i = 0; i < 10; i++) {
        Item item = draw_random_item(&shop_pool);
        // Determine rarity by checking which pool entry matches
        printf("   Draw %d: %s (price: %d)\n", i + 1, item.name, item.price);
        free_item(&item);
    }

    // Test 4: Rarity-specific drawing
    printf("\n4. Testing rarity-specific drawing...\n");
    const char* rarity_names[] = {"COMMON", "UNCOMMON", "RARE", "EPIC", "LEGENDARY"};
    for (int r = 0; r < 5; r++) {
        Item item = draw_item_by_rarity(&shop_pool, r);
        printf("   %s: %s (price: %d)\n", rarity_names[r], item.name, item.price);
        free_item(&item);
    }

    // Test 5: Shop buying mechanics
    printf("\n5. Testing shop buying...\n");
    int player_gold = 500;
    printf("   Player starts with %d gold\n", player_gold);

    // Try to buy first item
    printf("   Attempting to buy item 0 (%s)...\n", merchant.slots[0].item.name);
    int initial_stock = merchant.slots[0].stock;
    int price = merchant.slots[0].current_price;
    int success = shop_buy_item(&merchant, 0, &player_gold, NULL);
    printf("   Purchase %s\n", success ? "succeeded" : "failed");
    printf("   Gold remaining: %d (spent: %d)\n", player_gold, 500 - player_gold);
    printf("   Stock changed: %d -> %d\n", initial_stock, merchant.slots[0].stock);

    // Try to buy when out of stock
    printf("\n   Draining stock of item 0...\n");
    while (merchant.slots[0].stock > 0 && player_gold >= merchant.slots[0].current_price) {
        shop_buy_item(&merchant, 0, &player_gold, NULL);
    }
    printf("   Stock: %d, Gold: %d\n", merchant.slots[0].stock, player_gold);
    printf("   Attempting to buy out-of-stock item...\n");
    success = shop_buy_item(&merchant, 0, &player_gold, NULL);
    printf("   Purchase %s (expected: failed)\n", success ? "succeeded" : "failed");

    // Try to buy when insufficient gold
    player_gold = 5;
    printf("\n   Setting gold to %d (insufficient for most items)\n", player_gold);
    printf("   Attempting to buy expensive item...\n");
    success = shop_buy_item(&merchant, merchant.slot_count - 1, &player_gold, NULL);
    printf("   Purchase %s (expected: failed)\n", success ? "succeeded" : "failed");

    // Test 6: Shop selling mechanics
    printf("\n6. Testing shop selling...\n");
    player_gold = 100;
    Item test_item = create_oxygen_capsule(3);
    test_item.price = 50;
    printf("   Selling %s (price: %d)...\n", test_item.name, test_item.price);
    int sell_price = shop_sell_item(&merchant, &test_item, &player_gold);
    printf("   Sold for %d gold (50%% of price)\n", sell_price);
    printf("   Gold after sale: %d\n", player_gold);
    free_item(&test_item);

    // Test 7: Shop refresh
    printf("\n7. Testing shop refresh...\n");
    player_gold = 1000;
    printf("   Player gold: %d, Refresh cost: %d\n", player_gold, merchant.refresh_cost);
    printf("   Items before refresh: %d\n", merchant.slot_count);
    printf("   First item before: %s\n", merchant.slots[0].item.name);

    success = shop_refresh_inventory(&merchant, &player_gold);
    printf("   Refresh %s\n", success ? "succeeded" : "failed");
    printf("   Gold remaining: %d\n", player_gold);
    printf("   Items after refresh: %d\n", merchant.slot_count);
    printf("   First item after: %s\n", merchant.slots[0].item.name);
    printf("   New refresh cost: %d (increased by 10)\n", merchant.refresh_cost);

    // Test 8: Discount application
    printf("\n8. Testing discount system...\n");
    printf("   Prices before discount:\n");
    for (int i = 0; i < 3 && i < merchant.slot_count; i++) {
        printf("     Item %d: %d gold\n", i, merchant.slots[i].current_price);
    }

    shop_apply_discount(&merchant, 0.25f); // 25% discount
    printf("   Applied 25%% discount\n");
    printf("   Prices after discount:\n");
    for (int i = 0; i < 3 && i < merchant.slot_count; i++) {
        printf("     Item %d: %d gold (was %d)\n",
               i, merchant.slots[i].current_price, merchant.slots[i].original_price);
    }

    // Test 9: Restock
    printf("\n9. Testing restock system...\n");
    // Buy some items to reduce stock
    player_gold = 10000;
    for (int i = 0; i < 3 && i < merchant.slot_count; i++) {
        int orig_stock = merchant.slots[i].stock;
        shop_buy_item(&merchant, i, &player_gold, NULL);
        printf("   Item %d stock: %d -> %d\n", i, orig_stock, merchant.slots[i].stock);
    }

    printf("   Restocking...\n");
    shop_restock(&merchant);
    printf("   Stock after restock:\n");
    for (int i = 0; i < 3 && i < merchant.slot_count; i++) {
        printf("     Item %d: %d\n", i, merchant.slots[i].stock);
    }

    // Test 10: Sell price calculation
    printf("\n10. Testing sell price calculation...\n");
    Item items_to_test[] = {
            create_oxygen_capsule(1),
            create_health_kit(1),
            create_trident(),
            create_sonic_blade()
    };
    items_to_test[0].price = 10;
    items_to_test[1].price = 15;
    items_to_test[2].price = 100;
    items_to_test[3].price = 200;

    for (int i = 0; i < 4; i++) {
        int sell_price = get_item_sell_price(&items_to_test[i]);
        printf("   %s: Buy price %d -> Sell price %d (50%%)\n",
               items_to_test[i].name,
               items_to_test[i].price,
               sell_price);
        free_item(&items_to_test[i]);
    }

    // Test 11: Display shop function
    printf("\n11. Testing display_shop interface...\n");
    player_gold = 300;
    display_shop(&merchant, player_gold);
    printf("   (Check output above for proper formatting)\n");

    // Test 12: Edge cases
    printf("\n12. Testing edge cases...\n");

    // Invalid slot index
    printf("   Buying invalid slot index...\n");
    success = shop_buy_item(&merchant, -1, &player_gold, NULL);
    printf("   Result: %s (expected: failed)\n", success ? "succeeded" : "failed");

    success = shop_buy_item(&merchant, 999, &player_gold, NULL);
    printf("   Result: %s (expected: failed)\n", success ? "succeeded" : "failed");

    // Refresh without enough gold
    player_gold = 10;
    printf("   Refreshing with insufficient gold (%d)...\n", player_gold);
    success = shop_refresh_inventory(&merchant, &player_gold);
    printf("   Result: %s (expected: failed)\n", success ? "succeeded" : "failed");

    // Test 13: Multiple refreshes (progressive cost increase)
    printf("\n13. Testing progressive refresh costs...\n");
    player_gold = 10000;
    for (int i = 0; i < 5; i++) {
        int cost_before = merchant.refresh_cost;
        shop_refresh_inventory(&merchant, &player_gold);
        printf("   Refresh %d: Cost was %d, now %d, Gold left: %d\n",
               i + 1, cost_before, merchant.refresh_cost, player_gold);
    }

    // Test 14: Loot pool comparison
    printf("\n14. Comparing shop vs loot pools...\n");
    printf("   Drawing 5 items from each pool:\n");
    printf("   SHOP POOL:\n");
    for (int i = 0; i < 5; i++) {
        Item item = draw_random_item(&shop_pool);
        printf("     %s (price: %d)\n", item.name, item.price);
        free_item(&item);
    }
    printf("   LOOT POOL:\n");
    for (int i = 0; i < 5; i++) {
        Item item = draw_random_item(&loot_pool);
        printf("     %s (price: %d)\n", item.name, item.price);
        free_item(&item);
    }

    // Test 15: Memory cleanup
    printf("\n15. Cleaning up resources...\n");
    free_shop(&merchant);
    free_item_pool(&shop_pool);
    free_item_pool(&loot_pool);
    printf("   All resources freed\n");

    printf("\n=== ALL SHOP TESTS COMPLETED ===\n");
    return 0;
}
