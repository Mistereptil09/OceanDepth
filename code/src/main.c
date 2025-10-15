#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "interface/interface_table.h"
#include "core/player.h"
#include "core/combat_loop.h"
#include "core/creature_data.h"

int main(void)
{
    srand(time(NULL));

    // Initialize creature templates
    init_creature_templates();

    // Select interface
    current_interface = select_interface();

    // Create player
    char player_name[30];
    current_interface->get_input("Please give me your name !", player_name, sizeof(player_name));
    Player* player = create_player(player_name, 100, 50
        , 100);

    if (!player) {
        printf("Error: Failed to create player!\n");
        cleanup_creature_templates();
        return 1;
    }

    // Set player attack (since default is 10, let's give them a fighting chance)
    player->base.attack.base_value = 15;

    printf("\n\nWelcome, %s, to the Ocean Depths!\n", player_name);
    printf("You are about to face the creatures of the deep...\n");

    // Start battle with EASY difficulty
    int result = battle_loop(player, EASY);

    if (result) {
        printf("\nCongratulations! You survived the depths!\n");
        printf("Final Stats:\n");
        printf("  HP: %d/%d\n", player->base.current_health_points, player->base.max_health_points);
        printf("  Pearls: %d\n", player->pearls);
    } else {
        printf("\nThe ocean has claimed another soul...\n");
    }

    // Clean up
    free_player(player);
    cleanup_creature_templates();

    return 0;
}
