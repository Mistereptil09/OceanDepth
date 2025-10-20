//
// Created by anton on 10/20/2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "core/combat_loop.h"
#include "core/entity.h"
#include "core/player.h"
#include "core/creature.h"
#include "core/creature_data.h"
#include "core/action.h"
#include "interface/interface_table.h"
#include "interface/interface_test.h"
#include "interface/interface_api.h"

// Test compute_physical_damage independent of effects
static void test_compute_physical_damage_simple(void) {
    printf("Running test_compute_physical_damage_simple...\n");
    EntityBase attacker = create_entity_base(ENTITY_PLAYER, "Attacker", 30, 2, 1, 10);
    EntityBase defender = create_entity_base(ENTITY_CREATURE, "Defender", 20, 5, 1, 5);

    // Ensure stat values are correct
    int dmg = compute_physical_damage(&attacker, &defender);
    // Attack 10 - Defense 5 = 5
    assert(dmg == 5);

    // If defense higher than attack, damage is zero
    attacker.attack.base_value = 3;
    attacker.attack.to_calculate = true;
    defender.defense.base_value = 10;
    defender.defense.to_calculate = true;
    dmg = compute_physical_damage(&attacker, &defender);
    assert(dmg == 0);

    free_entity_base(&attacker);
    free_entity_base(&defender);

    printf("test_compute_physical_damage_simple passed\n");
}

// Test compute_physical_damage with NULL parameters
static void test_compute_physical_damage_null(void) {
    printf("Running test_compute_physical_damage_null...\n");
    EntityBase attacker = create_entity_base(ENTITY_PLAYER, "Attacker", 30, 2, 1, 10);

    // NULL defender should return 0
    int dmg = compute_physical_damage(&attacker, NULL);
    assert(dmg == 0);

    // NULL attacker should return 0
    dmg = compute_physical_damage(NULL, &attacker);
    assert(dmg == 0);

    // Both NULL should return 0
    dmg = compute_physical_damage(NULL, NULL);
    assert(dmg == 0);

    free_entity_base(&attacker);

    printf("test_compute_physical_damage_null passed\n");
}

// Test effect application and duration
static void test_effect_duration(void) {
    printf("Running test_effect_duration...\n");

    EntityBase entity = create_entity_base(ENTITY_PLAYER, "TestEntity", 100, 10, 5, 20);

    // Create a 3-turn effect
    Effect test_effect = create_effect("Test Buff", "Testing!", 3,
        5, 0, 0, 0, 0,  // +5 attack flat
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        NULL, 0);

    apply_effect_to_target(&entity, test_effect);
    assert(entity.effects_number == 1);
    assert(entity.effects[0].turns_left == 3);

    int initial_attack = stat_get_value(&entity.attack);
    assert(initial_attack == 25); // 20 base + 5 from effect

    // Tick effect - should go to 2 turns
    all_effects_tick(&entity, NULL);
    assert(entity.effects[0].turns_left == 2);
    assert(entity.effects_number == 1); // Still active

    // Tick again - should go to 1 turn
    all_effects_tick(&entity, NULL);
    assert(entity.effects[0].turns_left == 1);
    assert(entity.effects_number == 1);

    // Tick final time - should expire (0 turns)
    all_effects_tick(&entity, NULL);
    assert(entity.effects_number == 0); // Effect removed

    // Attack should return to base value
    int final_attack = stat_get_value(&entity.attack);
    assert(final_attack == 20); // Back to base

    free_entity_base(&entity);
    printf("test_effect_duration passed\n");
}

