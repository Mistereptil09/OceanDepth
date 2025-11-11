//
// Created by cantonio on 02/10/2025.
//
#include <stdio.h>
#include <stdlib.h>

#include "interface/interface_cli.h"
#include "interface/interface_api.h"

#include <string.h>

#include "core/map.h"
#include "core/player.h"
#include "core/creature.h"
#include "helper/input_validator.h"

void cli_display_map(Map* map, Player* player) {
    printf("\n========================================\n");
    printf("|     CARTE DES PROFONDEURS            |\n");
    printf("========================================\n\n");

    for (int row = 0; row < map->rows; row++) {
        printf("%-15s | ", get_depth_name(row));

        for (int col = 0; col < map->cols; col++) {
            if (is_player_at(player, row, col)) {
                printf("[@]");
            } else if (!is_cell_unlocked(player, row, col)) {
                printf("[?]");
            } else {
                Cell* cell = get_cell(map, row, col);
                printf("[%c]", get_cell_symbol(cell));
            }
            printf(" ");
        }
        printf("\n");
    }

    printf("\n========================================\n");
    printf("LEGENDE:\n");
    printf("[@]=Vous  [$]=Shop  [+]=Heal  [S]=Save\n");
    printf("[R]=Facile [W]=Moyen [P]=Difficile\n");
    printf("[A]=Boss  [?]=Verrouille\n");
    printf("========================================\n");
}

Position cli_get_movement_choice(Player* player) {
    printf("\n========================================\n");
    printf("Position actuelle: (%d, %d)\n",
           player->current_position.row, player->current_position.col);
    printf("Ou voulez-vous aller?\n");
    printf("[U] Haut | [D] Bas | [L] Gauche | [R] Droite\n");
    printf("[Q] Quitter\n");
    printf("========================================\n");

    char choice[10];
    cli_get_input("Direction", choice, sizeof(choice));

    return parse_direction_input(choice, player->current_position);
}

void cli_display_combat_state(void)
{
    // Query combat state from API
    const CombatState* state = get_combat_state();
    if (!state || !state->player) return;

    // Display player status
    printf("\n========================================\n");
    printf("|     VOTRE STATUT                     |\n");
    printf("========================================\n");
    printf("Nom: %s\n", get_player_name(state->player));
    printf("PV: %d/%d\n", get_player_hp(state->player), get_player_max_hp(state->player));
    printf("Oxygene: %d/%d\n", get_player_oxygen(state->player), get_player_max_oxygen(state->player));
    printf("Attaque: %d\n", get_player_attack(state->player));
    printf("Defense: %d\n", get_player_defense(state->player));
    printf("Perles: %d\n", get_player_pearls(state->player));
    printf("Fatigue: %d\n", get_player_fatigue(state->player));
    printf("==================\n\n");

    // Display alive enemies
    printf("========================================\n");
    printf("|     ENNEMIS RESTANTS                 |\n");
    printf("========================================\n");
    int alive_count = get_alive_creature_count();
    
    if (alive_count == 0) {
        printf("Aucun ennemi restant!\n");
    } else {
        for (int i = 1; i <= alive_count; i++) {
            Creature* creature = get_alive_creature_at(i);
            if (creature) {
                printf("\n[%d] %s\n", i, get_creature_name(creature));
                printf("    PV: %d/%d\n", 
                       get_creature_hp(creature), 
                       get_creature_max_hp(creature));
                printf("    Attaque: %d | Defense: %d | Vitesse: %d\n",
                       get_creature_attack(creature),
                       get_creature_defense(creature),
                       get_creature_speed(creature));
                
                // Display active effects on the creature
                int effect_count = get_creature_effect_count(creature);
                if (effect_count > 0) {
                    printf("    Effets actifs: ");
                    for (int j = 0; j < effect_count; j++) {
                        const char* effect_name = get_creature_effect_name(creature, j);
                        int turns_left = get_creature_effect_turns(creature, j);
                        if (effect_name) {
                            printf("%s(%d) ", effect_name, turns_left);
                        }
                    }
                    printf("\n");
                }
            }
        }
    }
    printf("========================================\n\n");
}

