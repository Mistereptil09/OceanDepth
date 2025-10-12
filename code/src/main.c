#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "interface/interface_table.h"
#include "core/player.h"
#include "core/game_loop.h"

int main(void)
{
    srand(time(NULL));
    current_interface = select_interface();
    char player_name[30];
    current_interface->get_input("Please give me your name !", player_name, sizeof(player_name));
    Player* player = create_player(player_name, 100, 10, 100);

    game_loop(player);
    return 0;
}
