//
// Created by Yasmine Slamani on 07/11/2025.
//

#include "../../includes/core/game_init.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "../../includes/core/combat_loop.h"
#include "../../includes/core/creature_data.h"
#include "../../includes/core/save_system.h"
#include "../../includes/core/map.h"
#include "../../includes/core/player.h"
#include "interface/interface_table.h"
#include "core/error_codes.h"

static Difficulty get_difficulty_for_cell(CellType type) {
    switch (type) {
        case REEF:
            return EASY;
        case SHIPWRECK:
            return MEDIUM;
        case PIT:
        case ABYSS:
            return HARD;
        default:
            return EASY;  // Default fallback
    }
}

static int handle_combat_cell(Player* player, Map* map, Difficulty diff, int* battles_won) {
    // Calculate battle seed: map_seed + (row * cols) + col
    int battle_seed = map->seed +
                      (player->current_position.row * map->cols) +
                      player->current_position.col;

    // Run battle (returns 1 for player win, 0 for player loss)
    int result = battle_loop(player, diff, battle_seed);

    if (result == 1) {  // Player won
        (*battles_won)++;
        printf("\n🎉 Victoire! Batailles gagnees: %d\n", *battles_won);

        // Unlock next cell
        int unlock_result = unlock_new_position(player);
        if (unlock_result == NEW_CELL) {
            printf("✓ Nouvelle cellule débloquée!\n");
        } else if (unlock_result == NEW_ROW) {
            printf("✓ Nouvelle profondeur débloquée!\n");
        } else if (unlock_result == WIN) {
            printf("\n🏆 VICTOIRE! Vous avez atteint l'Abysse et survécu!\n");
            return WIN;
        }
        return SUCCESS;
    } else {  // Player lost (result == 0)
        printf("\n💀 Defaite... La creature vous a vaincu.\n");
        return LOSS;  // Use as game over flag
    }
}

static void handle_heal_cell(Player* player) {
    printf("\n❤️  === CENTRE DE SOIN ===\n");
    printf("Vous recuperez toute votre sante et votre oxygene!\n");
    player->base.current_health_points = player->base.max_health_points;
    player->base.oxygen_level = player->base.max_oxygen_level;
    printf("HP: %d/%d\n", player->base.current_health_points, player->base.max_health_points);
    printf("O2: %d/%d\n", player->base.oxygen_level, player->base.max_oxygen_level);
}

static void handle_shop_cell(Player* player) {
    printf("(Boutique non implementee - a venir!)\n");
    // TODO: Implement shop interface
}

static void handle_save_cell(Player* player, int difficulty, int battles_won, int map_seed) {
    printf("\n💾 === POINT DE SAUVEGARDE ===\n");
    printf("Sauvegarde en cours...\n");

    int result = save_game_complete(player, difficulty, battles_won, map_seed);
    if (result == SUCCESS) {
        printf("✓ Partie sauvegardee avec succes!\n");
    } else {
        printf("✗ Erreur lors de la sauvegarde.\n");
    }
}

