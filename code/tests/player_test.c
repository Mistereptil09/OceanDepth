//
// Created by Yasmine Slamani on 03/10/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include "core/player.h"

/**
*   gcc -fsanitize=address -g -O1 -Icode/includes \
    code/tests/player_test.c \
    code/src/core/player.c \
    code/src/core/entity.c \
    code/src/core/inventory.c \
    code/src/core/effect.c \
    -o player_test
 */

int main(void) {
    printf("=== TEST PLAYER ===\n");

    printf("CREATE PLAYER\n");
    Player *p = create_player("Yasmine", 100, 10, 100, 0, 0, 0);
    if (!p) {
        fprintf(stderr, "Erreur: create_player a echoue.\n");
        return 1;
    }

    // Player created with entity base
    printf("CREATED PLAYER: %s\n", p->base.name);
    printf("HP: %d/%d | ATK: %d | DEF: %d | O2: %d/%d | Pearls: %d | Fatigue: %d\n\n",
           p->base.current_health_points, p->base.max_health_points,
           stat_get_value(&p->base.attack), stat_get_value(&p->base.defense),
           p->base.oxygen_level, p->base.max_oxygen_level,
           p->pearls, p->base.fatigue_level);

    // --- TEST HP ---
    printf("Appliquer 30 de degats : \n");
    entity_take_damage(&p->base, 30);
    printf("HP: %d/%d\n", p->base.current_health_points, p->base.max_health_points);

    printf("[HP] Soigne 20 HP : \n");
    entity_recover_hp(&p->base, 20);
    printf("HP: %d/%d\n\n", p->base.current_health_points, p->base.max_health_points);

    // --- TEST OXYGEN ---
    printf("[O2] Consomme 30 oxygene :\n");
    consume_oxygen(p, 30);
    printf("O2: %d/%d\n", p->base.oxygen_level, p->base.max_oxygen_level);

    printf("[O2] Recupere 50 oxygene :\n");
    recover_oxygen(p, 50);
    printf("O2: %d/%d\n\n", p->base.oxygen_level, p->base.max_oxygen_level);

    // --- TEST FATIGUE ---
    printf("[Fatigue] Ajoute 3 fatigue :\n");
    increase_fatigue(p, 3);
    printf("Fatigue: %d\n", p->base.fatigue_level);

    printf("[Fatigue] Recupere 2 fatigue :\n");
    recover_fatigue(p, 2);
    printf("Fatigue: %d\n\n", p->base.fatigue_level);

    // --- TEST PEARLS ---
    printf("[Pearls] Ajoute 5 perles :\n");
    increase_pearls(p, 5);
    printf("Perles: %d\n", p->pearls);

    printf("[Pearls] Retire 20 perles :\n");
    decrease_pearls(p, 20);
    printf("Perles: %d\n\n", p->pearls);

    // FREE
    free_player(p);

    printf("=== TESTS TERMINES ===\n");
    return 0;
}