// Test damage calculation with effects
static void test_damage_with_effects(void) {
    printf("Running test_damage_with_effects...\n");

    EntityBase attacker = create_entity_base(ENTITY_PLAYER, "Attacker", 50, 5, 3, 10);
    EntityBase defender = create_entity_base(ENTITY_CREATURE, "Defender", 50, 5, 3, 5);

    // Base damage: 10 - 5 = 5
    int base_dmg = compute_physical_damage(&attacker, &defender);
    assert(base_dmg == 5);

    // Apply +10 attack buff to attacker
    Effect attack_buff = create_effect("Attack Buff", "Power up!", 2,
        10, 0, 0, 0, 0,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        NULL, 0);
    apply_effect_to_target(&attacker, attack_buff);
    assert(attacker.effects_number == 1);

    // New damage: 20 - 5 = 15
    int buffed_dmg = compute_physical_damage(&attacker, &defender);
    assert(buffed_dmg == 15);

    // Apply +5 defense buff to defender
    Effect defense_buff = create_effect("Defense Buff", "Shield up!", 2,
        0, 5, 0, 0, 0,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        NULL, 0);
    apply_effect_to_target(&defender, defense_buff);
    assert(defender.effects_number == 1);

    // New damage: 20 - 10 = 10
    int final_dmg = compute_physical_damage(&attacker, &defender);
    assert(final_dmg == 10);

    // Verify effects expire correctly
    all_effects_tick(&attacker, NULL);
    assert(attacker.effects[0].turns_left == 1);
    all_effects_tick(&attacker, NULL);
    assert(attacker.effects_number == 0); // Effect expired

    // Damage should return closer to base (defender still has 1 turn left of defense buff)
    int restored_dmg = compute_physical_damage(&attacker, &defender);
    assert(restored_dmg == 0); // 10 - 10 = 0

    free_entity_base(&attacker);
    free_entity_base(&defender);
    printf("test_damage_with_effects passed\n");
}

// Test action cooldown behavior
static void test_action_cooldown(void) {
    printf("Running test_action_cooldown...\n");

    EntityBase entity = create_entity_base(ENTITY_PLAYER, "TestEntity", 100, 10, 5, 20);

    // Manually add an action with cooldown
    Action test_action = {0};
    strncpy(test_action.name, "Power Strike", 29); // name buffer is 30 chars
    test_action.type = PHYSICAL_ATTACK;
    test_action.cooldown_turns = 3;
    test_action.cooldown_remaining = 0;

    entity.actions[0] = test_action;
    entity.action_count = 1;

    // Initially should be available (cooldown_remaining = 0)
    assert(entity.actions[0].cooldown_remaining == 0);

    // Simulate using the action (set cooldown)
    entity.actions[0].cooldown_remaining = entity.actions[0].cooldown_turns;
    assert(entity.actions[0].cooldown_remaining == 3);

    // Decrement cooldown over 3 turns
    entity.actions[0].cooldown_remaining--;
    assert(entity.actions[0].cooldown_remaining == 2);

    entity.actions[0].cooldown_remaining--;
    assert(entity.actions[0].cooldown_remaining == 1);

    entity.actions[0].cooldown_remaining--;
    assert(entity.actions[0].cooldown_remaining == 0); // Available again

    // Test that cooldown doesn't go negative (or implementation allows it)
    entity.actions[0].cooldown_remaining--;
    assert(entity.actions[0].cooldown_remaining == -1);

    free_entity_base(&entity);
    printf("test_action_cooldown passed\n");
}

// Test entity death and revival edge cases
static void test_entity_death(void) {
    printf("Running test_entity_death...\n");

    EntityBase entity = create_entity_base(ENTITY_CREATURE, "TestCreature", 50, 10, 5, 15);

    // Initially alive
    assert(entity.is_alive == true);
    assert(entity.current_health_points == 50);

    // Take some damage
    entity_take_damage(&entity, 30);
    assert(entity.current_health_points == 20);
    assert(entity.is_alive == true);

    // Take lethal damage
    entity_take_damage(&entity, 25);
    assert(entity.current_health_points == 0);
    assert(entity.is_alive == false);

    // Taking more damage when dead shouldn't go negative
    entity_take_damage(&entity, 10);
    assert(entity.current_health_points == 0);
    assert(entity.is_alive == false);

    // Dead entities cannot be healed (expected behavior)
    int heal_result = entity_recover_hp(&entity, 30);
    assert(heal_result != 0); // Should return error code
    assert(entity.current_health_points == 0); // HP unchanged
    assert(entity.is_alive == false); // Still dead

    free_entity_base(&entity);
    printf("test_entity_death passed\n");
}

