//
// Created by Yasmine Slamani on 07/11/2025.
//

#include "../../includes/core/game_init.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../../includes/core/combat_loop.h"
#include "../../includes/core/creature_data.h"
#include "../../includes/core/save_system.h"
#include "interface/interface_table.h"
#include "core/error_codes.h"

int game_init(void) {
    // Interface choice
    // Configuration choice : load from saved file OR generate new data (fresh player, fresh seed, fresh map)
    // While player is alive
    // Display map, offer to move (check if move is valid)
    // Enter = check celltype, check difficulty mapped to cell type, map->seed + (row * map->cols) + col or something
    // TO DO : UPDATE COMBAT_LOOP TO TAKE A SEED, this will influence generate_creatures.
    // TO DO : UPDATE COMBAT_LOOP TO RETURN AN INDICATION OF WIN VS LOSS SO THAT WE CAN UNLOCK A NEW CELL IF WIN
    // SHOP CELL : DISPLAY SHOP
    // SAVE CELL : SAVE IN A WHILE
    // HEAL CENTER : GET HP, O2 back up




}
