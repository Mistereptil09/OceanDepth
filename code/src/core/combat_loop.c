#include "core/combat_loop.h"
#include "core/effect.h"
#include "core/entity.h"
#include "core/action.h"
#include "core/creature.h"
#include "core/player.h"
#include "core/item.h"
#include "interface/interface_table.h"
#include "interface/interface_api.h"
#include <stdio.h>

int compute_physical_damage(EntityBase *attacker, EntityBase *defender) {
    if (!attacker || !defender) return 0;

    // Enhanced debug output showing before and after values
    printf("[DEBUG] === COMPUTING DAMAGE ===\n");
    printf("[DEBUG] Attacker: %s, Defender: %s\n", attacker->name, defender->name);
    printf("[DEBUG] ATTACKER'S ATTACK BASE VALUE : %d\n", attacker->attack.base_value);
    printf("[DEBUG] ATTACKER'S ATTACK CACHED VALUE BEFORE RECALC : %d\n", attacker->attack.cached_value);
    printf("[DEBUG] DEFENDER'S DEFENSE BASE VALUE : %d\n", defender->defense.base_value);
    printf("[DEBUG] DEFENDER'S DEFENSE CACHED VALUE BEFORE RECALC : %d\n", defender->defense.cached_value);

    // Force recalculation
    attacker->attack.to_calculate = true;
    defender->defense.to_calculate = true;

    int atk = stat_get_value(&attacker->attack);
    int def = stat_get_value(&defender->defense);

    printf("[DEBUG] %s'S ATTACK CACHED VALUE AFTER RECALC : %d\n", attacker->name, attacker->attack.cached_value);
    printf("[DEBUG] %s'S DEFENSE CACHED VALUE AFTER RECALC : %d\n", defender->name, defender->defense.cached_value);

    int raw = atk - def;
    if (raw < 0) raw = 0;

    printf("COMPUTE_PHYSICAL_DAMAGE : ATK %d - DEF %d = RAW %d\n", atk, def, raw);
    printf("[DEBUG] === END OF DAMAGE COMPUTE ===\n\n");

    return raw;
}

int Attack(EntityBase *attacker, Action *action, EntityBase *defender) {
    if (!attacker || !defender || !action) return 0;

    // Show action
    current_interface->show_action(attacker->name, action->name);

    // Show custom message if available
    if (action->effect.display_message) {
        printf("\n%s!\n", action->effect.display_message);
    } else {
        printf("\n%s uses %s!\n", attacker->name, action->name);
    }

    Effect *applied_effect = NULL;

    if (action->type == PHYSICAL_ATTACK) {
        // Physical attack: apply effect based on target_type, then compute damage
        if (action->target_type == TARGET_SELF) {
            applied_effect = apply_action_to_target(attacker, action);
        } else if (action->target_type == TARGET_OPPONENT) {
            applied_effect = apply_action_to_target(defender, action);
        }

        if (applied_effect == NULL) {
            printf("Error while applying effect\n");
        } else if (applied_effect->on_tick != NULL) {
            effect_tick(attacker, defender, applied_effect);
        }

        // Calculate and apply damage (only for physical attacks)
        int dmg = compute_physical_damage(attacker, defender);
        if (dmg > 0) {
            entity_take_damage(defender, dmg);
            printf("%s deals %d damage to %s!\n", attacker->name, dmg, defender->name);
            printf("%s now has %d/%d HP remaining.\n",
                   defender->name,
                   defender->current_health_points,
                   defender->max_health_points);
        } else {
            printf("%s blocked the attack!\n", defender->name);
        }
    } else if (action->type == SPECIAL_SKILL) {
        // Special skill: apply effect based on target_type, NO damage
        if (action->target_type == TARGET_SELF) {
            applied_effect = apply_action_to_target(attacker, action);
        } else if (action->target_type == TARGET_OPPONENT) {
            applied_effect = apply_action_to_target(defender, action);
        }

        if (applied_effect == NULL) {
            printf("Error while applying effect\n");
        } else if (applied_effect->on_tick != NULL) {
            effect_tick(attacker, defender, applied_effect);
        }
        // NO damage computation for SPECIAL_SKILL
    }

    // Set cooldown
    action->cooldown_remaining = action->cooldown_turns;

    // Check defeat
    return !defender->is_alive ? 1 : 0;
}

