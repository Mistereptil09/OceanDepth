#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "interface/interface_table.h"
#include "core/player.h"
#include "core/combat_loop.h"
#include "core/creature_data.h"
#include "core/save_system.h"
#include "core/error_codes.h"

int main(void)
{
    int seed = time(NULL);
    srand(seed);

    // Initialize creature templates
    init_creature_templates();

    // Select interface
    current_interface = select_interface();

    Player* player = NULL;
    int difficulty = EASY;
    int battles_won = 0;
    char choice[10];

    // Check if save file exists
    if (save_file_exists()) {
        printf("\n=== Fichier de sauvegarde detecte! ===\n");
        printf("Voulez-vous charger votre partie sauvegardee? (oui/non): ");
        current_interface->get_input("", choice, sizeof(choice));

        if (strcmp(choice, "oui") == 0 || strcmp(choice, "o") == 0) {
            if (load_game_modular(&player, &difficulty, &battles_won, SAVE_ALL_DATA) == SUCCESS) {
                printf("Partie chargee! Bienvenue, %s!\n", player->base.name);
                printf("Batailles gagnees: %d\n", battles_won);
                printf("Difficulte actuelle: %d\n", difficulty);
            } else {
                printf("Erreur lors du chargement. Nouvelle partie...\n");
                player = NULL;
            }
        }
    }

    // If no save loaded, create new player
    if (!player) {
        char player_name[30];
        current_interface->get_input("Donnez-moi votre nom !", player_name, sizeof(player_name));
        Position start_pos = {0, 0};
        player = create_player(player_name, 100, 8, 100, start_pos, start_pos);

        if (!player) {
            printf("Erreur: Impossible de creer le joueur!\n");
            cleanup_creature_templates();
            return 1;
        }

        printf("\n\nBienvenue, %s, dans les Profondeurs Oceaniques!\n", player_name);
        printf("Vous allez affronter les creatures des abysses...\n");
    }

    // Game loop with save option
    int continue_playing = 1;
    while (continue_playing) {
        // Start battle
        int result = battle_loop(player, difficulty, seed);

        if (result) {
            battles_won++;
            printf("\nFelicitations! Vous avez survecu aux profondeurs!\n");
            printf("Statistiques:\n");
            printf("  PV: %d/%d\n", player->base.current_health_points, player->base.max_health_points);
            printf("  Perles: %d\n", player->pearls);
            printf("  Batailles gagnees: %d\n", battles_won);

            // Ask to save
            printf("\nVoulez-vous sauvegarder votre progression? (oui/non): ");
            current_interface->get_input("", choice, sizeof(choice));
            if (strcmp(choice, "oui") == 0 || strcmp(choice, "o") == 0) {
                save_game_modular(player, difficulty, battles_won, SAVE_ALL_DATA);
            }

            // Ask to continue
            printf("\nContinuer a jouer? (oui/non): ");
            current_interface->get_input("", choice, sizeof(choice));
            if (strcmp(choice, "non") == 0 || strcmp(choice, "n") == 0) {
                continue_playing = 0;
            }
        } else {
            printf("\nL'ocean a reclame une autre ame...\n");
            printf("Partie terminee. Batailles gagnees: %d\n", battles_won);

            // Delete save on death
            if (save_file_exists()) {
                printf("Suppression de la sauvegarde...\n");
                delete_save_file();
            }
            continue_playing = 0;
        }
    }

    // Clean up
    free_player(player);
    cleanup_creature_templates();

    return 0;
}
