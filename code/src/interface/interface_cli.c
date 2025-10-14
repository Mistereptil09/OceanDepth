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

void cli_display_map(Map* map)
{
    printf("Map\n");
}

void cli_display_combat_state(void)
{
    // Query combat state from API
    const CombatState* state = get_combat_state();
    if (!state || !state->player) return;

    // Display player status
    printf("\n========================================\n");
    printf("|     YOUR STATUS                      |\n");
    printf("========================================\n");
    printf("Name: %s\n", get_player_name(state->player));
    printf("HP: %d/%d\n", get_player_hp(state->player), get_player_max_hp(state->player));
    printf("Oxygen: %d/%d\n", get_player_oxygen(state->player), get_player_max_oxygen(state->player));
    printf("Attack: %d\n", get_player_attack(state->player));
    printf("Defense: %d\n", get_player_defense(state->player));
    printf("Pearls: %d\n", get_player_pearls(state->player));
    printf("==================\n\n");

    // Display alive enemies
    printf("Enemies remaining:\n");
    int alive_count = get_alive_creature_count();
    for (int i = 1; i <= alive_count; i++) {
        Creature* creature = get_alive_creature_at(i);
        if (creature) {
            printf("  %d. %s - HP: %d/%d\n",
                   i,
                   get_creature_name(creature),
                   get_creature_hp(creature),
                   get_creature_max_hp(creature));
        }
    }
    printf("\n");
}

void cli_display_combat_intro(Creature** creatures, int count)
{
    printf("========================================\n");
    printf("|     ENEMIES APPROACHING!             |\n");
    printf("========================================\n");

    for (int i = 0; i < count; i++) {
        if (creatures[i] && is_creature_alive(creatures[i])) {
            printf("\n=== %s (ID: %d) ===\n",
                   get_creature_name(creatures[i]),
                   get_creature_id(creatures[i]));
            printf("HP: %d/%d\n",
                   get_creature_hp(creatures[i]),
                   get_creature_max_hp(creatures[i]));
            printf("Attack: %d\n", get_creature_attack(creatures[i]));
            printf("Defense: %d\n", get_creature_defense(creatures[i]));
            printf("Speed: %d\n", get_creature_speed(creatures[i]));
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
    printf("        ROUND %d\n", round_number);
    printf("========================================\n");
}

void cli_display_victory(void)
{
    printf("\n\n");
    printf("========================================\n");
    printf("|                                      |\n");
    printf("|      VICTORY!                        |\n");
    printf("|                                      |\n");
    printf("========================================\n");
}

void cli_display_defeat(void)
{
    printf("\n\n");
    printf("========================================\n");
    printf("|                                      |\n");
    printf("|      GAME OVER                       |\n");
    printf("|                                      |\n");
    printf("========================================\n");
}

void cli_display_battle_start(void)
{
    printf("\n\n");
    printf("========================================\n");
    printf("|                                      |\n");
    printf("|       BATTLE STARTING!               |\n");
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
    printf("\n%s uses %s!\n", entity_name, action_name);
}

void cli_show_defeat_by(const char* enemy_name)
{
    printf("\n>> You have been defeated by %s! <<\n", enemy_name);
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
            printf("Please enter a number between %d and %d.\n", min, max);
        } else {
            printf("Invalid input. Please try again.\n");
        }
    }
}
void cli_show_attack(EntityBase* attacker, EntityBase* target, int damage)
{
    printf("%s attacks %s for %d damage!\n", attacker->name, target->name, damage);
}

void cli_show_inventory(Inventory* inv)
{
    printf("Inventory!\n");
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
        printf("Invalid input. Please try again.\n");
    }
}

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
    .get_choice = cli_get_choice,
    .show_attack = cli_show_attack,
    .show_inventory = cli_show_inventory,
    .get_input = cli_get_input,
    .show_action = cli_show_action,
    .show_defeat_by = cli_show_defeat_by,
};