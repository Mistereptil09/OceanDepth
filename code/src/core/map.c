//
// Created by natha on 11/10/2025.
//

#include "../../includes/core/map.h"
#include "../../includes/core/player.h"
#include <stdio.h>
#include <stdlib.h>

int map_init(Map* map, int width, int height) {
    map->width = width;
    map->height = height;

    map->cells = (Cell**)malloc(height * sizeof(Cell *));
    if (map->cells == NULL) {
        fprintf(stderr, "map_init: failed to allocate memory for map cells\n");
        return -1;
    }

    for (int i = 0; i < height; i++) {
        map->cells[i] = (Cell*)malloc(width * sizeof(Cell));

        if (map->cells[i] == NULL) {
            fprintf(stderr, "map_init: failed to allocate memory for map cells[i]\n");
            for (int j = 0; j < i; j++) {
                free(map->cells[j]);
            }
            free(map->cells);
            return -1;
        }
    }

    return 0;
}

void free_map(Map* map){
    if (map->cells == NULL)
        return;

    for (int j = 0; j < map->height; j++) {
        free(map->cells[j]);
    }
    free(map->cells);
}

Cell* cell_in_map(Map* map, int x, int y) {
    if (x < 0) {
        return 0;
    }
    if (y < 0) {
        return 0;
    }
    if (x >= map->width) {
        return 0;
    }
    if (y >= map->height) {
        return 0;
    }
    return &map->cells[y][x];
}

int cost_o2(int profondeur) {
    if (profondeur <= 50) {
        return 1;
    }
    if (profondeur <= 150) {
        return 2;
    }
    if (profondeur <= 300) {
        return 4;
    }
    return 5;
}

int substract_o2(Player player, Cell* destination) {
    int cost = cost_o2(destination->depth);
    if (player.o2 -= cost < 0) {
        return -1;
    }
    return player.o2 - cost;

}

int accessebility_cell(Cell* destination, Player player) {
    if (player.inventory[0] < destination->require_equipment) {
        return 0;
    }
    return 1;
}

int player_movement(Map* map, Direction dir, Player player) {

    int x = player.position.x;
    int y = player.position.y;

    if (dir == LEFT)
        x--;
    else if (dir == RIGHT)
        x++;
    else if (dir == DOWN)
        y++;
    else if (dir == UP)
        y--;

    if (!cell_in_map(map, x, y)) {
        return -1;
    }

    Cell* destination = cell_in_map(map, x, y);

    if (!accessebility_cell(destination, player)) {
        return -2;
    }

    substract_o2(player, destination);

    player.position.x = x;
    player.position.y = y;

    return 0;
}