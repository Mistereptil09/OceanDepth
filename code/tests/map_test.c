//
// Map system test
//
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "core/map.h"
#include "core/player.h"
#include "core/error_codes.h"

void test_map_creation() {
    printf("\n=== Test: Map Creation ===\n");

    int seed = 12345;
    Map* map = create_map(seed);

    assert(map != NULL);
    assert(map->rows == 4);
    assert(map->cols == 4);
    assert(map->seed == seed);
    assert(map->grid != NULL);

    printf("✓ Map created with correct dimensions and seed\n");

    free_map(map);
    printf("✓ Map freed successfully\n");
}

void test_map_generation_deterministic() {
    printf("\n=== Test: Deterministic Generation ===\n");

    int seed = 99999;
    Map* map1 = create_map(seed);
    Map* map2 = create_map(seed);

    // Same seed should produce same map
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            Cell* cell1 = get_cell(map1, r, c);
            Cell* cell2 = get_cell(map2, r, c);
            assert(cell1->type == cell2->type);
        }
    }

    printf("✓ Same seed produces identical maps\n");

    free_map(map1);
    free_map(map2);
}

void test_surface_generation() {
    printf("\n=== Test: Surface Generation ===\n");

    Map* map = create_map(time(NULL));

    // Surface should have specific layout
    assert(get_cell(map, 0, 0)->type == SHOP);
    assert(get_cell(map, 0, 1)->type == HEAL);
    assert(get_cell(map, 0, 2)->type == SAVE);
    assert(get_cell(map, 0, 3)->type == EMPTY);

    printf("✓ Surface has correct static layout\n");

    free_map(map);
}

void test_depth_generation() {
    printf("\n=== Test: Depth Generation ===\n");

    Map* map = create_map(time(NULL));

    // Depth 1: Should have REEF, CAVE, SHIPWRECK
    int reef_count = 0, cave_count = 0, shipwreck_count = 0;
    for (int c = 0; c < 4; c++) {
        CellType type = get_cell(map, 1, c)->type;
        if (type == REEF) reef_count++;
        else if (type == CAVE) cave_count++;
        else if (type == SHIPWRECK) shipwreck_count++;
    }
    assert(reef_count == 2);
    assert(cave_count == 1);
    assert(shipwreck_count == 1);
    printf("✓ Depth 1 has correct composition (2 REEF, 1 CAVE, 1 SHIPWRECK)\n");

    // Depth 2: Should have SHIPWRECK, REEF, PIT
    reef_count = shipwreck_count = 0;
    int pit_count = 0;
    for (int c = 0; c < 4; c++) {
        CellType type = get_cell(map, 2, c)->type;
        if (type == REEF) reef_count++;
        else if (type == SHIPWRECK) shipwreck_count++;
        else if (type == PIT) pit_count++;
    }
    assert(shipwreck_count == 2);
    assert(reef_count == 1);
    assert(pit_count == 1);
    printf("✓ Depth 2 has correct composition (2 SHIPWRECK, 1 REEF, 1 PIT)\n");

    // Depth 3: Should have CAVE at start, PITs, ABYSS at end
    assert(get_cell(map, 3, 0)->type == CAVE);
    assert(get_cell(map, 3, 1)->type == PIT);
    assert(get_cell(map, 3, 2)->type == PIT);
    assert(get_cell(map, 3, 3)->type == ABYSS);
    printf("✓ Depth 3 has correct layout (CAVE, PIT, PIT, ABYSS)\n");

    free_map(map);
}

void test_get_cell() {
    printf("\n=== Test: Get Cell ===\n");

    Map* map = create_map(12345);

    // Valid cells
    assert(get_cell(map, 0, 0) != NULL);
    assert(get_cell(map, 3, 3) != NULL);

    // Out of bounds
    assert(get_cell(map, -1, 0) == NULL);
    assert(get_cell(map, 0, -1) == NULL);
    assert(get_cell(map, 4, 0) == NULL);
    assert(get_cell(map, 0, 4) == NULL);

    // NULL map
    assert(get_cell(NULL, 0, 0) == NULL);

    printf("✓ get_cell handles valid and invalid inputs correctly\n");

    free_map(map);
}

