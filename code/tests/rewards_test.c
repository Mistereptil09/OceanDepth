//
// Created by anton on 11/2/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "core/reward_system.h"
#include "core/inventory.h"
#include "core/inventory_data.h"
#include "core/player.h"
#include "core/item.h"
#include "core/error_codes.h"
#include "core/creature.h"

// ======================= TEST HELPERS =======================

void print_inventory(Inventory* inv) {
    printf("Inventory (%d/%d items):\n", inv->count, INVENTORY_SIZE);
    for (int i = 0; i < inv->count; i++) {
        printf("  [%d] %s (quantity: %d, price: %d)\n",
               i + 1, inv->items[i].name, inv->items[i].quantity, inv->items[i].price);
    }
}

void print_test_header(const char* test_name) {
    printf("\n========================================\n");
    printf("TEST: %s\n", test_name);
    printf("========================================\n");
}

void print_test_result(int passed) {
    printf("Result: %s\n", passed ? "PASSED" : "FAILED");
}

// ======================= INVENTORY TESTS =======================

int test_create_inventory() {
    print_test_header("Create Inventory");

    Inventory* inv = create_inventory();
    assert(inv != NULL);

    printf("Created inventory with %d initial items\n", inv->count);
    print_inventory(inv);

    // Verify initial items
    assert(inv->count == 6);
    assert(strcmp(inv->items[0].name, "Epee") == 0 || strlen(inv->items[0].name) > 0);

    free_inventory(inv);
    free(inv);

    print_test_result(1);
    return 1;
}

int test_add_item_to_inventory() {
    print_test_header("Add Item to Inventory");

    Inventory* inv = create_inventory();
    int initial_count = inv->count;

    printf("Initial count: %d\n", initial_count);

    // Create a test item
    Item test_item = create_oxygen_capsule(1);

    // Try to add item
    int result = add_item_to_inventory(inv, &test_item);

    printf("Add result: %d (SUCCESS=%d)\n", result, SUCCESS);
    printf("New count: %d\n", inv->count);

    if (initial_count < INVENTORY_SIZE) {
        assert(result == SUCCESS);
        assert(inv->count == initial_count + 1);
        printf("Successfully added item\n");
    } else {
        assert(result == INVENTORY_FULL);
        printf("Inventory was full as expected\n");
    }

    print_inventory(inv);

    free_inventory(inv);
    free(inv);

    print_test_result(1);
    return 1;
}

int test_inventory_full() {
    print_test_header("Inventory Full Scenario");

    Inventory* inv = create_inventory();

    // Fill inventory to capacity
    while (inv->count < INVENTORY_SIZE) {
        Item item = create_health_kit(1);
        add_item_to_inventory(inv, &item);
    }

    printf("Filled inventory to %d/%d items\n", inv->count, INVENTORY_SIZE);

    // Try to add one more item
    Item extra_item = create_oxygen_capsule(1);
    int result = add_item_to_inventory(inv, &extra_item);

    printf("Attempt to add to full inventory: result=%d (INVENTORY_FULL=%d)\n",
           result, INVENTORY_FULL);

    assert(result == INVENTORY_FULL);
    assert(inv->count == INVENTORY_SIZE);

    free_item(&extra_item);
    free_inventory(inv);
    free(inv);

    print_test_result(1);
    return 1;
}

int test_remove_item_by_index() {
    print_test_header("Remove Item by Index");

    Inventory* inv = create_inventory();
    int initial_count = inv->count;

    printf("Initial count: %d\n", initial_count);
    print_inventory(inv);

    // Remove item at index 0
    int result = remove_item_by_index(inv, 0);

    printf("\nAfter removing index 0:\n");
    printf("Result: %d (SUCCESS=%d)\n", result, SUCCESS);
    printf("New count: %d\n", inv->count);
    print_inventory(inv);

    assert(result == SUCCESS);
    assert(inv->count == initial_count - 1);

    // Try invalid index
    result = remove_item_by_index(inv, 100);
    printf("\nAttempt to remove invalid index 100: result=%d\n", result);
    assert(result != SUCCESS);

    free_inventory(inv);
    free(inv);

    print_test_result(1);
    return 1;
}

int test_inventory_has_space() {
    print_test_header("Inventory Has Space Check");

    Inventory* inv = create_inventory();

    printf("Initial state:\n");
    printf("  Count: %d/%d\n", inv->count, INVENTORY_SIZE);
    printf("  Has space: %s\n", inventory_has_space(inv) ? "YES" : "NO");

    // Fill to capacity
    while (inv->count < INVENTORY_SIZE) {
        Item item = create_health_kit(1);
        add_item_to_inventory(inv, &item);
    }

    printf("\nAfter filling:\n");
    printf("  Count: %d/%d\n", inv->count, INVENTORY_SIZE);
    printf("  Has space: %s\n", inventory_has_space(inv) ? "YES" : "NO");

    assert(!inventory_has_space(inv));

    free_inventory(inv);
    free(inv);

    print_test_result(1);
    return 1;
}

