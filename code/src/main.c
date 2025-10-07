#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "interface/interface_table.h"
#include "core/player.h"

int main(void)
{
    srand(time(NULL));
    printf("Hello, World!\n");
    current_interface = select_interface();

    Player* player = create_player("Player1");
    current_interface->show_inventory(&player->inventory);
    Creature* creature = create_from_template(1, 1);
    strcpy(creature->base.name, "méchant pas beau"); // pour des utilités de test

    current_interface->show_attack(&creature->base, &player->base, 10);

    return 0;
}