void test_player_position() {
    printf("\n=== Test: Player Position ===\n");

    Position start = {0, 0};
    Position max = {0, 0};
    Player* player = create_player("TestPlayer", 100, 10, 100, start, max);

    assert(player != NULL);
    assert(player->current_position.row == 0);
    assert(player->current_position.col == 0);
    assert(player->max_position.row == 0);
    assert(player->max_position.col == 0);

    printf("✓ Player created with correct positions\n");

    free_player(player);
}

void test_unlock_new_position() {
    printf("\n=== Test: Unlock New Position ===\n");

    Position start = {0, 0};
    Position max = {0, 0};
    Player* player = create_player("TestPlayer", 100, 10, 100, start, max);

    // Player at (0,0), max at (0,0) - should unlock (0,1)
    int result = unlock_new_position(player);
    assert(result == NEW_CELL);
    assert(player->max_position.row == 0);
    assert(player->max_position.col == 1);
    printf("✓ Unlocked next cell in same row\n");

    // Move to end of row and unlock
    player->current_position.col = 3;
    player->max_position.col = 3;
    result = unlock_new_position(player);
    assert(result == NEW_ROW);
    assert(player->max_position.row == 1);
    assert(player->max_position.col == 0);
    printf("✓ Unlocked first cell of next row\n");

    // Try unlocking when not at frontier
    player->current_position.row = 0;
    player->current_position.col = 0;
    result = unlock_new_position(player);
    assert(result == INVALID_INPUT);
    printf("✓ Cannot unlock when not at frontier\n");

    // Unlock all the way to end
    player->current_position.row = 3;
    player->current_position.col = 3;
    player->max_position.row = 3;
    player->max_position.col = 3;
    result = unlock_new_position(player);
    assert(result == WIN);
    printf("✓ Returns WIN when at final cell\n");

    free_player(player);
}

void test_is_valid_move() {
    printf("\n=== Test: Movement Validation ===\n");

    Map* map = create_map(12345);
    Position start = {0, 0};
    Position max = {0, 1};  // Can access (0,0) and (0,1)
    Player* player = create_player("TestPlayer", 100, 10, 100, start, max);

    // Valid moves (adjacent and unlocked)
    Position right = {0, 1};
    assert(is_valid_move(map, player, right) == 1);
    printf("✓ Can move right to unlocked adjacent cell\n");

    Position down = {1, 0};
    assert(is_valid_move(map, player, down) == 0);  // Not unlocked
    printf("✓ Cannot move to locked cell\n");

    // Invalid: diagonal
    Position diagonal = {1, 1};
    assert(is_valid_move(map, player, diagonal) == 0);
    printf("✓ Cannot move diagonally\n");

    // Invalid: too far
    Position far = {0, 3};
    assert(is_valid_move(map, player, far) == 0);
    printf("✓ Cannot jump multiple cells\n");

    // Invalid: out of bounds
    Position oob = {-1, 0};
    assert(is_valid_move(map, player, oob) == 0);
    printf("✓ Cannot move out of bounds\n");

    // Unlock more and test
    player->max_position.row = 1;
    player->max_position.col = 0;
    assert(is_valid_move(map, player, down) == 1);  // Now unlocked
    printf("✓ Can move to newly unlocked cell\n");

    free_player(player);
    free_map(map);
}

void test_player_move_to() {
    printf("\n=== Test: Player Movement ===\n");

    Position start = {0, 0};
    Position max = {3, 3};
    Player* player = create_player("TestPlayer", 100, 10, 100, start, max);

    Position new_pos = {1, 2};
    int result = player_move_to(player, new_pos);

    assert(result == SUCCESS);
    assert(player->current_position.row == 1);
    assert(player->current_position.col == 2);
    printf("✓ Player moved to new position\n");

    // NULL player
    result = player_move_to(NULL, new_pos);
    assert(result == POINTER_NULL);
    printf("✓ Handles NULL player gracefully\n");

    free_player(player);
}

int main(void) {
    printf("\n========================================\n");
    printf("    MAP SYSTEM TEST SUITE\n");
    printf("========================================\n");

    test_map_creation();
    test_map_generation_deterministic();
    test_surface_generation();
    test_depth_generation();
    test_get_cell();
    test_player_position();
    test_unlock_new_position();
    test_is_valid_move();
    test_player_move_to();

    printf("\n========================================\n");
    printf("    ALL TESTS PASSED! ✓\n");
    printf("========================================\n\n");

    return 0;
}