#include "core/combat_loop.h"
#include "core/effect.h"
#include "core/entity.h"
#include "core/action.h"
#include "core/creature.h"
#include "core/player.h"
#include "core/item.h"
#include "core/inventory.h"
#include "core/reward_system.h"
#include "interface/interface_table.h"
#include "interface/interface_api.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

int compute_physical_damage(EntityBase *attacker, EntityBase *defender) {
    if (!attacker || !defender) return 0;


    // Force recalculation
    attacker->attack.to_calculate = true;
    defender->defense.to_calculate = true;

    int atk = stat_get_value(&attacker->attack);
    int def = stat_get_value(&defender->defense);


    int raw = atk - def;
    if (raw < 0) raw = 0;

    current_interface->show_damage_calculation(atk, def, raw);
    return raw;
}

int Attack(EntityBase *attacker, Action *action, EntityBase *defender) {
    if (!attacker || !defender || !action) return 0;

    current_interface->show_action(attacker->name, action->name);

    if (action->effect.display_message) {
        current_interface->show_action_effect(action->effect.display_message);
    }

    Effect *applied_effect = NULL;

    if (action->type == PHYSICAL_ATTACK) {
        // physical attack: apply effect based on target_type, then compute damage
        if (action->target_type == TARGET_SELF) {
            applied_effect = apply_action_to_target(attacker, action);
        } else if (action->target_type == TARGET_OPPONENT) {
            applied_effect = apply_action_to_target(defender, action);
        }

        if (applied_effect == NULL) {
            current_interface->show_effect_error();
        } else if (applied_effect->on_tick != NULL) {
            effect_tick(attacker, defender, applied_effect);
        }

        // calculate and apply damage
        int dmg = compute_physical_damage(attacker, defender);
        if (dmg > 0) {
            entity_take_damage(defender, dmg);
            current_interface->show_damage_dealt(attacker->name, defender->name,
                                                dmg, defender->current_health_points,
                                                defender->max_health_points);

            // if defender is a player, apply oxygen stress
            if (defender->type == ENTITY_PLAYER) {
                Player *player_defender = (Player*)((char*)defender - offsetof(Player, base));
                int oxygen_stress = 1;
                consume_oxygen(player_defender, oxygen_stress);
                current_interface->show_oxygen_stress(oxygen_stress,
                                                     player_defender->base.oxygen_level,
                                                     player_defender->base.max_oxygen_level);
            }
        } else {
            current_interface->show_attack_blocked(defender->name);
        }
    } else if (action->type == SPECIAL_SKILL) {
        // Special skill: apply effect based on target_type, NO damage
        if (action->target_type == TARGET_SELF) {
            applied_effect = apply_action_to_target(attacker, action);
        } else if (action->target_type == TARGET_OPPONENT) {
            applied_effect = apply_action_to_target(defender, action);
        }

        if (applied_effect == NULL) {
            current_interface->show_effect_error();
        } else if (applied_effect->on_tick != NULL) {
            effect_tick(attacker, defender, applied_effect);
        }
        // NO damage computation for SPECIAL_SKILL
    }

    action->cooldown_remaining = action->cooldown_turns;

    return !defender->is_alive ? 1 : 0;
}

/**
 * @brief Executes the player's turn
 * @param player Pointer to the player
 * @param alive_count Number of alive creatures
 * @return 0 if player died, 1 if turn completed successfully, -1 if should retry (e.g., cooldown)
 */
