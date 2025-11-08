//
// Created by Yasmine Slamani on 04/11/2025.
//
#include <stdlib.h>
#include <stdio.h>
#include "core/map.h"
#include "core/player.h"
#include "core/error_codes.h"


Map* create_map(int seed) {
    Map* map = malloc(sizeof(Map));
    if (!map) return NULL;
    int rows = 4, cols = 4;
    map->rows = rows;
    map->cols = cols;
    map->seed = seed;
    Cell **cells = malloc(sizeof(Cell *) * rows);
    for (int i = 0; i < rows; i++) {
        cells[i] = calloc(cols, sizeof(Cell));
    }
    map->grid = cells;
    // surface
    generate_surface(cells[0], cols);
    // depth 1
    generate_depth_1(cells[1], seed, cols);
    // depth 2
    generate_depth_2(cells[2], seed, cols);
    // depth 3
    generate_depth_3(cells[3], seed, cols);

    return map;
}

int generate_surface(Cell* cells, int size) {
    if (size != 4) return INVALID_INPUT;
    cells[0].type = SHOP;
    cells[1].type = HEAL;
    cells[2].type = SAVE;
    cells[3].type = EMPTY;
    return SUCCESS;
}
int generate_depth_1(Cell* cells, int seed, int size) {
    if (size != 4) return INVALID_INPUT;
    cells[3].type = CAVE;

    CellType types[3] = {REEF, REEF, SHIPWRECK};

    srand(seed + 1);
    for (int i = 2; i > 0; i--) {
        int j = rand() % (i + 1);
        CellType temp = types[i];
        types[i] = types[j];
        types[j] = temp;
    }

    for (int i = 0; i < 3; i++) {
        cells[i].type = types[i];
    }

    return SUCCESS;

}
int generate_depth_2(Cell* cells, int seed, int size) {
    if (size != 4) return INVALID_INPUT;

    CellType types[4] = {SHIPWRECK, SHIPWRECK, REEF, PIT};

    srand(seed + 2);
    for (int i = 3; i > 0; i--) {
        int j = rand() % (i + 1);
        CellType temp = types[i];
        types[i] = types[j];
        types[j] = temp;
    }

    for (int i = 0; i < 4; i++) {
        cells[i].type = types[i];
    }

    return SUCCESS;
}
int generate_depth_3(Cell* cells, int seed, int size) {
    if (size != 4) return INVALID_INPUT;
    cells[0].type = CAVE;
    cells[1].type = cells[2].type = PIT;
    cells[3].type = ABYSS;

    return SUCCESS;
}

void free_map(Map* map) {
    if (!map) return;
    for (int i = 0; i < map->rows; i++) {
        free(map->grid[i]);
    }
    free(map->grid);
    free(map);
}

Cell* get_cell(Map* map, int row, int col) {
    if (!map) return NULL;
    if (row < 0 || row >= map->rows) return NULL;
    if (col < 0 || col >= map->cols) return NULL;

    return &map->grid[row][col];
}

int is_valid_move(Map* map, Player* player, Position to_position) {
    if (!map) return 0;
    Cell* cell = get_cell(map, to_position.row, to_position.col);
    if (!cell) return 0;

    int is_unlocked = (to_position.row < player->max_position.row) ||
                      (to_position.row == player->max_position.row &&
                       to_position.col <= player->max_position.col);
    if (!is_unlocked) return 0;

    int is_adjacent = (player->current_position.col == to_position.col &&
                       abs(player->current_position.row - to_position.row) == 1) ||
                      (player->current_position.row == to_position.row &&
                       abs(player->current_position.col - to_position.col) == 1);

    return is_adjacent;
}

