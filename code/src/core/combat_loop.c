#include "core/combat_loop.h"
#include "core/effect.h"
#include "core/entity.h"
#include "core/action.h"
#include "core/creature.h"
#include "core/player.h"
#include "interface/interface_table.h"
#include "interface/interface_api.h"
#include <stdio.h>

int compute_physical_damage(EntityBase* attacker, EntityBase* defender)
{
    if (!attacker || !defender) return 0;
    // Ensure we recalc if base values changed directly in tests
    printf("[DEBUG] === COMPUTING DAMAGE ===\n");
    printf("[DEBUG] Attacker: %s, Defender: %s\n", attacker->name, defender->name);
    printf("[DEBUG] ATTACKER'S ATTACK BASE VALUE : %d\n", attacker->attack.base_value);
    printf("[DEBUG] ATTACKER'S ATTACK CACHED VALUE BEFORE RECALC : %d\n", attacker->attack.cached_value);
    printf("[DEBUG] DEFENDER'S DEFENSE BASE VALUE : %d\n", defender->defense.base_value);
    printf("[DEBUG] DEFENDER'S DEFENSE CACHED VALUE AFTER RECALC : %d\n", defender->defense.cached_value);

    attacker->attack.to_calculate = true;
    defender->defense.to_calculate = true;

    int atk = stat_get_value(&attacker->attack);
    int def = stat_get_value(&defender->defense);

    printf("[DEBUG] %s'S ATTACK CACHED VALUE AFTER RECALC : %d\n", attacker->name, attacker->attack.cached_value);
    printf("[DEBUG] %s'S DEFENSE CACHED VALUE AFTER RECALC : %d\n", defender->name, defender->defense.cached_value);

    int raw = atk - def;
    if (raw < 0) raw = 0;

    printf("COMPUTE_PHYSICAL_DAMAGE : ATK %d - DEF %d = RAW %d\n", atk, def, raw);

    return raw;
}

int Attack(EntityBase* attacker, Action* action, EntityBase* defender){
    // Show what is the chosen action
    current_interface->show_action(attacker->name, action->name);

    Effect *effect_to_apply = NULL;
    if (action->type == PHYSICAL_ATTACK) {
        effect_to_apply = apply_action_to_target(defender, action);
        
        if (effect_to_apply == NULL) {
            printf("Error while applying the effect");
        } else {
            if (effect_to_apply->on_tick != NULL) {
                effect_tick(attacker, defender, effect_to_apply);
            }
        }

        // Deal damage only for physical attacks
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
        // Self-buff: apply effect to self (no damage dealt)
        effect_to_apply = apply_action_to_target(attacker, action);
        printf("%s buffed itself!\n", attacker->name);
        
        if (effect_to_apply == NULL) {
            printf("Error while applying the buff effect");
        }
    }

    action->cooldown_remaining = action->cooldown_turns;
    if (!defender->is_alive) {
        return 1;
    }

    return 0;
}

int battle_loop(Player* player, Difficulty difficulty) {
    if (!player) return 0;

    // Display battle start
    current_interface->display_battle_start();

    // Generate creatures based on difficulty
    int creature_count = 0;
    Creature** creatures = generate_creatures(difficulty, &creature_count);

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

        /** TO DO -> CALCULATE HOW MANY ATTACKS CAN BE DONE DURING PLAYER'S TURN BASED ON FATIGUE LEVELS **/

        // Displays possible actions to player
        // Tick player's effects at start of their turn
        all_effects_tick(&player->base, NULL);

        // Checks if the player is still alive
        if (!player->base.is_alive) {
            printf("\nYou died from your afflictions!\n");
            current_interface->display_defeat();
            free_generated_creatures(creatures, creature_count);
            return 0;
        }

        // Displays possible actions to player
        printf("\n=== Your Actions ===\n");
        for (int i = 0; i < player->base.action_count; i++) {
            Action* action = &player->base.actions[i];
            printf("%d. %s", i + 1, action->name);
            if (action->cooldown_remaining > 0) {
                printf(" [Cooldown: %d turns]", action->cooldown_remaining);
            }
            if (action->type == PHYSICAL_ATTACK) { // applies to others
                printf(" (Applies damage to the ennemy's stats)");
            } else if (action->type == SPECIAL_SKILL) { // applies to player
                printf(" (Boost your own stats!)");
            }
            printf("\n");
        }

        // Player chooses action
        int action_choice = current_interface->get_choice("Choose your action", 1, player->base.action_count);
        Action* chosen_action = &player->base.actions[action_choice - 1];

        // Check cooldown
        if (chosen_action->cooldown_remaining > 0) {
            printf("\n%s is on cooldown! Choose another action.\n", chosen_action->name);
            continue;
        }

        // Decrement all cooldowns at start of player turn
        for (int i = 0; i < player->base.action_count; i++) {
            if (player->base.actions[i].cooldown_remaining > 0) {
                player->base.actions[i].cooldown_remaining--;
            }
        }

        // Player chooses target to attack (attack happens regardless of the action chosen
        int target_choice = current_interface->get_choice("Choose your target", 1, alive_count);
        Creature* target = get_alive_creature_at(target_choice);

        if (!target) {
            printf("Invalid target!\n");
            continue;
        }

        Attack(&player->base, chosen_action, &target->base);

        // Tick player's effects at THE END of their turn
        all_effects_tick(&player->base, NULL);

        // Checks if the player is still alive
        if (!player->base.is_alive) {
            printf("\nYou died from your afflictions!\n");
            current_interface->display_defeat();
            free_generated_creatures(creatures, creature_count);
            return 0;
        }

        // ====== PHASE 2: ENEMY TURN ======
        printf("\n--- Enemy Turn ---\n");
        for (int i = 0; i < creature_count; i++) {
            if (!creatures[i]->base.is_alive) continue;
            if (!player->base.is_alive) break;

            Creature* attacker = creatures[i];

            /** NOTE :check that the random selection also handles the validity conditions if it still doesn't */
            Action* action = select_action(attacker);
            if (!action) {
                printf("%s has no available actions!\n", attacker->base.name);
                continue;
            }

            // Creature attacks player with its own action
            Attack(&attacker->base, action, &player->base);

            if (!player->base.is_alive) {
                current_interface->show_defeat_by(attacker->base.name);
                current_interface->display_defeat();
                free_generated_creatures(creatures, creature_count);
                return 0;
            }

            action->cooldown_remaining = action->cooldown_turns;

            // Tick creature's effects at the END of its turn
            all_effects_tick(&attacker->base, &player->base);

            if (!attacker->base.is_alive) {
                printf("%s died from its own effects!\n", attacker->base.name);
                continue;
            }
        }

        round++;
        current_interface->wait_for_enter("\nPress Enter to continue...");
    }

    free_generated_creatures(creatures, creature_count);
    return 0;
}