#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "core/creature.h"

#include <string.h>

#include "core/creature_data.h"
#include "core/entity.h"

/** gcc -fsanitize=address -fsanitize=undefined -g -Wall -Wextra -I./code/includes -o creature_test code/tests/creature_test.c code/src/core/creature.c
 * code/src/core/creature_data.c code/src/core/effect.c code/src/core/entity.c*/

void display_creature(const Creature* c) {
    printf("  ID: %d, Type: %d\n", c->id, c->type);
    printf("  HP: %d/%d, DEF: %d, SPD: %d, Alive: %s\n", 
           c->base.current_health_points, c->base.max_health_points,
           stat_get_value((Stat*)&c->base.defense), stat_get_value((Stat*)&c->base.speed),
           c->base.is_alive ? "YES" : "NO");
    printf("  Actions: '%s', '%s'\n", 
           c->base.actions[0].name, c->base.actions[1].name);
}

int main(void) {
    srand(time(NULL));
    init_creature_templates();
    
    printf("=== CREATURE TESTS ===\n");
    
    // Test 1: Solo creature creation (low-level)
    printf("\n1. Solo creature creation:\n");
    EntityBase base = create_entity_base(ENTITY_CREATURE, "Test Shark", 50, 10, 2, 15);
    Creature* shark = create_creature(1, SHARK, base, NULL);
    display_creature(shark);
    
    // Test 2: Create from template
    printf("\n2. Create from template:\n");
    Creature* medium_creature = create_from_template(CREATURE_MEDIUM, 2);
    printf("Generated medium creature:\n");
    display_creature(medium_creature);
    
    // Test 3: Generate creature sets
    printf("\n3. Generate MEDIUM difficulty set:\n");
    int medium_count;
    Creature** medium_set = generate_creatures(MEDIUM, &medium_count);
    printf("Generated %d creatures:\n", medium_count);
    for (int i = 0; i < medium_count; i++) {
        printf("Creature %d:\n", i);
        display_creature(medium_set[i]);
        printf("\n");
    }
    
    printf("4. Generate FINAL difficulty set:\n");
    int final_count;
    Creature** final_set = generate_creatures(FINAL, &final_count);
    printf("Generated %d creatures:\n", final_count);
    for (int i = 0; i < final_count; i++) {
        printf("Creature %d:\n", i);
        display_creature(final_set[i]);
        printf("\n");
    }
    
    // Test 4: Damage and recovery
    printf("5. Testing damage/recovery on first medium creature:\n");
    Creature* test_creature = medium_set[0];
    printf("Before damage: HP = %d/%d\n", 
           test_creature->base.current_health_points, test_creature->base.max_health_points);
    
    entity_take_damage(&test_creature->base, 20);
    printf("After 30 damage: HP = %d/%d, Alive = %s\n", 
           test_creature->base.current_health_points, test_creature->base.max_health_points,
           test_creature->base.is_alive ? "YES" : "NO");
    
    entity_recover_hp(&test_creature->base, 15);
    printf("After 15 recovery: HP = %d/%d, Alive = %s\n",
           test_creature->base.current_health_points, test_creature->base.max_health_points,test_creature->base.is_alive ? "YES" : "NO" );
    
    // Test lethal damage
    entity_take_damage(&test_creature->base, 200);
    printf("After lethal damage of 200: HP = %d/%d, Alive = %s\n",
           test_creature->base.current_health_points, test_creature->base.max_health_points,
           test_creature->base.is_alive ? "YES" : "NO");

    printf("\n7. Testing select_action function:\n");

       Creature* test1 = create_from_template(CREATURE_MEDIUM, 20);


       printf("Test creature 1 actions: %s, %s\n",
              test1->base.actions[0].name, test1->base.actions[1].name);

       int ac1_count = 0, ac2_count = 0;
       for (int i = 0; i < 100; i++) {
              Action* action = select_action(test1);
              if (strcmp(action->name, test1->base.actions[0].name) == 0) ac1_count++;
              else ac2_count++;
       }
       printf("100 tests - action 1: %d, action 2: %d\n", ac1_count, ac2_count);

       // Cleanup
       free_creature(test1);

    // Test 5: Free everything
    printf("\n6. Freeing all creatures:\n");
    free_creature(shark);
    free_creature(medium_creature);
    free_generated_creatures(medium_set, medium_count);
    free_generated_creatures(final_set, final_count);
    
    cleanup_creature_templates();
    printf("All creatures and templates freed.\n");
    
    return 0;
}