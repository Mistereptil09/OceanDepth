#include "../../tests/interface/interface_test.h"
#include "interface/interface_table.h"
#include "interface/interface_api.h"
#include "core/map.h"
#include "core/creature.h"
#include "core/entity.h"
#include "core/inventory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Test interface implementations - mostly silent, no user input required

void test_display_map(Map* map) {
    (void)map; // Unused in tests
    // Silent for tests
}

void test_display_combat_state(void) {
    // Silent for tests - could print if needed for debugging
}

void test_display_combat_intro(Creature** creatures, int count) {
    (void)creatures; // Unused in tests
    printf("Test: %d enemies generated\n", count);
}

void test_display_round_header(int round_number) {
    printf("Test: Round %d\n", round_number);
}

void test_display_victory(void) {
    printf("Test: Victory!\n");
}

void test_display_defeat(void) {
    printf("Test: Defeat!\n");
}

void test_display_battle_start(void) {
    printf("Test: Battle starting\n");
}

void test_wait_for_enter(const char* prompt) {
    // Don't wait - just continue immediately
    if (prompt) {
        printf("%s [auto-continue]\n", prompt);
    }
}

void test_show_action(const char* entity_name, const char* action_name) {
    (void)entity_name; // Unused in tests
    (void)action_name; // Unused in tests
    // Silent for tests
}

void test_show_defeat_by(const char* enemy_name) {
    printf("Test: Defeated by %s\n", enemy_name);
}

int test_get_choice(const char* prompt, const int min, const int max) {
    (void)max; // Unused in tests
    // For automated testing, always return the first valid choice
    printf("Test: Auto-choosing %d for '%s'\n", min, prompt);
    return min;
}

void test_show_attack(EntityBase* attacker, EntityBase* target, int damage) {
    (void)attacker; // Unused in tests
    (void)target; // Unused in tests
    (void)damage; // Unused in tests
    // Silent for tests
}

void test_show_inventory(Inventory* inv) {
    (void)inv; // Unused in tests
    // Silent for tests
}

void test_get_input(const char* prompt, char* result, size_t result_size) {
    (void)prompt; // Unused in tests
    // For tests, provide a default value
    snprintf(result, result_size, "TestInput");
}

// Define the test interface vtable
InterfaceVTable test_interface = {
    .display_map = test_display_map,
    .display_combat_state = test_display_combat_state,
    .display_combat_intro = test_display_combat_intro,
    .display_round_header = test_display_round_header,
    .display_victory = test_display_victory,
    .display_defeat = test_display_defeat,
    .display_battle_start = test_display_battle_start,
    .wait_for_enter = test_wait_for_enter,
    .show_action = test_show_action,
    .show_defeat_by = test_show_defeat_by,
    .get_choice = test_get_choice,
    .show_attack = test_show_attack,
    .show_inventory = test_show_inventory,
    .get_input = test_get_input
};

