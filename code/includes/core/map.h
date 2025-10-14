//
// Created by natha on 11/10/2025.
//

#ifndef OCEANDEPTH_MAP_H
#define OCEANDEPTH_MAP_H

#endif //OCEANDEPTH_MAP_H

#include "../../includes/core/player.h"

typedef enum {
    SURFACE,
    REEF,
    CAVE,
    SHIPWRECK,
    SEAWEED,
    ABYSS,
} ZoneType;

typedef struct {
    ZoneType type;
    int depth;
    int ennemies;
    int safe;
    int shop;
    int boss;
    int treasure;
    int discovered;
    int require_equipment;
    int courant;
    int visibilite;
    int temperature;
} Cell;

typedef struct {
    int width, height;
    Cell **cells;
} Map;

typedef enum { UP, DOWN, LEFT, RIGHT } Direction;


int map_init(Map* map, int width, int height);
void free_map(Map* map);

Cell* cell_in_map(Map* map, int x, int y);

int cost_o2(int profondeur);
int substract_o2(Player player, Cell* destination);

int accessebility_cell(Cell* destination, PLayer player);

int player_movement(Map* map, Direction dir, Player player);