static int player_turn(Player *player, int alive_count) {
    // DISPLAY INVENTORY ITEMS
    Item *chosen_item = NULL;
    while (chosen_item == NULL) {
        // DISPLAY INVENTORY ITEMS
        current_interface->show_inventory_selection(player->inventory.items, player->inventory.count);
        int item_choice = current_interface->get_choice("Choose your item", 1, player->inventory.count);
        Item *selected_item = &player->inventory.items[item_choice - 1];

        // VALIDATION: Check if weapon is on cooldown
        if (item_on_cooldown(selected_item) && selected_item->type == ITEM_WEAPON) {
            current_interface->show_item_on_cooldown(selected_item->name);
            continue;
        }

        // VALIDATION: Check if consumable would have no effect
        if (selected_item->type == ITEM_CONSUMABLE) {
            int would_have_effect = 0;

            if (selected_item->hp_boost > 0 && player->base.current_health_points < player->base.max_health_points) {
                would_have_effect = 1;
            }
            if (selected_item->oxygen_boost > 0 && player->base.oxygen_level < player->base.max_oxygen_level) {
                would_have_effect = 1;
            }
            if (selected_item->fatigue_relief > 0 && player->base.fatigue_level > 0) {
                would_have_effect = 1;
            }

            if (!would_have_effect) {
                current_interface->show_consumable_no_effect(selected_item->name);
                continue; // Re-ask for item choice
            }
        }

        chosen_item = selected_item;
    }

    if (chosen_item->type == ITEM_WEAPON) {
        // Display possible actions from the chosen item
        current_interface->show_weapon_actions(chosen_item);

        // player chooses action from the item (or auto-select if only one)
        int action_choice;
        if (chosen_item->action_count == 1) {
            action_choice = 1;
            current_interface->show_auto_selection(chosen_item->actions[0].name);
        } else {
            action_choice = current_interface->get_choice("Choose your action", 1, chosen_item->action_count);
        }
        Action *chosen_action = &chosen_item->actions[action_choice - 1];

        if (chosen_action->cooldown_remaining > 0) {
            current_interface->show_action_on_cooldown(chosen_action->name);
            return -1;
        }

        Creature *target = NULL;

        // Only ask for target if it's a physical_attack
        if (chosen_action->type == PHYSICAL_ATTACK) {
            int target_choice;
            if (alive_count == 1) {
                target_choice = 1;
                target = get_alive_creature_at(target_choice);
                current_interface->show_auto_selection(target->base.name);
            } else {
                target_choice = current_interface->get_choice("Choisissez votre cible", 1, alive_count);
                target = get_alive_creature_at(target_choice);
            }


            if (!target) {
                current_interface->show_invalid_target();
                return -1;
            }
        } else {
            // For SPECIAL_SKILL, use first creature as dummy
            target = get_alive_creature_at(1);
        }

        // Consume oxygen based on action type BEFORE executing attack
        int oxygen_cost;
        if (chosen_action->type == PHYSICAL_ATTACK) {
            oxygen_cost = 1 + (rand() % 2); // 1-2 oxygen (reduced from 2-4)
        } else {
            oxygen_cost = 3 + (rand() % 3); // 3-5 oxygen (reduced from 5-8)
        }

        consume_oxygen(player, oxygen_cost);
        current_interface->show_oxygen_consumed(oxygen_cost,
                                                player->base.oxygen_level,
                                                player->base.max_oxygen_level);

        if (target) {
            int defeated = Attack(&player->base, chosen_action, &target->base);

            if (defeated) {
                current_interface->show_creature_defeated(target->base.name);
            }
        } else if (chosen_action->type == SPECIAL_SKILL && chosen_action->target_type == TARGET_SELF) {
            // SPECIAL_SKILL targeting self when no enemies remain - just apply the effect to player
            Effect *applied_effect = apply_action_to_target(&player->base, chosen_action);
            if (applied_effect && applied_effect->on_tick != NULL) {
                effect_tick(&player->base, NULL, applied_effect);
            }
            chosen_action->cooldown_remaining = chosen_action->cooldown_turns;
        }
    } else {
        use_consumable(player, chosen_item);
        if (chosen_item->quantity <= 0) {
            remove_item_to_inventory(&player->inventory, chosen_item);
        }
    }

    // Decrement cooldowns for ALL item actions
    for (int i = 0; i < player->inventory.count; i++) {
        Item *item = &player->inventory.items[i];
        for (int j = 0; j < item->action_count; j++) {
            if (item->actions[j].cooldown_remaining > 0) {
                item->actions[j].cooldown_remaining--;
            }
        }
    }

    // Check oxygen critical level (DISPLAY IN RED)
    if (player->base.oxygen_level <= 10 && player->base.oxygen_level > 0) {
        current_interface->show_oxygen_critical(player->base.oxygen_level);
    }

    // Check oxygen & take damage if 0
    if (player->base.oxygen_level <= 0) {
        entity_take_damage(&player->base, 5);
        current_interface->show_oxygen_death(5, player->base.current_health_points,
                                            player->base.max_health_points);

        if (!player->base.is_alive) {
            current_interface->show_death_from_suffocation();
            current_interface->display_defeat();
            return 0;
        }
    }

    // Tick player's effects at THE END of their turn
    all_effects_tick(&player->base, NULL);

    if (!player->base.is_alive) {
        current_interface->show_death_from_afflictions();
        current_interface->display_defeat();
        return 0;
    }

    return 1;
}

/**
 * @brief Executes all creature turns
 * @param creatures Array of creature pointers
 * @param creature_count Number of creatures
 * @param player Pointer to the player
 * @return 0 if player died, 1 if turns completed successfully
 */
static int creature_turns(Creature **creatures, int creature_count, Player *player) {
    current_interface->show_enemy_turn();

    for (int i = 0; i < creature_count; i++) {
        if (!creatures[i]->base.is_alive) continue;
        if (!player->base.is_alive) break;

        Creature *attacker = creatures[i];

        // Select action for creature
        Action *action = select_action(attacker);
        if (!action) {
            current_interface->show_no_actions_available(attacker->base.name);
            continue;
        }

        // Creature attacks player
        int player_defeated = Attack(&attacker->base, action, &player->base);

        if (player_defeated) {
            current_interface->show_defeat_by(attacker->base.name);
            current_interface->display_defeat();
            return 0; // Player died
        }

        // Check oxygen critical level after enemy attack
        if (player->base.oxygen_level <= 10 && player->base.oxygen_level > 0) {
            current_interface->show_oxygen_critical(player->base.oxygen_level);
        }

        // Tick creature's effects at the END of its turn
        all_effects_tick(&attacker->base, &player->base);

        if (!attacker->base.is_alive) {
            current_interface->show_creature_died_from_effects(attacker->base.name);
            continue;
        }
    }

    return 1; // All creature turns completed
}