int test_inventory_get_free_slots() {
    print_test_header("Get Free Slots");

    Inventory* inv = create_inventory();

    int free_slots = inventory_get_free_slots(inv);
    int expected = INVENTORY_SIZE - inv->count;

    printf("Current count: %d\n", inv->count);
    printf("Free slots: %d\n", free_slots);
    printf("Expected: %d\n", expected);

    assert(free_slots == expected);

    // Add an item
    Item item = create_oxygen_capsule(1);
    add_item_to_inventory(inv, &item);

    int new_free_slots = inventory_get_free_slots(inv);
    printf("\nAfter adding one item:\n");
    printf("Free slots: %d\n", new_free_slots);
    printf("Expected: %d\n", free_slots - 1);

    assert(new_free_slots == free_slots - 1);

    free_inventory(inv);
    free(inv);

    print_test_result(1);
    return 1;
}

// Callback for testing inventory replacement
static int test_replace_callback(Inventory* inv, Item* new_item) {
    (void)new_item; // Unused
    // Always replace the first item (index 0)
    return 0;
}

int test_add_item_with_callback() {
    print_test_header("Add Item with Callback (Replace)");

    Inventory* inv = create_inventory();

    // Fill inventory
    while (inv->count < INVENTORY_SIZE) {
        Item item = create_health_kit(1);
        add_item_to_inventory(inv, &item);
    }

    printf("Inventory filled to capacity: %d items\n", inv->count);

    // Get name of first item
    char first_item_name[30];
    strncpy(first_item_name, inv->items[0].name, sizeof(first_item_name) - 1);
    first_item_name[29] = '\0';

    printf("First item before: %s\n", first_item_name);

    // Try to add with callback
    Item new_item = create_fatigue_stimulant(1);
    int result = add_item_to_inventory_with_callback(inv, &new_item, test_replace_callback);

    printf("Add with callback result: %d (SUCCESS=%d)\n", result, SUCCESS);
    printf("First item after: %s\n", inv->items[0].name);
    printf("Count: %d\n", inv->count);

    assert(result == SUCCESS);
    assert(inv->count == INVENTORY_SIZE); // Should still be full
    assert(strcmp(inv->items[0].name, first_item_name) != 0); // First item should be different

    print_inventory(inv);

    free_inventory(inv);
    free(inv);

    print_test_result(1);
    return 1;
}

// ======================= REWARD SYSTEM TESTS =======================

// Note: The reward system uses interface callbacks which are hard to test in isolation
// We'll test the components we can without the full interface

int test_reward_system_with_null_player() {
    print_test_header("Reward System with NULL Player");

    // Should not crash with NULL player
    award_post_battle_rewards(NULL, EASY);

    printf("No crash with NULL player - PASS\n");

    print_test_result(1);
    return 1;
}

int test_reward_system_integration() {
    print_test_header("Reward System Integration (Manual)");

    printf("Note: Full reward system testing requires interface setup.\n");
    printf("The reward system depends on:\n");
    printf("  - current_interface global\n");
    printf("  - User input callbacks\n");
    printf("  - Item pool generation\n");
    printf("\nThese components should be tested in integration tests.\n");
    printf("For now, we verify the basic structure is sound.\n");

    // Create a player
    Position position = {0};
    Player* player = create_player("TestPlayer", 100, 10, 100, position, position);
    assert(player != NULL);

    printf("\nPlayer created successfully\n");
    printf("Inventory slots: %d/%d\n", player->inventory.count, INVENTORY_SIZE);

    // The actual award_post_battle_rewards call would require interface setup
    // which is beyond the scope of a unit test

    free_player(player);

    print_test_result(1);
    return 1;
}

// ======================= MAIN TEST RUNNER =======================

int main(void) {
    printf("\n\n");
    printf("========================================\n");
    printf("     REWARD SYSTEM & INVENTORY MANAGEMENT TESTS\n");
    printf("========================================\n");

    int total_tests = 0;
    int passed_tests = 0;

    // Inventory Tests
    printf("\n========================================\n");
    printf("              INVENTORY TESTS\n");
    printf("========================================\n");

    total_tests++; passed_tests += test_create_inventory();
    total_tests++; passed_tests += test_add_item_to_inventory();
    total_tests++; passed_tests += test_inventory_full();
    total_tests++; passed_tests += test_remove_item_by_index();
    total_tests++; passed_tests += test_inventory_has_space();
    total_tests++; passed_tests += test_inventory_get_free_slots();
    total_tests++; passed_tests += test_add_item_with_callback();

    // Reward System Tests
    printf("\n========================================\n");
    printf("            REWARD SYSTEM TESTS\n");
    printf("========================================\n");

    total_tests++; passed_tests += test_reward_system_with_null_player();
    total_tests++; passed_tests += test_reward_system_integration();

    // Summary
    printf("\n========================================\n");
    printf("                    TEST SUMMARY\n");
    printf("----------------------------------------\n");
    printf("  Total Tests:  %2d\n", total_tests);
    printf("  Passed:       %2d\n", passed_tests);
    printf("  Failed:       %2d\n", total_tests - passed_tests);
    printf("========================================\n");

    if (passed_tests == total_tests) {
        printf("\nALL TESTS PASSED!\n");
        return 0;
    } else {
        printf("\nSOME TESTS FAILED!\n");
        return 1;
    }
}