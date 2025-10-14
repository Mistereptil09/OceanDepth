//
// Created by cantonio on 02/10/2025.
//
#include <stdio.h>
#include <stdlib.h>

#include "interface/interface_cli.h"

#include <string.h>

#include "core/map.h"
#include "core/player.h"
#include "core/creature.h"
#include "helper/input_validator.h"

void cli_display_map(Map* map)
{
    printf("Map\n");
}

void cli_display_combat(Player* player, Creature* creatures, int count)
{
    printf("Display combat\n");
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
    .display_combat = cli_display_combat,
    .get_choice = cli_get_choice,
    .show_attack = cli_show_attack,
    .show_inventory = cli_show_inventory,
    .get_input = cli_get_input,
};