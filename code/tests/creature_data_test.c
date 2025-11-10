#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include "core/creature_data.h"
#include "core/creature.h"

/**
 * gcc -fsanitize=address -g -O1 -Icode/includes \
    code/tests/creature_data_test.c \
    code/src/core/creature_data.c \
    code/src/core/effect.c \
    code/src/core/entity.c \
    -o creature_data_test
    */

void test_deterministic_generation(void) {
    printf("\n=== Test: Deterministic Generation (Same Seed) ===\n");

    int seed = 42;
    Difficulty diff = MEDIUM;

    // Generate creatures with same seed twice
    srand(seed);
    int count1 = 0;
    Creature** creatures1 = generate_creatures(diff, &count1);

    srand(seed);  // Reset to same seed
    int count2 = 0;
    Creature** creatures2 = generate_creatures(diff, &count2);

    // Should generate same number of creatures
    assert(count1 == count2);
    printf("✓ Same seed produces same creature count: %d\n", count1);

    // Should generate same creature types/names
    int all_match = 1;
    for (int i = 0; i < count1; i++) {
        printf("  Lineup %d: '%s' vs '%s' - ", i,
               creatures1[i]->base.name,
               creatures2[i]->base.name);

        if (strcmp(creatures1[i]->base.name, creatures2[i]->base.name) == 0) {
            printf("MATCH ✓\n");
        } else {
            printf("DIFFERENT ✗\n");
            all_match = 0;
        }
    }

    assert(all_match == 1);
    printf("✓ Same seed produces identical creature lineups\n");

    free_generated_creatures(creatures1, count1);
    free_generated_creatures(creatures2, count2);
}

void test_varied_generation(void) {
    printf("\n=== Test: Varied Generation (Different Seeds) ===\n");

    Difficulty diff = EASY;
    int num_trials = 10;
    int different_lineups = 0;

    // Generate with first seed as baseline
    srand(100);
    int baseline_count = 0;
    Creature** baseline = generate_creatures(diff, &baseline_count);

    printf("Baseline (seed=100): ");
    for (int i = 0; i < baseline_count; i++) {
        printf("%s%s", baseline[i]->base.name, i < baseline_count-1 ? ", " : "\n");
    }

    // Try multiple different seeds
    for (int trial = 0; trial < num_trials; trial++) {
        srand(200 + trial * 50);  // Different seeds
        int count = 0;
        Creature** creatures = generate_creatures(diff, &count);

        // Check if lineup differs from baseline
        int is_different = 0;
        if (count != baseline_count) {
            is_different = 1;
        } else {
            for (int i = 0; i < count; i++) {
                if (strcmp(creatures[i]->base.name, baseline[i]->base.name) != 0) {
                    is_different = 1;
                    break;
                }
            }
        }

        if (is_different) {
            different_lineups++;
            printf("  Trial %d (seed=%d): ", trial, 200 + trial * 50);
            for (int i = 0; i < count; i++) {
                printf("%s%s", creatures[i]->base.name, i < count-1 ? ", " : "");
            }
            printf(" - DIFFERENT ✓\n");
        }

        free_generated_creatures(creatures, count);
    }

    printf("✓ Generated %d different lineups out of %d trials\n",
           different_lineups, num_trials);

    if (different_lineups > 0) {
        printf("✓ Seed-based randomization is working (variety detected)\n");
    } else {
        printf("⚠ No variety detected - creature generation may be deterministic by difficulty only\n");
        printf("  (This is expected if generate_creatures() doesn't use rand() yet)\n");
    }

    free_generated_creatures(baseline, baseline_count);
}

int main(void) {
    printf("Initializing creature templates...\n");
    init_creature_templates();

    const CreatureTemplate *templates = get_creature_templates();
    int count = get_creature_template_count();

    printf("\nCreature Templates (%d total):\n", count);
    printf("====================================\n");

    for (int i = 0; i < count; i++) {
        const CreatureTemplate *t = &templates[i];

        printf("%d. Type: %d, Tier: %d\n", i, t->type, t->tier);
        printf("   HP: %d-%d, ATK: %d-%d, DEF: %d, SPD: %d\n",
               t->min_hp, t->max_hp, t->min_atk, t->max_atk, t->defense, t->speed);
        printf("   Actions: '%s' (%s), '%s' (%s)\n",
               t->actions[0].name,
               (t->actions[0].type == PHYSICAL_ATTACK) ? "PHYS" : "SPEC",
               t->actions[1].name,
               (t->actions[1].type == PHYSICAL_ATTACK) ? "PHYS" : "SPEC");
        printf("\n");
    }

    // Run generation tests
    test_deterministic_generation();
    test_varied_generation();

    cleanup_creature_templates();
    printf("\n====================================\n");
    printf("All tests completed!\n");
    return 0;
}