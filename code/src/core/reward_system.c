//
// Created by anton on 11/2/2025.
#include "interface/interface_table.h"
#include "interface/interface_api.h"
#include "core/item_pool.h"
#include "core/inventory.h"
#include "core/error_codes.h"
#include "core/reward_system.h"
#include "core/player.h"
#include <stdio.h>   // snprintf
#include <stdlib.h>  // calloc, free
#ifdef _WIN32
#include <malloc.h>
#endif


// decide how many pearls to give out based on difficulty
static int get_pearl_reward(Difficulty d) {
    switch (d) {
        case EASY: return 5 + (rand() % 6);      // 5-10 pearls
        case MEDIUM: return 10 + (rand() % 11);  // 10-20 pearls
        case HARD: return 20 + (rand() % 11);    // 20-30 pearls
        case FINAL: return 40 + (rand() % 11);   // 40-50 pearls
        default: return 10;
    }
}

// decide how many loot options to draw based on difficulty
static int get_reward_draw_count(Difficulty d) {
    switch (d) {
        case EASY: return 2;
        case MEDIUM: return 3;
        case HARD: return 3;
        case FINAL: return 4;
        default: return 3;
    }
}

// when inventory is full ask the player which slot to replace or cancel
static int reward_inventory_replace_callback(Inventory* inv, Item* new_item) {
    if (!inv) return -1;

    // show current inventory
    if (current_interface && current_interface->show_inventory) {
        current_interface->show_inventory(inv);
    }

    // show replacement prompt
    if (current_interface && current_interface->show_inventory_replacement_prompt) {
        current_interface->show_inventory_replacement_prompt((new_item && new_item->name[0]) ? new_item->name : "(inconnu)");
    }

    int choice = 0;
    if (current_interface && current_interface->get_choice) {
        choice = current_interface->get_choice("Votre choix", 1, inv->count + 1);
    } else {
        choice = inv->count + 1; // default cancel when no input available
    }

    if (choice == inv->count + 1) {
        if (current_interface && current_interface->show_inventory_full) {
            current_interface->show_inventory_full();
        }
        return -1; // cancel
    }

    return choice - 1;
}

//dDraw N loot options from the loot pool
static void draw_loot_options(Item* out_items, int count) {
    ItemPool pool = create_loot_pool();
    for (int i = 0; i < count; i++) {
        out_items[i] = draw_random_item(&pool);
    }
    free_item_pool(&pool);
}

// present rewards and return chosen index
static int choose_reward_index(Item* options, int count) {
    if (!options || count <= 0) return -1;

    if (current_interface && current_interface->ask_item_choice_reward) {
        Item* chosen_ptr = current_interface->ask_item_choice_reward(count, options);
        if (!chosen_ptr) return -1;
        return (int)(chosen_ptr - options);
    }

    // fallback
    if (current_interface && current_interface->show_information) {
        current_interface->show_information("\n=== Choisissez une recompense ===");
        char line[96];
        for (int i = 0; i < count; i++) {
            snprintf(line, sizeof(line), "%d. %s", i + 1, options[i].name);
            current_interface->show_information(line);
        }
    }
    int idx = 1;
    if (current_interface && current_interface->get_choice) {
        idx = current_interface->get_choice("Votre choix", 1, count);
    }
    return idx - 1;
}

// add selected reward to inventory handling full inventory
static int grant_reward_to_player(Player* player, Item* reward) {
    if (!player || !reward) return POINTER_NULL;

    int res = add_item_to_inventory_with_callback(&player->inventory, reward, reward_inventory_replace_callback);
    if (res == SUCCESS) {
        if (current_interface && current_interface->show_reward_obtained) {
            current_interface->show_reward_obtained(reward->name);
        }
    } else if (res == INVENTORY_FULL) {
        // if still full and user cancelled replacement
        if (current_interface && current_interface->show_inventory_full) {
            current_interface->show_inventory_full();
        }
    }
    return res;
}

// free all unselected items
static void free_unselected_items(Item* options, int count, int keep_index) {
    for (int i = 0; i < count; i++) {
        if (i != keep_index) {
            free_item(&options[i]);
        }
    }
}

// orchestrates the reward flow at end of battle
void award_post_battle_rewards(Player* player, Difficulty difficulty) {
    if (!player) return;

    // pearls first
    int pearl_reward = get_pearl_reward(difficulty);
    increase_pearls(player, pearl_reward);
    if (current_interface && current_interface->show_pearl_reward) {
        current_interface->show_pearl_reward(pearl_reward, player->pearls);
    }

    int draw_count = get_reward_draw_count(difficulty);
    if (draw_count <= 0) return;

    Item* options = calloc((size_t)draw_count, sizeof(Item));
    if (!options) return;

    // draw loot
    draw_loot_options(options, draw_count);

    // present and choose
    int chosen = choose_reward_index(options, draw_count);
    if (chosen < 0 || chosen >= draw_count) {
        // invalid choice: free all and exit
        for (int i = 0; i < draw_count; i++) free_item(&options[i]);
        free(options);
        return;
    }

    // grant and cleanup others
    int grant_res = grant_reward_to_player(player, &options[chosen]);
    free_unselected_items(options, draw_count, chosen);

    // free the chosen only if not granted
    if (grant_res != SUCCESS) {
        free_item(&options[chosen]);
    }

    // chosen item memory is now owned by the inventory
    free(options);
}