/**
 * @brief Executes the player's turn
 * @param player Pointer to the player
 * @param alive_count Number of alive creatures
 * @return 0 if player died, 1 if turn completed successfully, -1 if should retry (e.g., cooldown)
 */
static int player_turn(Player *player, int alive_count) {
    // CHANGES
    // DISPLAY INVENTORY ITEMS
    printf("\n=== Your Inventory ===\n");
    for (int i = 0; i < player->inventory.count; i++) {
        Item *item = &player->inventory.items[i];
        printf("%d. %s", i + 1, item->name);
        int is_on_cooldown = item_on_cooldown(item);
        if (is_on_cooldown && item->type == ITEM_WEAPON) {
            printf("[Cooldown]");
        }
        printf("\n");
    }
    int item_choice = current_interface->get_choice("Choose your item", 1, player->inventory.count);
    Item *chosen_item = &player->inventory.items[item_choice - 1];

    // IF IT HAS ACTIONS, CHECKS WHETHER ALL ACTIONS ARE ON COOLDOWN, IF SO YOU CANNOT SELECT THAT ITEM
    // THEN DISPLAY ITEM ACTIONS LIKE WE DOING NOW
    if (item_on_cooldown(chosen_item) && chosen_item->type == ITEM_WEAPON) {
        printf("\n%s's actions are all on cooldown! Choose another item.\n", chosen_item->name);
        return -1; // Retry turn
    }

    // IF IT IS A CONSUMABLE, CALL USE_CONSUMABLE FUNCTION WHICH WE WILL IMPLEMENT AFTER

    if (chosen_item->type == ITEM_WEAPON) {
        // Display possible actions from the chosen item
        printf("\n=== %s Actions ===\n", chosen_item->name);
        for (int i = 0; i < chosen_item->action_count; i++) {
            Action *action = &chosen_item->actions[i];
            printf("%d. %s", i + 1, action->name);
            if (action->cooldown_remaining > 0) {
                printf(" [Cooldown: %d turns]", action->cooldown_remaining);
            }
            if (action->type == PHYSICAL_ATTACK) {
                printf(" (Applies damage to the enemy's stats)");
            } else if (action->type == SPECIAL_SKILL) {
                printf(" (Boost your own stats!)");
            }
            printf("\n");
        }

        // Player chooses action from the item
        int action_choice = current_interface->get_choice("Choose your action", 1, chosen_item->action_count);
        Action *chosen_action = &chosen_item->actions[action_choice - 1];

        // Check cooldown
        if (chosen_action->cooldown_remaining > 0) {
            printf("\n%s is on cooldown! Choose another action.\n", chosen_action->name);
            return -1; // Retry turn
        }

        Creature *target = NULL;

        // Only ask for target if it's a physical_attack
        if (chosen_action->type == PHYSICAL_ATTACK) {
            int target_choice = current_interface->get_choice("Choose your target", 1, alive_count);
            target = get_alive_creature_at(target_choice);

            if (!target) {
                printf("Invalid target!\n");
                return -1;
            }
            // OXYGEN DECREASE = -2 to -4
        } else {
            // For SPECIAL_SKILL, use first creature as dummy (won't be used anyway)
            target = get_alive_creature_at(1);
            // OXYGEN DECREASE = -5 to -8
        }

        // Execute the attack (Attack function handles effect application and damage)
        int defeated = Attack(&player->base, chosen_action, &target->base);

        if (defeated) {
            printf("\n>> You defeated the %s! <<\n", target->base.name);
        }
    } else {
        use_consumable(player, chosen_item);
        if (chosen_item->quantity <= 0) {
            remove_item_to_inventory(&player->inventory, chosen_item);
        }
    }

    // Decrement cooldowns for ALL item actions in inventory
    for (int i = 0; i < player->inventory.count; i++) {
        Item *item = &player->inventory.items[i];
        for (int j = 0; j < item->action_count; j++) {
            if (item->actions[j].cooldown_remaining > 0) {
                item->actions[j].cooldown_remaining--;
            }
        }
    }

    // Tick player's effects at THE END of their turn
    all_effects_tick(&player->base, NULL);

    // Check if the player is still alive after effects
    if (!player->base.is_alive) {
        printf("\nYou died from your afflictions!\n");
        current_interface->display_defeat();
        return 0;
    }

    return 1; // Turn completed successfully
}