int game_init(void) {
    // Initialize creature templates
    init_creature_templates();

    // Interface choice
    current_interface = select_interface();

    Player* player = NULL;
    Map* map = NULL;
    int battles_won = 0;
    int difficulty = EASY;
    int map_seed = 0;
    char choice[10];

    // Configuration choice: load from saved file OR generate new data
    if (save_file_exists()) {
        printf("\n=== Fichier de sauvegarde detecte! ===\n");
        printf("Voulez-vous charger votre partie sauvegardee? (oui/non): ");
        current_interface->get_input("", choice, sizeof(choice));

        if (strcmp(choice, "oui") == 0 || strcmp(choice, "o") == 0) {
            // Load player and progression
            if (load_game_modular(&player, &difficulty, &battles_won, SAVE_ALL_DATA) == SUCCESS) {
                // Get map seed and regenerate map
                if (get_map_seed_from_save(&map_seed) == SUCCESS) {
                    map = create_map(map_seed);
                    printf("Partie chargee! Bienvenue, %s!\n", player->base.name);
                    printf("Position: (%d,%d) | Batailles gagnees: %d\n",
                           player->current_position.row,
                           player->current_position.col,
                           battles_won);
                } else {
                    printf("Erreur lors du chargement du seed. Nouvelle partie...\n");
                    free_player(player);
                    player = NULL;
                }
            } else {
                printf("Erreur lors du chargement. Nouvelle partie...\n");
                player = NULL;
            }
        }
    }

    // If no save loaded, create new game
    if (player == NULL) {
        printf("\n=== Nouvelle partie ===\n");
        printf("Nom du joueur: ");
        char player_name[30];
        current_interface->get_input("", player_name, sizeof(player_name));

        // Create new player at starting position
        Position start_pos = {1, 0};
        player = create_player(player_name, 100, 10, 100, start_pos, start_pos);

        if (!player) {
            fprintf(stderr, "Erreur: impossible de creer le joueur\n");
            cleanup_creature_templates();
            return MEMORY_ALLOCATION_ERROR;
        }

        // Generate new map with time-based seed
        map_seed = (int)time(NULL);
        map = create_map(map_seed);

        if (!map) {
            fprintf(stderr, "Erreur: impossible de creer la carte\n");
            free_player(player);
            cleanup_creature_templates();
            return MEMORY_ALLOCATION_ERROR;
        }

        printf("\nBienvenue dans OceanDepth, %s!\n", player->base.name);
        printf("Nouveau seed: %d\n", map_seed);
    }

    // Main game loop
    int game_running = 1;
    while (game_running) {
        // Display map
        printf("\n");
        current_interface->display_map(map, player);

        // Check if player is dead
        if (player->base.current_health_points <= 0) {
            printf("\n💀 GAME OVER - Vous etes mort!\n");
            break;
        }

        // Get current cell
        Cell* current_cell = get_cell(map, player->current_position.row, player->current_position.col);
        if (!current_cell) {
            fprintf(stderr, "Erreur: cellule invalide\n");
            break;
        }

        // Show options
        printf("\n[Position: %d,%d] Type: ", player->current_position.row, player->current_position.col);
        switch (current_cell->type) {
            case SHOP: printf("BOUTIQUE\n"); break;
            case HEAL: printf("SOIN\n"); break;
            case SAVE: printf("SAUVEGARDE\n"); break;
            case EMPTY: printf("VIDE\n"); break;
            case REEF: printf("RECIF\n"); break;
            case CAVE: printf("GROTTE\n"); break;
            case SHIPWRECK: printf("EPAVE\n"); break;
            case PIT: printf("FOSSE\n"); break;
            case ABYSS: printf("ABYSSE\n"); break;
        }

        printf("\nActions: [enter] Interagir | [move] Deplacer | [quit] Quitter\n> ");
        current_interface->get_input("", choice, sizeof(choice));

        if (strcmp(choice, "quit") == 0 || strcmp(choice, "q") == 0) {
            printf("Sauvegarde avant de quitter...\n");
            save_game_complete(player, difficulty, battles_won, map_seed);
            game_running = 0;
        }
        else if (strcmp(choice, "move") == 0 || strcmp(choice, "m") == 0) {
            printf("Direction [up/down/left/right]: ");
            current_interface->get_input("", choice, sizeof(choice));

            Position new_pos = player->current_position;
            if (strcmp(choice, "up") == 0 || strcmp(choice, "u") == 0) new_pos.row--;
            else if (strcmp(choice, "down") == 0 || strcmp(choice, "d") == 0) new_pos.row++;
            else if (strcmp(choice, "left") == 0 || strcmp(choice, "l") == 0) new_pos.col--;
            else if (strcmp(choice, "right") == 0 || strcmp(choice, "r") == 0) new_pos.col++;
            else {
                printf("Direction invalide.\n");
                continue;
            }

            if (is_valid_move(map, player, new_pos)) {
                player_move_to(player, new_pos);
                printf("✓ Deplacement reussi!\n");
            } else {
                printf("✗ Deplacement invalide (case non debloquee ou hors limites).\n");
            }
        }
        else if (strcmp(choice, "enter") == 0 || strcmp(choice, "") == 0) {
            // Handle cell interaction
            int cell_result = SUCCESS;
            switch (current_cell->type) {
                case SHOP:
                    handle_shop_cell(player);
                    break;

                case HEAL:
                    handle_heal_cell(player);
                    break;

                case SAVE:
                    handle_save_cell(player, difficulty, battles_won, map_seed);
                    break;

                case REEF:
                case SHIPWRECK:
                case PIT:
                case ABYSS:
                    cell_result = handle_combat_cell(player, map, get_difficulty_for_cell(current_cell->type), &battles_won);
                    // End game if player won final battle or lost any battle
                    if (cell_result == WIN || cell_result != SUCCESS) {
                        game_running = 0;
                    }
                    break;

                case CAVE:
                    printf("\n🏖️  === GROTTE ===\n");
                    printf("Un endroit paisible pour se reposer...\n");
                    printf("(Fonctionnalite a venir)\n");
                    break;

                case EMPTY:
                    printf("Rien d'interessant ici.\n");
                    break;
            }
        }
        else {
            printf("Commande non reconnue.\n");
        }
    }

    // Cleanup
    printf("\nMerci d'avoir joue!\n");
    free_map(map);
    free_player(player);
    cleanup_creature_templates();

    return SUCCESS;
}