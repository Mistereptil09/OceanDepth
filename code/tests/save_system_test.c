//
// Test file for save system functionality
//

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "core/save_system.h"
#include "core/player.h"
#include "core/item.h"
#include "core/error_codes.h"

void test_save_and_load() {
    printf("=== Testing Save and Load System ===\n");

    // Create a test player
    Position position = {0};

    Player* original = create_player("TestPlayer", 100, 10, 80, position, position);
    assert(original != NULL);

    // Modify player state
    original->base.current_health_points = 75;
    original->base.oxygen_level = 60;
    original->base.fatigue_level = 2;
    original->pearls = 150;

    printf("Original Player:\n");
    printf("  Name: %s\n", original->base.name);
    printf("  HP: %d/%d\n", original->base.current_health_points, original->base.max_health_points);
    printf("  Oxygen: %d/%d\n", original->base.oxygen_level, original->base.max_oxygen_level);
    printf("  Fatigue: %d\n", original->base.fatigue_level);
    printf("  Pearls: %d\n", original->pearls);

    // Save the game
    int difficulty = 2;
    int battles_won = 5;

    printf("\nSaving game...\n");
    int result = save_game_modular(original, difficulty, battles_won, SAVE_ALL_DATA);
    assert(result == SUCCESS);

    // Check if save file exists
    assert(save_file_exists() == 1);
    printf("Save file created successfully!\n");

    // Load the game
    Player* loaded = NULL;
    int loaded_difficulty;
    int loaded_battles_won;

    printf("\nLoading game...\n");
    result = load_game_modular(&loaded, &loaded_difficulty, &loaded_battles_won, SAVE_ALL_DATA);
    assert(result == SUCCESS);
    assert(loaded != NULL);

    printf("Loaded Player:\n");
    printf("  Name: %s\n", loaded->base.name);
    printf("  HP: %d/%d\n", loaded->base.current_health_points, loaded->base.max_health_points);
    printf("  Oxygen: %d/%d\n", loaded->base.oxygen_level, loaded->base.max_oxygen_level);
    printf("  Fatigue: %d\n", loaded->base.fatigue_level);
    printf("  Pearls: %d\n", loaded->pearls);
    printf("  Difficulty: %d\n", loaded_difficulty);
    printf("  Battles Won: %d\n", loaded_battles_won);

    // Verify loaded data matches original
    assert(strcmp(loaded->base.name, original->base.name) == 0);
    assert(loaded->base.current_health_points == original->base.current_health_points);
    assert(loaded->base.max_health_points == original->base.max_health_points);
    assert(loaded->base.oxygen_level == original->base.oxygen_level);
    assert(loaded->base.max_oxygen_level == original->base.max_oxygen_level);
    assert(loaded->base.fatigue_level == original->base.fatigue_level);
    assert(loaded->pearls == original->pearls);
    assert(loaded_difficulty == difficulty);
    assert(loaded_battles_won == battles_won);

    printf("\n✓ All data matches!\n");

    // Clean up
    free_player(original);
    free_player(loaded);

    // Delete save file
    printf("\nDeleting save file...\n");
    result = delete_save_file();
    assert(result == SUCCESS);
    assert(save_file_exists() == 0);
    printf("Save file deleted successfully!\n");

    printf("\n=== All tests passed! ===\n");
}

void test_save_with_inventory() {
    printf("\n=== Testing Save with Inventory ===\n");

    // Create a player
    Position position = {0};
    Player* player = create_player("InventoryTest", 100, 10, 80, position, position);
    assert(player != NULL);

    // Add some items to inventory
    Action dummy_action = {0};
    Item item1 = create_item("Healing Potion", ITEM_CONSUMABLE, &dummy_action, 1, 1, 0, 0, 20, 10);
    Item item2 = create_item("Oxygen Tank", ITEM_CONSUMABLE, &dummy_action, 1, 1, 30, 0, 0, 15);

    add_item_to_inventory(&player->inventory, &item1);
    add_item_to_inventory(&player->inventory, &item2);

    printf("Original inventory count: %d\n", player->inventory.count);
    printf("  Item 1: %s (HP: %d)\n", player->inventory.items[0].name, player->inventory.items[0].hp_boost);
    printf("  Item 2: %s (O2: %d)\n", player->inventory.items[1].name, player->inventory.items[1].oxygen_boost);

    // Save
    printf("\nSaving game with inventory...\n");
    save_game_modular(player, 1, 3, SAVE_ALL_DATA);

    // Load
    Player* loaded = NULL;
    int diff, battles;
    printf("Loading game...\n");
    load_game_modular(&loaded, &diff, &battles, SAVE_ALL_DATA);

    printf("Loaded inventory count: %d\n", loaded->inventory.count);
    printf("  Item 1: %s (HP: %d)\n", loaded->inventory.items[0].name, loaded->inventory.items[0].hp_boost);
    printf("  Item 2: %s (O2: %d)\n", loaded->inventory.items[1].name, loaded->inventory.items[1].oxygen_boost);

    // Verify
    assert(loaded->inventory.count == player->inventory.count);
    assert(strcmp(loaded->inventory.items[0].name, player->inventory.items[0].name) == 0);
    assert(loaded->inventory.items[0].hp_boost == player->inventory.items[0].hp_boost);
    assert(strcmp(loaded->inventory.items[1].name, player->inventory.items[1].name) == 0);
    assert(loaded->inventory.items[1].oxygen_boost == player->inventory.items[1].oxygen_boost);

    printf("\n✓ Inventory saved and loaded correctly!\n");

    // Clean up
    free_player(player);
    free_player(loaded);
    delete_save_file();
}

int main(void) {
    test_save_and_load();
    test_save_with_inventory();

    printf("\n=== ALL SAVE SYSTEM TESTS PASSED ===\n");
    return 0;
}

