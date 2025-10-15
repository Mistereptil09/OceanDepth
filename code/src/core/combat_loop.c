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
    attacker->attack.to_calculate = true;
    defender->defense.to_calculate = true;
    int atk = attacker->attack.current_value;
    int def = defender->defense.current_value;
    int raw = atk - def;
    if (raw < 0) raw = 0;
    return raw;
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

        /** Making sure that current_values of player attack and creature's defenses are fresh cached_values **/
        stat_prepare_for_turn(&player->base.attack);
        for (int i = 0; i < creature_count; i++) {
            stat_prepare_for_turn(&creatures[i]->base.defense);
        }

        // Tick player's effects at start of their turn
        all_effects_tick(&player->base, NULL);

        if (!player->base.is_alive) {
            printf("\nYou died from your afflictions!\n");
            current_interface->display_defeat();
            free_generated_creatures(creatures, creature_count);
            return 0;
        }

        printf("\n=== Your Actions ===\n");
        for (int i = 0; i < player->base.action_count; i++) {
            Action* action = &player->base.actions[i];
            printf("%d. %s", i + 1, action->name);
            if (action->cooldown_remaining > 0) {
                printf(" [Cooldown: %d turns]", action->cooldown_remaining);
            }
            if (action->type == PHYSICAL_ATTACK) {
                printf(" (Attack)");
            } else if (action->type == SPECIAL_SKILL) {
                printf(" (Buff)");
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

        if (chosen_action->type == PHYSICAL_ATTACK) {
            // Player chooses target for attack
            int target_choice = current_interface->get_choice("Choose your target", 1, alive_count);
            Creature* target = get_alive_creature_at(target_choice);

            if (!target) {
                printf("Invalid target!\n");
                continue;
            }

            printf("\nYou use %s on the %s!\n", chosen_action->name, target->base.name);

            // Apply effect first (e.g., Bleed)
            apply_action_to_target(&target->base, *chosen_action);

            /** if effect has an on_tick, separately apply it now */
            if (chosen_action->effect.on_tick != NULL) {
                effect_tick(&player->base, &target->base, &chosen_action->effect);
            }


            // Then calculate and deal damage
            int dmg = compute_physical_damage(&player->base, &target->base);
            if (dmg > 0) {
                entity_take_damage(&target->base, dmg);
                printf("You deal %d damage to the %s!\n", dmg, target->base.name);
            } else {
                printf("Your attack was blocked!\n");
            }

            if (!target->base.is_alive) {
                printf("\n>> You defeated the %s! <<\n", target->base.name);
            }

            // Set cooldown
            chosen_action->cooldown_remaining = chosen_action->cooldown_turns;

        } else if (chosen_action->type == SPECIAL_SKILL) {
            // Self-buff skill
            printf("\nYou use %s!\n", chosen_action->name);
            apply_action_to_target(&player->base, *chosen_action);
            printf("You feel empowered!\n");

            // Set cooldown
            /** Make sure that hardcoded Action cooldowns and associated Effect's turns are cohesive */
            chosen_action->cooldown_remaining = chosen_action->cooldown_turns;
        }

        // ====== PHASE 2: ENEMY TURN ======
        printf("\n--- Enemy Turn ---\n");
        for (int i = 0; i < creature_count; i++) {
            if (!creatures[i]->base.is_alive) continue;
            if (!player->base.is_alive) break;

            Creature* attacker = creatures[i];

            /** YAS : same thing as for player, stat_prepare_for_turn should be called on creature stats and player stats that might get modified by effect functions without being cached */

            // Tick creature's effects at start of its turn
            all_effects_tick(&attacker->base, &player->base);

            if (!attacker->base.is_alive) {
                printf("%s died from its own effects!\n", attacker->base.name);
                continue;
            }

            /** YAS : Good, but note to self -> check that the random selection also handles the validity conditions if it still doesn't */
            Action* action = select_action(attacker);
            if (!action) {
                printf("%s has no available actions!\n", attacker->base.name);
                continue;
            }

            // Show what the creature is doing
            current_interface->show_action(attacker->base.name, action->name);

            if (action->type == PHYSICAL_ATTACK) {
                // Apply effect first (e.g., Poison/Bleed debuff on player)
                /** YAS : again, shouldn't be added to list before ticking ? */
                apply_action_to_target(&player->base, *action);

                // Then deal damage
                int player_hp_before = player->base.current_health_points;
                int dmg = compute_physical_damage(&attacker->base, &player->base);
                if (dmg > 0) {
                    entity_take_damage(&player->base, dmg);
                }

                // Show damage
                int damage_dealt = player_hp_before - player->base.current_health_points;
                if (damage_dealt > 0) {
                    printf("%s deals %d damage to you!\n", attacker->base.name, damage_dealt);
                    printf("You have %d/%d HP remaining.\n",
                           player->base.current_health_points,
                           player->base.max_health_points);
                } else {
                    printf("You blocked the attack!\n");
                }

                if (!player->base.is_alive) {
                    current_interface->show_defeat_by(attacker->base.name);
                    current_interface->display_defeat();
                    free_generated_creatures(creatures, creature_count);
                    return 0;
                }
            } else if (action->type == SPECIAL_SKILL) {
                // Self-buff: apply effect to self
                apply_action_to_target(&attacker->base, *action);
                printf("%s buffed itself!\n", attacker->base.name);
            }
        }

        round++;
        current_interface->wait_for_enter("\nPress Enter to continue...");
    }

    free_generated_creatures(creatures, creature_count);
    return 0;
}


int attack(EntityBase* attacker, EntityBase* defender) {
    if (!attacker || !defender) return 0;

    int dmg = compute_physical_damage(attacker, defender);
    if (dmg > 0) {
        entity_take_damage(defender, dmg);
        printf("%s deals %d damage to %s!\n", attacker->name, dmg, defender->name);
    } else {
        printf("%s's attack was blocked by %s!\n", attacker->name, defender->name);
    }

    if (!defender->is_alive) {
        printf("%s has been defeated!\n", defender->name);
        return 1;
    }
    return 0;
}