void cli_display_combat_intro(Creature** creatures, int count)
{
    printf("========================================\n");
    printf("|     ENNEMIS EN APPROCHE!             |\n");
    printf("========================================\n");

    for (int i = 0; i < count; i++) {
        if (creatures[i] && is_creature_alive(creatures[i])) {
            printf("\n=== %s (ID: %d) ===\n",
                   get_creature_name(creatures[i]),
                   get_creature_id(creatures[i]));
            printf("PV: %d/%d\n",
                   get_creature_hp(creatures[i]),
                   get_creature_max_hp(creatures[i]));
            printf("Attaque: %d\n", get_creature_attack(creatures[i]));
            printf("Defense: %d\n", get_creature_defense(creatures[i]));
            printf("Vitesse: %d\n", get_creature_speed(creatures[i]));
            printf("Actions:\n");
            for (int j = 0; j < MAX_ACTIONS; j++) {
                const char* action_name = get_creature_action_name(creatures[i], j);
                if (action_name) {
                    printf("  %d. %s\n", j + 1, action_name);
                }
            }
            printf("==================\n");
        }
    }
}

void cli_display_round_header(int round_number)
{
    printf("\n\n");
    printf("========================================\n");
    printf("        TOUR %d\n", round_number);
    printf("========================================\n");
}

void cli_display_victory(void)
{
    printf("\n\n");
    printf("========================================\n");
    printf("|                                      |\n");
    printf("|      VICTOIRE!                       |\n");
    printf("|                                      |\n");
    printf("========================================\n");
}

void cli_display_defeat(void)
{
    printf("\n\n");
    printf("========================================\n");
    printf("|                                      |\n");
    printf("|      PARTIE TERMINEE                 |\n");
    printf("|                                      |\n");
    printf("========================================\n");
}

void cli_display_battle_start(void)
{
    printf("\n\n");
    printf("========================================\n");
    printf("|                                      |\n");
    printf("|       COMBAT COMMENCE!               |\n");
    printf("|                                      |\n");
    printf("========================================\n\n");
}