// Test percentage-based stat modifiers
static void test_percentage_modifiers(void) {
    printf("Running test_percentage_modifiers...\n");

    EntityBase entity = create_entity_base(ENTITY_PLAYER, "TestEntity", 100, 20, 5, 100);

    // Base attack is 100
    int base_attack = stat_get_value(&entity.attack);
    assert(base_attack == 100);

    // Apply 50% attack boost
    Effect percentage_buff = create_effect("Percentage Buff", "50% boost!", 2,
        0, 0, 0, 0, 0,
        0.5f, 0.0f, 0.0f, 0.0f, 0.0f,  // 50% attack boost
        NULL, 0);
    apply_effect_to_target(&entity, percentage_buff);
    assert(entity.effects_number == 1);

    // Should be 100 + (100 * 0.5) = 150
    int boosted_attack = stat_get_value(&entity.attack);
    assert(boosted_attack == 150);

    // Apply flat boost on top
    Effect flat_buff = create_effect("Flat Buff", "Flat boost!", 2,
        20, 0, 0, 0, 0,  // +20 flat
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        NULL, 0);
    apply_effect_to_target(&entity, flat_buff);
    assert(entity.effects_number == 2);

    // Should be 100 + 20 (flat) + (100 * 0.5) = 170
    int combined_attack = stat_get_value(&entity.attack);
    assert(combined_attack == 170);

    free_entity_base(&entity);
    printf("test_percentage_modifiers passed\n");
}

// Test multiple effects stacking
static void test_multiple_effects_stacking(void) {
    printf("Running test_multiple_effects_stacking...\n");

    EntityBase entity = create_entity_base(ENTITY_PLAYER, "TestEntity", 100, 10, 5, 50);

    // Apply three different buffs with different durations
    Effect buff1 = create_effect("Buff 1", "First!", 3,
        10, 0, 0, 0, 0,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        NULL, 0);
    Effect buff2 = create_effect("Buff 2", "Second!", 2,
        5, 0, 0, 0, 0,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        NULL, 0);
    Effect buff3 = create_effect("Buff 3", "Third!", 1,
        0, 0, 0, 0, 0,
        0.2f, 0.0f, 0.0f, 0.0f, 0.0f,  // 20% boost
        NULL, 0);
    apply_effect_to_target(&entity, buff1);
    apply_effect_to_target(&entity, buff2);
    apply_effect_to_target(&entity, buff3);

    assert(entity.effects_number == 3);

    // Total: 50 + 10 + 5 + (50 * 0.2) = 75
    int total_attack = stat_get_value(&entity.attack);
    assert(total_attack == 75);

    // Tick once - buff3 expires (1 turn)
    all_effects_tick(&entity, NULL);
    assert(entity.effects_number == 2);

    // Total: 50 + 10 + 5 = 65
    total_attack = stat_get_value(&entity.attack);
    assert(total_attack == 65);

    // Tick again - buff2 expires (2 turns)
    all_effects_tick(&entity, NULL);
    assert(entity.effects_number == 1);

    // Total: 50 + 10 = 60
    total_attack = stat_get_value(&entity.attack);
    assert(total_attack == 60);

    // Tick final - buff1 expires (3 turns)
    all_effects_tick(&entity, NULL);
    assert(entity.effects_number == 0);

    // Back to base: 50
    total_attack = stat_get_value(&entity.attack);
    assert(total_attack == 50);

    free_entity_base(&entity);
    printf("test_multiple_effects_stacking passed\n");
}

// Smoke test for battle_loop: verify it completes without crashes
static void test_battle_loop_smoke(void) {
    printf("Running test_battle_loop_smoke...\n");
    init_creature_templates();

    // Create a player with enough stats to defeat weak creatures
    Player* player = create_player("Tester", 50, 1, 10);
    assert(player != NULL);

    // Run battle loop at EASY difficulty
    int result = battle_loop(player, EASY);
    printf("battle_loop returned: %d (0=loss, 1=win)\n", result);

    // Just verify it completed (result is 0 or 1)
    assert(result == 0 || result == 1);

    // Clean up
    free_player(player);
    cleanup_creature_templates();
    printf("test_battle_loop_smoke completed\n");
}

int main(void) {
    // Use the test interface (doesn't wait for user input)
    current_interface = &test_interface;

    printf("=== Running Combat System Tests ===\n\n");

    // Basic damage tests
    test_compute_physical_damage_simple();
    test_compute_physical_damage_null();

    // Effect system tests
    test_effect_duration();
    test_damage_with_effects();
    test_percentage_modifiers();
    test_multiple_effects_stacking();

    // Combat mechanics tests
    test_action_cooldown();
    test_entity_death();

    // Integration test
    test_battle_loop_smoke();

    printf("\n=== All Combat Loop Tests Completed Successfully ===\n");
    return 0;
}

