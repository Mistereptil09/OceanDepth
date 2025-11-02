#include "interface_test.h"
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

// ========== COMBAT FEEDBACK FUNCTIONS (for new interface API) ==========

void test_show_oxygen_consumed(int amount, int current, int max) {
    (void)amount; (void)current; (void)max;
    // Silent for tests
}

void test_show_oxygen_critical(int current) {
    (void)current;
    // Silent for tests
}

void test_show_oxygen_death(int damage, int hp, int max_hp) {
    (void)damage; (void)hp; (void)max_hp;
    // Silent for tests
}

void test_show_fatigue_status(int fatigue, int max_actions) {
    (void)fatigue; (void)max_actions;
    // Silent for tests
}

void test_show_fatigue_increased(int new_fatigue) {
    (void)new_fatigue;
    // Silent for tests
}

void test_show_fatigue_recovered(int new_fatigue) {
    (void)new_fatigue;
    // Silent for tests
}

void test_show_passive_oxygen(int amount, int current, int max) {
    (void)amount; (void)current; (void)max;
    // Silent for tests
}

void test_show_damage_dealt(const char* attacker_name, const char* target_name,
                            int damage, int target_hp, int target_max_hp) {
    (void)attacker_name; (void)target_name; (void)damage; (void)target_hp; (void)target_max_hp;
    // Silent for tests
}

void test_show_attack_blocked(const char* defender_name) {
    (void)defender_name;
    // Silent for tests
}

void test_show_creature_defeated(const char* creature_name) {
    (void)creature_name;
    // Silent for tests
}

void test_show_actions_taken(int actions_taken) {
    (void)actions_taken;
    // Silent for tests
}

void test_show_death_from_afflictions(void) {
    // Silent for tests
}

void test_show_death_from_suffocation(void) {
    // Silent for tests
}

void test_show_enemy_turn(void) {
    // Silent for tests
}

void test_show_your_turn(void) {
    // Silent for tests
}

void test_show_ending_turn(void) {
    // Silent for tests
}

void test_show_action_effect(const char* message) {
    (void)message;
    // Silent for tests
}

void test_show_effect_error(void) {
    // Silent for tests
}

void test_show_action_on_cooldown(const char* action_name) {
    (void)action_name;
    // Silent for tests
}

void test_show_item_on_cooldown(const char* item_name) {
    (void)item_name;
    // Silent for tests
}

void test_show_no_actions_available(const char* creature_name) {
    (void)creature_name;
    // Silent for tests
}

void test_show_creature_died_from_effects(const char* creature_name) {
    (void)creature_name;
    // Silent for tests
}


void test_display_shop(const char* shop_name, int player_gold, int refresh_cost,
                       const char** items, const int* prices, const int* stocks,
                       const int* rarities, const int* can_afford, int item_count) {
    printf("\n--- SHOP: %s ---\n", shop_name);
    printf("Gold: %d | Refresh: %d\n", player_gold, refresh_cost);
    for (int i = 0; i < item_count; i++) {
        const char* rarity_str[] = {"COM", "UNC", "RARE", "EPIC", "LEGEND"};
        printf(" %d. %-20s [%s] %3dg (Stock:%2d)%s\n",
               i + 1, items[i], rarity_str[rarities[i]], prices[i], stocks[i],
               can_afford[i] ? "" : " [TOO EXPENSIVE]");
    }
}

void test_show_purchase_success(const char* item_name, int price, int quantity) {
    printf("Test: Purchased %s for %d gold", item_name, price);
    if (quantity > 0) printf(" (x%d)", quantity);
    printf("\n");
}

void test_show_purchase_failed(const char* reason) {
    printf("Test: Purchase failed - %s\n", reason);
}

void test_show_sell_success(const char* item_name, int gold_received) {
    printf("Test: Sold %s for %d gold\n", item_name, gold_received);
}

void test_show_sell_failed(const char* reason) {
    printf("Test: Sell failed - %s\n", reason);
}

void test_show_shop_refreshed(void) {
    printf("Test: Shop refreshed\n");
}

void test_show_refresh_failed(int cost, int player_gold) {
    printf("Test: Refresh failed - need %d, have %d\n", cost, player_gold);
}

void test_show_discount_applied(int discount_percent) {
    printf("Test: Discount applied - %d%% off\n", discount_percent);
}

void test_show_shop_restocked(void) {
    printf("Test: Shop restocked\n");
}

void test_show_information(const char* message) {
    (void)message; // Silent in tests
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
    .get_input = test_get_input,
    .show_information = test_show_information,

    // Combat feedback functions
    .show_oxygen_consumed = test_show_oxygen_consumed,
    .show_oxygen_critical = test_show_oxygen_critical,
    .show_oxygen_death = test_show_oxygen_death,
    .show_fatigue_status = test_show_fatigue_status,
    .show_fatigue_increased = test_show_fatigue_increased,
    .show_fatigue_recovered = test_show_fatigue_recovered,
    .show_passive_oxygen = test_show_passive_oxygen,
    .show_damage_dealt = test_show_damage_dealt,
    .show_attack_blocked = test_show_attack_blocked,
    .show_creature_defeated = test_show_creature_defeated,
    .show_actions_taken = test_show_actions_taken,
    .show_death_from_afflictions = test_show_death_from_afflictions,
    .show_death_from_suffocation = test_show_death_from_suffocation,
    .show_enemy_turn = test_show_enemy_turn,
    .show_your_turn = test_show_your_turn,
    .show_ending_turn = test_show_ending_turn,
    .show_action_effect = test_show_action_effect,
    .show_effect_error = test_show_effect_error,
    .show_action_on_cooldown = test_show_action_on_cooldown,
    .show_item_on_cooldown = test_show_item_on_cooldown,
    .show_no_actions_available = test_show_no_actions_available,
    .show_creature_died_from_effects = test_show_creature_died_from_effects,

    // Shop functions
    .display_shop = test_display_shop,
    .show_purchase_success = test_show_purchase_success,
    .show_purchase_failed = test_show_purchase_failed,
    .show_sell_success = test_show_sell_success,
    .show_sell_failed = test_show_sell_failed,
    .show_shop_refreshed = test_show_shop_refreshed,
    .show_refresh_failed = test_show_refresh_failed,
    .show_discount_applied = test_show_discount_applied,
    .show_shop_restocked = test_show_shop_restocked,
};
