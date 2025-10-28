#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "interface/interface_table.h"
#include "core/player.h"
#include "core/combat_loop.h"
#include "core/creature_data.h"

int main(void)
{
    srand(time(NULL));

    // Initialize creature templates
    init_creature_templates();

    // Select interface
    current_interface = select_interface();

    // Create player
    char player_name[30];
    current_interface->get_input("Donnez-moi votre nom !", player_name, sizeof(player_name));
    Player* player = create_player(player_name, 100, 8
        , 100);

    if (!player) {
        printf("Erreur: Impossible de creer le joueur!\n");
        cleanup_creature_templates();
        return 1;
    }

    printf("\n\nBienvenue, %s, dans les Profondeurs Oceaniques!\n", player_name);
    printf("Vous allez affronter les creatures des abysses...\n");

    // Start battle with EASY difficulty
    int result = battle_loop(player, EASY);

    if (result) {
        printf("\nFelicitations! Vous avez survecu aux profondeurs!\n");
        printf("Statistiques finales:\n");
        printf("  PV: %d/%d\n", player->base.current_health_points, player->base.max_health_points);
        printf("  Perles: %d\n", player->pearls);
    } else {
        printf("\nL'ocean a reclame une autre ame...\n");
    }

    // Clean up
    free_player(player);
    cleanup_creature_templates();

    return 0;
}
