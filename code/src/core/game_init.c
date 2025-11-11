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
#include "../../includes/core/shop.h"
#include "../../includes/core/item_pool.h"
#include "../../includes/core/inventory.h"
#include "interface/interface_table.h"
#include "core/error_codes.h"

static time_t last_heal_time = 0;
#define HEAL_COOLDOWN_SECONDS (7 * 60)

static Difficulty get_difficulty_for_cell(CellType type) {
    switch (type) {
        case REEF:
            return EASY;
        case SHIPWRECK:
            return MEDIUM;
        case PIT:
            return HARD;
        case ABYSS:
            return FINAL;
        default:
            return EASY;
    }
}

static int handle_combat_cell(Player* player, Map* map, Difficulty diff, int* battles_won) {
    int battle_seed = map->seed +
                      (player->current_position.row * map->cols) +
                      player->current_position.col;

    int result = battle_loop(player, diff, battle_seed);

    if (result == 1) {
        (*battles_won)++;
        printf("\n🎉Victoire! Batailles gagnees: %d\n", *battles_won);

        int unlock_result = unlock_new_position(player);
        if (unlock_result == NEW_CELL) {
            printf("Nouvelle cellule débloquée!\n");
        } else if (unlock_result == NEW_ROW) {
            printf("Nouvelle profondeur débloquée!\n");
        } else if (unlock_result == WIN) {
            printf("\n🏆VICTOIRE! Vous avez atteint l'Abysse !\n");
            return WIN;
        }
        return SUCCESS;
    } else {
        printf("\n💀Defaite : La creature vous a vaincu.\n");
        return LOSS;
    }
}

static void handle_heal_cell(Player* player) {
    printf("\n=== CENTRE DE SOIN ===\n");

    // Check if player has any heals left
    if (player->heal_uses_left <= 0) {
        printf("⚠️  Vous avez epuise tous vos soins pour cette partie!\n");
        printf("Vous ne pouvez plus utiliser le centre de soin.\n");
        return;
    }

    time_t current_time = time(NULL);
    if (last_heal_time != 0) {
        double seconds_since_last = difftime(current_time, last_heal_time);
        if (seconds_since_last < HEAL_COOLDOWN_SECONDS) {
            int remaining_seconds = (int)(HEAL_COOLDOWN_SECONDS - seconds_since_last);
            int remaining_minutes = remaining_seconds / 60;
            int remaining_secs = remaining_seconds % 60;
            printf("Centre de soin en cooldown!\n");
            printf("Temps restant: %d minutes %d secondes\n", remaining_minutes, remaining_secs);
            return;
        }
    }

    printf("Vous recuperez 50%% de votre sante et votre oxygene!\n");
    printf("Soins restants: %d -> %d\n", player->heal_uses_left, player->heal_uses_left - 1);

    int hp_heal = player->base.max_health_points / 2;
    int oxygen_heal = player->base.max_oxygen_level / 2;

    player->base.current_health_points += hp_heal;
    if (player->base.current_health_points > player->base.max_health_points) {
        player->base.current_health_points = player->base.max_health_points;
    }

    player->base.oxygen_level += oxygen_heal;
    if (player->base.oxygen_level > player->base.max_oxygen_level) {
        player->base.oxygen_level = player->base.max_oxygen_level;
    }

    printf("HP: %d/%d\n", player->base.current_health_points, player->base.max_health_points);
    printf("O2: %d/%d\n", player->base.oxygen_level, player->base.max_oxygen_level);

    player->heal_uses_left--;
    last_heal_time = current_time;
}