void cli_wait_for_enter(const char* prompt)
{
    if (prompt) {
        printf("%s", prompt);
    }
    fflush(stdout);

    // Clear input buffer properly
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void cli_show_action(const char* entity_name, const char* action_name)
{
    printf("\n%s utilise %s!\n", entity_name, action_name);
}

void cli_show_defeat_by(const char* enemy_name)
{
    printf("\n>> Vous avez ete vaincu par %s! <<\n", enemy_name);
}

int cli_get_choice(const char* prompt, const int min, const int max)
{
    char buffer[20];
    int value;

    while (1) {
        printf("%s (%d-%d): ", prompt, min, max);
        fflush(stdout);

        if (get_sanitized_input(buffer, sizeof(buffer))) {
            value = atoi(buffer);  // ← Convert to int

            if (value >= min && value <= max) {
                return value;
            }
            printf("Veuillez entrer un nombre entre %d et %d.\n", min, max);
        } else {
            printf("Entree invalide. Veuillez reessayer.\n");
        }
    }
}
void cli_show_attack(EntityBase* attacker, EntityBase* target, int damage)
{
    printf("%s attaque %s pour %d degats!\n", attacker->name, target->name, damage);
}

void cli_show_inventory(Inventory* inv)
{
    printf("Inventaire!\n");
}

void cli_get_input(const char* prompt, char* result, size_t result_size){

    char* buffer = malloc(result_size * sizeof(char));
    while (1)
    {
        printf("%s : ", prompt);
        fflush(stdout);
        if (get_sanitized_input(buffer, result_size))
        {
            strcpy(result, buffer);
            free(buffer);
            return;
        }
        printf("Entree invalide. Veuillez reessayer.\n");
    }
}

void cli_show_information(const char* message)
{
    if (message && *message) {
        printf("%s\n", message);
    }
}

// ========== COMBAT FEEDBACK IMPLEMENTATIONS ==========

void cli_show_oxygen_consumed(int amount, int current, int max)
{
    printf("Oxygene consomme: -%d (actuel: %d/%d)\n", amount, current, max);
}

void cli_show_oxygen_critical(int current)
{
    printf("\n!  === OXYGENE CRITIQUE! === !\n");
    printf("️! Il ne reste que %d d'oxygene! !\n", current);
    printf("Utilisez une capsule MAINTENANT ou vous allez suffoquer!\n\n");
}

void cli_show_oxygen_death(int damage, int hp, int max_hp)
{
    printf("\n! PLUS D'OXYGENE! Vous suffoquez!\n");
    printf("Vous perdez %d PV par tour jusqu'a utilisation d'une capsule d'oxygene!\n", damage);
    printf("PV: %d/%d\n", hp, max_hp);
}

void cli_show_fatigue_status(int fatigue, int max_actions)
{
    printf("Fatigue: %d/%d (Vous pouvez effectuer jusqu'a %d action(s) ce tour)\n",
           fatigue, MAX_FATIGUE, max_actions);
}

void cli_show_fatigue_increased(int new_fatigue)
{
    printf("Fatigue augmentee a %d/%d\n", new_fatigue, MAX_FATIGUE);
}

void cli_show_fatigue_recovered(int new_fatigue)
{
    printf("\nFatigue reduite a %d/%d (recuperation...)\n", new_fatigue, MAX_FATIGUE);
}

void cli_show_passive_oxygen(int amount, int current, int max)
{
    printf("Consommation passive d'oxygene: -%d (actuel: %d/%d)\n\n", amount, current, max);
}

void cli_show_damage_dealt(const char* attacker_name, const char* target_name,
                           int damage, int target_hp, int target_max_hp)
{
    printf("%s inflige %d degats a %s!\n", attacker_name, damage, target_name);
    printf("%s a maintenant %d/%d PV restants.\n", target_name, target_hp, target_max_hp);
}

void cli_show_attack_blocked(const char* defender_name)
{
    printf("%s a bloque l'attaque!\n", defender_name);
}

void cli_show_creature_defeated(const char* creature_name)
{
    printf("\n>> Vous avez vaincu le %s! <<\n", creature_name);
}

void cli_show_actions_taken(int actions_taken)
{
    printf("\nVous avez effectue %d action(s) ce tour.\n", actions_taken);
}

void cli_show_death_from_afflictions(void)
{
    printf("\nVous êtes mort de vos afflictions!\n");
}

void cli_show_death_from_suffocation(void)
{
    printf("\nVous êtes mort par suffocation!\n");
}

void cli_show_enemy_turn(void)
{
    printf("\n--- Tour des Ennemis ---\n");
}

void cli_show_your_turn(void)
{
    printf("\n=== Votre Tour ===\n");
}

void cli_show_ending_turn(void)
{
    printf("Fin de votre tour...\n");
}

void cli_show_action_effect(const char* message)
{
    printf("\n%s!\n", message);
}

void cli_show_effect_error(void)
{
    printf("Erreur lors de l'application de l'effet\n");
}

void cli_show_action_on_cooldown(const char* action_name)
{
    printf("\n%s est en recharge! Choisissez une autre action.\n", action_name);
}

void cli_show_item_on_cooldown(const char* item_name)
{
    printf("\nToutes les actions de %s sont en recharge! Choisissez un autre objet.\n", item_name);
}

void cli_show_no_actions_available(const char* creature_name)
{
    printf("%s n'a aucune action disponible!\n", creature_name);
}

void cli_show_creature_died_from_effects(const char* creature_name)
{
    printf("%s est mort de ses propres effets!\n", creature_name);
}

Item* cli_ask_item_choice_reward(int max_item_pool_size, Item* drawn_item)
{
    printf("\n=== Choisissez un objet de recompense ===\n");
    for (int i = 0; i < max_item_pool_size; i++) {
        printf("%d. %s\n", i + 1, drawn_item[i].name);
    }
    int item_choice = cli_get_choice("Choisissez votre objet", 1, max_item_pool_size);
    return &drawn_item[item_choice - 1];
}

void cli_show_inventory_full(void)
{
    printf("\n=== Inventaire plein! ===\n");
}

// ========== SHOP INTERFACE IMPLEMENTATIONS ==========

void cli_display_shop(const char* shop_name, int player_gold, int refresh_cost,
                      const char** items, const int* prices, const int* stocks,
                      const int* rarities, const int* can_afford, int item_count) {
    printf("\n========================================\n");
    printf("| %s |\n", shop_name);
    printf("========================================\n");
    printf("Votre Or: %d\n", player_gold);
    printf("Cout de rafraichissement: %d\n", refresh_cost);
    printf("========================================\n\n");

    printf("Articles disponibles:\n");
    for (int i = 0; i < item_count; i++) {
        const char* rarity_str = "";
        switch(rarities[i]) {
            case 0: rarity_str = "[Commun]"; break;
            case 1: rarity_str = "[Peu commun]"; break;
            case 2: rarity_str = "[Rare]"; break;
            case 3: rarity_str = "[Epique]"; break;
            case 4: rarity_str = "[Legendaire]"; break;
        }

        printf(" %d. %-20s %s - %d or (Stock: %d)%s\n",
               i + 1,
               items[i],
               rarity_str,
               prices[i],
               stocks[i],
               can_afford[i] ? "" : " [Pas assez d'or]");
    }

    printf("\n========================================\n");
    printf("[A]cheter | [V]endre | [R]afraichir | [Q]uitter\n");
    printf("========================================\n");
}

void cli_show_purchase_success(const char* item_name, int price, int quantity) {
    printf("\n>> Achat reussi! <<\n");
    printf("Vous avez achete %s pour %d or", item_name, price);
    if (quantity > 0) {
        printf(" (x%d)", quantity);
    }
    printf("\n");
}

void cli_show_purchase_failed(const char* reason) {
    printf("\n>> Achat echoue: %s <<\n", reason);
}

void cli_show_sell_success(const char* item_name, int gold_received) {
    printf("\n>> Vente reussie! <<\n");
    printf("Vous avez vendu %s pour %d or\n", item_name, gold_received);
}

void cli_show_sell_failed(const char* reason) {
    printf("\n>> Vente echouee: %s <<\n", reason);
}

void cli_show_shop_refreshed(void) {
    printf("\n>> Boutique rafraichie! Nouveaux articles disponibles. <<\n");
}

void cli_show_refresh_failed(int cost, int player_gold) {
    printf("\n>> Rafraichissement echoue! <<\n");
    printf("Cout: %d or (Vous avez: %d or)\n", cost, player_gold);
}

void cli_show_discount_applied(int discount_percent) {
    printf("\n>> PROMOTION SPECIALE! <<\n");
    printf("Tous les articles a -%d%%!\n", discount_percent);
}

void cli_show_shop_restocked(void) {
    printf("\n>> Boutique reapprovisionnee! <<\n");
}

// ========== COMBAT LOOP SPECIFIC IMPLEMENTATIONS ==========

void cli_show_damage_calculation(int atk, int def, int raw) {
    printf("ATK %d - DEF %d = BRUT %d\n", atk, def, raw);
}

void cli_show_oxygen_stress(int oxygen_stress, int current, int max) {
    printf("Stress from attack: -%d oxygen (current: %d/%d)\n", oxygen_stress, current, max);
}

void cli_show_inventory_selection(Item* items, int item_count) {
    printf("\n=== Your Inventory ===\n");
    for (int i = 0; i < item_count; i++) {
        Item *item = &items[i];
        printf("%d. %s", i + 1, item->name);
        int is_on_cooldown = item_on_cooldown(item);
        if (is_on_cooldown && item->type == ITEM_WEAPON) {
            printf("[Cooldown]");
        }
        printf("\n");
    }
}

void cli_show_consumable_no_effect(const char* item_name) {
    printf("\n%s would have no effect right now! All stats are already maxed.\n", item_name);
}

void cli_show_weapon_actions(Item* weapon) {
    printf("\n=== %s Actions ===\n", weapon->name);
    for (int i = 0; i < weapon->action_count; i++) {
        Action *action = &weapon->actions[i];
        printf("%d. %s", i + 1, action->name);
        if (action->cooldown_remaining > 0) {
            printf(" [Cooldown: %d turns]", action->cooldown_remaining);
        }
        if (action->target_type == TARGET_OPPONENT) {
            printf(" (Applies damage to the enemy's stats)");
        } else if (action->target_type == TARGET_SELF) {
            printf(" (Boost your own stats!)");
        }
        printf("\n");
    }
}

void cli_show_auto_selection(const char* choice_name) {
    printf("(Choix-automatique: %s)\n", choice_name);
}

void cli_show_invalid_target(void) {
    printf("Cible invalide!\n");
}

void cli_show_creature_generation_error(void) {
    printf("Erreur: Impossible de generer les creatures!\n");
}

// ========== REWARD INTERFACE IMPLEMENTATIONS ==========

void cli_show_pearl_reward(int amount, int total) {
    printf("\n========================================\n");
    printf("Vous avez gagne %d perles!\n", amount);
    printf("Total: %d perles\n", total);
    printf("========================================\n");
}

void cli_show_reward_obtained(const char* item_name) {
    printf("\n========================================\n");
    printf("Recompense obtenue:\n");
    printf("%s\n", item_name ? item_name : "(inconnu)");
    printf("========================================\n");
}

void cli_show_inventory_replacement_prompt(const char* new_item_name) {
    printf("\n========================================\n");
    printf("Votre inventaire est plein.\n");
    printf("Remplacer un objet par: %s\n", new_item_name ? new_item_name : "(inconnu)");
    printf("Choisissez un emplacement a remplacer,\n");
    printf("ou le dernier numero pour annuler.\n");
    printf("========================================\n");
}

// ========== END OF IMPLEMENTATIONS ==========

// Define the CLI vtable instance
InterfaceVTable cli_interface = {
    .display_map = cli_display_map,
    .display_combat_state = cli_display_combat_state,
    .display_combat_intro = cli_display_combat_intro,
    .display_round_header = cli_display_round_header,
    .display_victory = cli_display_victory,
    .display_defeat = cli_display_defeat,
    .display_battle_start = cli_display_battle_start,
    .wait_for_enter = cli_wait_for_enter,
    .show_action = cli_show_action,
    .show_defeat_by = cli_show_defeat_by,
    .get_choice = cli_get_choice,
    .show_attack = cli_show_attack,
    .show_inventory = cli_show_inventory,
    .get_input = cli_get_input,
    .show_information = cli_show_information,
    .show_oxygen_consumed = cli_show_oxygen_consumed,
    .show_oxygen_critical = cli_show_oxygen_critical,
    .show_oxygen_death = cli_show_oxygen_death,
    .show_fatigue_status = cli_show_fatigue_status,
    .show_fatigue_increased = cli_show_fatigue_increased,
    .show_fatigue_recovered = cli_show_fatigue_recovered,
    .show_passive_oxygen = cli_show_passive_oxygen,
    .show_damage_dealt = cli_show_damage_dealt,
    .show_attack_blocked = cli_show_attack_blocked,
    .show_creature_defeated = cli_show_creature_defeated,
    .show_actions_taken = cli_show_actions_taken,
    .show_death_from_afflictions = cli_show_death_from_afflictions,
    .show_death_from_suffocation = cli_show_death_from_suffocation,
    .show_enemy_turn = cli_show_enemy_turn,
    .show_your_turn = cli_show_your_turn,
    .show_ending_turn = cli_show_ending_turn,
    .show_action_effect = cli_show_action_effect,
    .show_effect_error = cli_show_effect_error,
    .show_action_on_cooldown = cli_show_action_on_cooldown,
    .show_item_on_cooldown = cli_show_item_on_cooldown,
    .show_no_actions_available = cli_show_no_actions_available,
    .show_creature_died_from_effects = cli_show_creature_died_from_effects,
    .ask_item_choice_reward = cli_ask_item_choice_reward,
    .show_inventory_full = cli_show_inventory_full,
    .show_pearl_reward = cli_show_pearl_reward,
    .show_reward_obtained = cli_show_reward_obtained,
    .show_inventory_replacement_prompt = cli_show_inventory_replacement_prompt,
    .display_shop = cli_display_shop,
    .show_purchase_success = cli_show_purchase_success,
    .show_purchase_failed = cli_show_purchase_failed,
    .show_sell_success = cli_show_sell_success,
    .show_sell_failed = cli_show_sell_failed,
    .show_shop_refreshed = cli_show_shop_refreshed,
    .show_refresh_failed = cli_show_refresh_failed,
    .show_discount_applied = cli_show_discount_applied,
    .show_shop_restocked = cli_show_shop_restocked,
    .show_damage_calculation = cli_show_damage_calculation,
    .show_oxygen_stress = cli_show_oxygen_stress,
    .show_inventory_selection = cli_show_inventory_selection,
    .show_consumable_no_effect = cli_show_consumable_no_effect,
    .show_weapon_actions = cli_show_weapon_actions,
    .show_auto_selection = cli_show_auto_selection,
    .show_invalid_target = cli_show_invalid_target,
    .show_creature_generation_error = cli_show_creature_generation_error,
};