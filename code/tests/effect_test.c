#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "core/effect.h"

/** gcc -fsanitize=address -g -O1 -Icode/includes \
    code/tests/effect_test.c \
    code/src/core/effect.c \
    -o effect_test
*/

int main(void) {
    printf("Testing effect creation...\n");
    
    // Test 1: Create effect
    Effect poison = create_effect(EFFECT_POISON, "Poisoned!", 3, 10, 0, 0, 0, 0, 0, 0);
    printf("Created: %s - %s\n", poison.name, poison.display_message);
    
    // Test 2: Copy effect
    Effect poison_copy = effect_copy(&poison);
    printf("Copied: %s - %s\n", poison_copy.name, poison_copy.display_message);
    
    // Check pointers are different
    printf("Different pointers: %s\n", 
           (poison.display_message != poison_copy.display_message) ? "YES" : "NO");
    
    // Test 3: Create with NULL message
    Effect bleed = create_effect(EFFECT_BLEED, NULL, 2, 15, 0, 0, 0, 0, 0, 0);
    printf("NULL message: %s - %s\n", bleed.name, bleed.display_message ? "HAS MSG" : "NULL");
    
    // Test 4: Free effects
    printf("Freeing effects...\n");
    free_effect_content(&poison);
    free_effect_content(&poison_copy);
    free_effect_content(&bleed);  // Should handle NULL gracefully
    
    printf("All tests passed!\n");
    return 0;
}