static void handle_shop_cell(Player* player) {
    printf("\n=== BOUTIQUE ===\n");

    ItemPool pool = create_loot_pool();
    Shop shop = create_shop("Boutique des Profondeurs", &pool);

    int shopping = 1;
    while (shopping) {
        display_shop(&shop, player->pearls);

        printf("\n[Perles: %d] Options:\n", player->pearls);
        printf("1. Acheter un article\n");
        printf("2. Vendre un article\n");
        printf("3. Rafraichir l'inventaire (cout: %d perles)\n", shop.refresh_cost);
        printf("4. Quitter la boutique\n");

        int choice = current_interface->get_choice("Votre choix", 1, 4);

        switch (choice) {
            case 1: {
                if (shop.slot_count == 0) {
                    printf("La boutique est vide!\n");
                    break;
                }

                int item_choice = current_interface->get_choice(
                    "Choisissez un article a acheter (0 pour annuler)", 0, shop.slot_count);

                if (item_choice == 0) break;

                int slot_index = item_choice - 1;

                // Check space BEFORE purchasing
                if (player->inventory.count >= INVENTORY_SIZE) {
                    // Inventory is full - ask to replace or cancel
                    printf("\nVotre inventaire est plein. Remplacer un objet par: %s ?\n",
                           shop.slots[slot_index].item.name);
                    current_interface->show_inventory(&player->inventory);
                    printf("Choisissez un emplacement a remplacer, ou %d pour annuler.\n",
                           player->inventory.count + 1);

                    int replace_choice = current_interface->get_choice("Votre choix",
                                                                        1, player->inventory.count + 1);

                    if (replace_choice == player->inventory.count + 1) {
                        printf("Achat annule.\n");
                        break; // cancel purchase
                    }

                    // Remove item to make space then purchase
                    int replace_index = replace_choice - 1;
                    remove_item_by_index(&player->inventory, replace_index);
                }

                // now purchase and add to inventory
                if (shop_buy_item(&shop, slot_index, &player->pearls, NULL)) {
                    Item* bought_item = &shop.slots[slot_index].item;
                    int result = add_item_to_inventory(&player->inventory, bought_item);
                    if (result == SUCCESS) {
                        printf("Article ajoute a votre inventaire!\n");
                    } else {
                        printf("Erreur: Impossible d'ajouter l'article!\n");
                        // refund pearls
                        player->pearls += shop.slots[slot_index].current_price;
                        shop.slots[slot_index].stock++;
                    }
                }
                break;
            }

            case 2: {
                if (player->inventory.count == 0) {
                    printf("Votre inventaire est vide!\n");
                    break;
                }

                printf("\n=== Votre Inventaire ===\n");
                for (int i = 0; i < player->inventory.count; i++) {
                    Item* item = &player->inventory.items[i];
                    int sell_price = get_item_sell_price(item);
                    printf("%d. %s (prix de vente: %d perles)\n", i + 1, item->name, sell_price);
                }

                int sell_choice = current_interface->get_choice(
                    "Choisissez un article a vendre (0 pour annuler)", 0, player->inventory.count);

                if (sell_choice == 0) break;

                Item* item_to_sell = &player->inventory.items[sell_choice - 1];
                if (shop_sell_item(&shop, item_to_sell, &player->pearls)) {
                    remove_item_to_inventory(&player->inventory, item_to_sell);
                }
                break;
            }

            case 3: {
                shop_refresh_inventory(&shop, &player->pearls);
                break;
            }

            case 4: {
                printf("A bientot!\n");
                shopping = 0;
                break;
            }
        }
    }

    free_shop(&shop);
    free_item_pool(&pool);
}

