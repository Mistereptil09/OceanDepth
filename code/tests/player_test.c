//
// Created by Yasmine Slamani on 03/10/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include "core/player.h"

int main(void) {
    printf("=== TEST PLAYER ===\n");

    // Create a player
    Player *p = create_player("Yasmine");
    if (!p) {
        fprintf(stderr, "Erreur: create_player a échoué.\n");
        return 1;
    }

    printf("Player créé: %s\n", p->name);
    printf("HP: %d/%d | ATK: %d | DEF: %d | O2: %d/%d | Pearls: %d | Fatigue: %d\n\n",
           p->stats.current_health_points, p->stats.max_health_points,
           p->stats.current_attack, p->stats.current_defense,
           p->oxygen_level, p->max_oxygen_level,
           p->pearls, p->fatigue_level);

    // --- TEST HP ---
    printf("[HP] Inflige 30 dégâts...\n");
    take_damage(p, 30);
    printf("HP: %d/%d\n", p->stats.current_health_points, p->stats.max_health_points);

    printf("[HP] Soigne 20 HP...\n");
    recover_hp(p, 20);
    printf("HP: %d/%d\n\n", p->stats.current_health_points, p->stats.max_health_points);

    // --- TEST OXYGEN ---
    printf("[O2] Consomme 30 oxygène...\n");
    consume_oxygen(p, 30);
    printf("O2: %d/%d\n", p->oxygen_level, p->max_oxygen_level);

    printf("[O2] Récupère 50 oxygène...\n");
    recover_oxygen(p, 50);
    printf("O2: %d/%d\n\n", p->oxygen_level, p->max_oxygen_level);

    // --- TEST FATIGUE ---
    printf("[Fatigue] Ajoute 3 fatigue...\n");
    increase_fatigue(p, 3);
    printf("Fatigue: %d\n", p->fatigue_level);

    printf("[Fatigue] Récupère 2 fatigue...\n");
    recover_fatigue(p, 2);
    printf("Fatigue: %d\n\n", p->fatigue_level);

    // --- TEST PEARLS ---
    printf("[Pearls] Ajoute 5 perles...\n");
    increase_pearls(p, 5);
    printf("Perles: %d\n", p->pearls);

    printf("[Pearls] Retire 20 perles...\n");
    decrease_pearls(p, 20);
    printf("Perles: %d\n\n", p->pearls);

    // Free function
    free_player(p);

    printf("=== TESTS TERMINÉS ===\n");
    return 0;
}