int battle_loop(Player *player, Difficulty difficulty, int seed) {
    if (!player) return 0;
    srand(seed);

    // Reset fatigue before fight
    player->base.fatigue_level = 0;

    // Display battle start
    current_interface->display_battle_start();

    // Generate creatures based on difficulty
    int creature_count = 0;
    Creature **creatures = generate_creatures(difficulty, &creature_count);

    if (!creatures) {
        current_interface->show_creature_generation_error();
        return 0;
    }

    // Display all enemies at start
    current_interface->display_combat_intro(creatures, creature_count);

    current_interface->wait_for_enter("\nAppuyez sur Entree pour commencer le combat...");

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
            // Post-battle rewards
            award_post_battle_rewards(player, difficulty);
            free_generated_creatures(creatures, creature_count);
            return 1;
        }

        // Display combat state (player + enemies)
        current_interface->display_combat_state();

        // ====== PHASE 1: PLAYER TURN ======

        // Calculate max actions based on current fatigue
        int max_actions;
        if (player->base.fatigue_level >= 4) {
            max_actions = 1; // High fatigue: 1 action only
        } else if (player->base.fatigue_level >= 2) {
            max_actions = 2; // Medium fatigue: 2 actions
        } else {
            max_actions = 3; // Low fatigue: 3 actions
        }

        current_interface->show_your_turn();
        current_interface->show_fatigue_status(player->base.fatigue_level, max_actions);

        // Allow multiple actions based on fatigue
        int actions_taken = 0;
        while (actions_taken < max_actions) {
            // Update alive count BEFORE asking for another action
            alive_count = get_alive_creature_count();
            if (alive_count == 0) {
                break; // All enemies defeated
            }

            // Ask if player wants to take another action (only after first action)
            if (actions_taken > 0) {
                // Check again if enemies are still alive before asking
                alive_count = get_alive_creature_count();
                if (alive_count == 0) {
                    break; // All enemies defeated during previous action
                }

                int continue_action = current_interface->get_choice(
                    "Effectuer une autre action? (1=Oui, 2=Non)", 1, 2);
                if (continue_action == 2) {
                    current_interface->show_ending_turn();
                    break;
                }
            }

            int player_result = player_turn(player, alive_count);

            if (player_result == 0) {
                // Player died
                free_generated_creatures(creatures, creature_count);
                return 0;
            } else if (player_result == -1) {
                // Retry turn (cooldown or invalid target) - don't count as action
                continue;
            }

            // Action was successful - increase fatigue and count
            actions_taken++;
            if (player->base.fatigue_level < MAX_FATIGUE) {
                player->base.fatigue_level++;
                current_interface->show_fatigue_increased(player->base.fatigue_level);
            }

            // Update combat state after action to refresh alive_count cache
            set_combat_state(player, creatures, creature_count, round);
        }

        current_interface->show_actions_taken(actions_taken);

        // Check if all enemies defeated after player turn
        if (get_alive_creature_count() == 0) {
            current_interface->display_victory();
            // Post-battle rewards
            award_post_battle_rewards(player, difficulty);
            free_generated_creatures(creatures, creature_count);
            return 1;
        }

        // ====== PHASE 2: PASSIVE OXYGEN CONSUMPTION ======
        // Passive oxygen consumption per round (exploration cost)
        int passive_oxygen = 1; // Base passive consumption (reduced from 2)
        consume_oxygen(player, passive_oxygen);
        current_interface->show_passive_oxygen(passive_oxygen,
                                              player->base.oxygen_level,
                                              player->base.max_oxygen_level);

        // Check oxygen critical/death after passive consumption
        if (player->base.oxygen_level <= 10 && player->base.oxygen_level > 0) {
            current_interface->show_oxygen_critical(player->base.oxygen_level);
        }

        if (player->base.oxygen_level <= 0) {
            entity_take_damage(&player->base, 5);
            current_interface->show_oxygen_death(5, player->base.current_health_points,
                                                player->base.max_health_points);

            if (!player->base.is_alive) {
                current_interface->show_death_from_suffocation();
                current_interface->display_defeat();
                free_generated_creatures(creatures, creature_count);
                return 0;
            }
        }

        // ====== PHASE 3: ENEMY TURN ======
        int creature_result = creature_turns(creatures, creature_count, player);

        if (creature_result == 0) {
            // Player was defeated by creatures
            free_generated_creatures(creatures, creature_count);
            return 0;
        }

        // ====== PHASE 4: END OF ROUND ======

        // Recover fatigue at end of round
        if (player->base.fatigue_level > 0) {
            player->base.fatigue_level--;
            current_interface->show_fatigue_recovered(player->base.fatigue_level);
        }

        round++;
        current_interface->wait_for_enter("\nAppuyez sur Entree pour continuer...");
    }

    free_generated_creatures(creatures, creature_count);
    return 0;
}