static void handle_save_cell(Player* player, int difficulty, int battles_won, int map_seed) {
    printf("\n=== POINT DE SAUVEGARDE ===\n");
    printf("Sauvegarde en cours...\n");

    int result = save_game_complete(player, difficulty, battles_won, map_seed);
    if (result == SUCCESS) {
        printf("Partie sauvegardee !\n");
    } else {
        printf("Erreur lors de la sauvegarde.\n");
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
            // Load player
            if (load_game_modular(&player, &difficulty, &battles_won, SAVE_ALL_DATA) == SUCCESS) {
                // Get seed and regenerate
                if (get_map_seed_from_save(&map_seed) == SUCCESS) {
                    map = create_map(map_seed);
                    printf("Partie chargee! Bienvenue, %s!\n", player->base.name);
                    printf("Position: (%d,%d) | Batailles gagnees: %d\n",
                           player->current_position.row,
                           player->current_position.col,
                           battles_won);
                } else {
                    printf("Erreur lors du chargement du seed. Nouvelle partie.\n");
                    free_player(player);
                    player = NULL;
                }
            } else {
                printf("Erreur lors du chargement. Nouvelle partie.\n");
                player = NULL;
            }
        }
    }

    // If nothing loaded, create new game
    if (player == NULL) {
        printf("\n=== Nouvelle partie ===\n");
        printf("Nom du joueur ");
        char player_name[30];
        current_interface->get_input("", player_name, sizeof(player_name));

        Position start_pos = {1, 0};
        player = create_player(player_name, 150, 10, 150, start_pos, start_pos);

        if (!player) {
            fprintf(stderr, "Erreur: impossible de creer le joueur\n");
            cleanup_creature_templates();
            return MEMORY_ALLOCATION_ERROR;
        }

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

    // Main loop
    int game_running = 1;
    while (game_running) {
        printf("\n");
        current_interface->display_map(map, player);

        if (player->base.current_health_points <= 0) {
            printf("\n💀 GAME OVER - Vous etes mort!\n");
            delete_save_file();
            break;
        }

        Cell* current_cell = get_cell(map, player->current_position.row, player->current_position.col);
        if (!current_cell) {
            fprintf(stderr, "Erreur: cellule invalide\n");
            break;
        }

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

        printf("\nActions: [(e)nter] Interagir | [(m)ove] Deplacer | [(q)uit] Quitter\n> ");
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
            if (strcmp(choice, "up") == 0 || strcmp(choice, "u") == 0) {
                new_pos.row--;
                if (new_pos.row < 0) new_pos.row = map->rows - 1;
            }
            else if (strcmp(choice, "down") == 0 || strcmp(choice, "d") == 0) {
                new_pos.row++;
                if (new_pos.row >= map->rows) new_pos.row = 0;
            }
            else if (strcmp(choice, "left") == 0 || strcmp(choice, "l") == 0) {
                new_pos.col--;
                if (new_pos.col < 0) new_pos.col = map->cols - 1;
            }
            else if (strcmp(choice, "right") == 0 || strcmp(choice, "r") == 0) {
                new_pos.col++;
                if (new_pos.col >= map->cols) new_pos.col = 0;
            }
            else {
                printf("Direction invalide.\n");
                continue;
            }

            if (is_valid_move(map, player, new_pos)) {
                player_move_to(player, new_pos);
                printf("Deplacement reussi!\n");
            } else {
                printf("Deplacement invalide (case non debloquee ou hors limites).\n");
            }
        }
        else if (strcmp(choice, "enter") == 0 || strcmp(choice, "e") == 0) {
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
                    // Game over if player won final boss or lost a battle
                    if (cell_result == WIN || cell_result != SUCCESS) {
                        game_running = 0;
                        // Delete save file on loss
                        if (cell_result == LOSS) {
                            delete_save_file();
                        }
                    }
                    break;

                case CAVE:
                    printf("\n === GROTTE ===\n");

                    if (player->has_used_cave) {
                        printf("Vous avez deja utilise cette grotte.\n");
                        printf("Elle ne peut etre utilisee qu'une seule fois par partie.\n");
                    } else {
                        printf("Une grotte mystique! Elle restaure completement votre sante et oxygene!\n");
                        printf("Attention: Vous ne pourrez l'utiliser qu'une seule fois!\n");

                        // full heal
                        player->base.current_health_points = player->base.max_health_points;
                        player->base.oxygen_level = player->base.max_oxygen_level;
                        player->base.fatigue_level = 0;

                        printf("HP: %d/%d (100%% restaure!)\n", player->base.current_health_points, player->base.max_health_points);
                        printf("O2: %d/%d (100%% restaure!)\n", player->base.oxygen_level, player->base.max_oxygen_level);
                        printf("Fatigue: 0 (completement repose!)\n");

                        player->has_used_cave = 1;

                        int unlock_result = unlock_new_position(player);
                        if (unlock_result == NEW_CELL) {
                            printf("Nouvelle cellule débloquée!\n");
                        } else if (unlock_result == NEW_ROW) {
                            printf("Nouvelle profondeur débloquée!\n");
                        } else if (unlock_result == WIN) {
                            printf("\n🏆VICTOIRE! Vous avez atteint l'Abysse !\n");
                            cell_result = WIN;
                        }
                    }
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

    printf("\nMerci d'avoir joue!\n");
    free_map(map);
    free_player(player);
    cleanup_creature_templates();

    return SUCCESS;
}