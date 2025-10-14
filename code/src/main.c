#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "interface/interface_table.h"
#include "core/player.h"

int main(void)
{
    srand(time(NULL));
    current_interface = select_interface();
    char player_name[30];
    current_interface->get_input("Please give me your name !", player_name, sizeof(player_name));
    Player* player = create_player(player_name, 100, 10, 100);

    // TODO: Add game loop here

    // Clean up
    free_player(player);
    return 0;
}
