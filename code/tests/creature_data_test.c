#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "core/creature_data.h"

/**
 * gcc -fsanitize=address -g -O1 -Icode/includes \
    code/tests/creature_data_test.c \
    code/src/core/creature_data.c \
    code/src/core/effect.c \
    code/src/core/entity.c \
    -o creature_data_test
    */
int main(void) {
    srand(time(NULL));

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

    cleanup_creature_templates();
    printf("Templates cleaned up.\n");
    return 0;
}