/**
 * @brief Executes all creature turns
 * @param creatures Array of creature pointers
 * @param creature_count Number of creatures
 * @param player Pointer to the player
 * @return 0 if player died, 1 if turns completed successfully
 */
static int creature_turns(Creature **creatures, int creature_count, Player *player) {
    printf("\n--- Enemy Turn ---\n");

    for (int i = 0; i < creature_count; i++) {
        if (!creatures[i]->base.is_alive) continue;
        if (!player->base.is_alive) break;

        Creature *attacker = creatures[i];

        // Select action for creature
        Action *action = select_action(attacker);
        if (!action) {
            printf("%s has no available actions!\n", attacker->base.name);
            continue;
        }

        // Creature attacks player
        int player_defeated = Attack(&attacker->base, action, &player->base);

        if (player_defeated) {
            current_interface->show_defeat_by(attacker->base.name);
            current_interface->display_defeat();
            return 0; // Player died
        }

        // Tick creature's effects at the END of its turn
        all_effects_tick(&attacker->base, &player->base);

        if (!attacker->base.is_alive) {
            printf("%s died from its own effects!\n", attacker->base.name);
            continue;
        }
    }

    return 1; // All creature turns completed
}

int battle_loop(Player *player, Difficulty difficulty) {
    if (!player) return 0;

    // Display battle start
    current_interface->display_battle_start();

    // Generate creatures based on difficulty
    int creature_count = 0;
    Creature **creatures = generate_creatures(difficulty, &creature_count);

    if (!creatures) {
        printf("Error: Failed to generate creatures!\n");
        return 0;
    }

    // Display all enemies at start
    current_interface->display_combat_intro(creatures, creature_count);

    current_interface->wait_for_enter("\nPress Enter to begin combat...");

    int round = 1;

    // Main combat loop
    while (player->base.is_alive) {
        // Update combat state for interface queries
        set_combat_state(player, creatures, creature_count, round);

        // Display round header
        current_interface->display_round_header(round);

        // Check victory condition
        int alive_count = get_alive_creature_count();
        if (alive_count == 0) {
            current_interface->display_victory();
            free_generated_creatures(creatures, creature_count);
            return 1;
        }

        // Display combat state (player + enemies)
        current_interface->display_combat_state();

        // ====== PHASE 1: PLAYER TURN ======
        int player_result = player_turn(player, alive_count);

        if (player_result == 0) {
            // Player died
            free_generated_creatures(creatures, creature_count);
            return 0;
        } else if (player_result == -1) {
            // Retry turn (cooldown or invalid target)
            continue;
        }

        // ====== PHASE 2: ENEMY TURN ======
        int creature_result = creature_turns(creatures, creature_count, player);

        if (creature_result == 0) {
            // Player was defeated by creatures
            free_generated_creatures(creatures, creature_count);
            return 0;
        }


        round++;
        current_interface->wait_for_enter("\nPress Enter to continue...");
    }

    free_generated_creatures(creatures, creature_count);
    return 0;
}
