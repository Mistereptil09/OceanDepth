//
// Created by cantonio on 02/10/2025.
//

#ifndef OCEANDEPTHS_MAP_H
#define OCEANDEPTHS_MAP_H
#include "player.h"

typedef enum {
    EMPTY = 0,
    SHOP,
    HEAL,
    SAVE,
    REEF,           // Easy fights
    CAVE,           // Rest areas
    SHIPWRECK,      // Medium fights
    PIT,          // Hard fights
    ABYSS,          // Boss fight
} CellType;

typedef struct {
    CellType type;
} Cell;

typedef struct {
    int rows;         //  depths (4 = Surface + 3 depths)
    int cols;         // width of each depth
    Cell** grid;
    int seed;         // seed used for generation
} Map;

// Map management
Map* create_map(int seed);
void free_map(Map* map);

// Cell access
Cell* get_cell(Map* map, int row, int col);

// Movement validation
int is_valid_move(Map* map, Player* player, Position to_position);


// Grid's depths
int generate_surface(Cell* cells, int size);
int generate_depth_1(Cell* cells, int seed, int size);
int generate_depth_2(Cell* cells, int seed, int size);
int generate_depth_3(Cell* cells, int seed, int size);

#endif //OCEANDEPTHS_MAP_H