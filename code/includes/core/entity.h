//
// Created by Yasmine Slamani on 02/10/2025.
//

#ifndef OCEANDEPTH_STATS_H
#define OCEANDEPTH_STATS_H

#include "core/effect.h"

typedef enum {
    ENTITY_PLAYER,
    ENTITY_CREATURE
} EntityType;

typedef struct EntityBase{
    EntityType type;
    char name[30];

    int base_attack;
    int current_attack;

    int base_defense;
    int current_defense;

    int max_health_points;
    int current_health_points;

    int speed;

    // possibly add effects list here ?
    int effects_number;
    Effect* effects;

    int is_alive;
} EntityBase;

// create stats
// free stats
#endif //OCEANDEPTH_STATS_H
