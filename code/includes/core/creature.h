//
// Created by Yasmine Slamani on 03/10/2025.
//

#ifndef OCEANDEPTH_CREATURE_H
#define OCEANDEPTH_CREATURE_H
#include "effect.h"
#include "stats.h"

#endif //OCEANDEPTH_CREATURE_H

typedef enum {
    KRAKEN,
    KING_GRAB,
    JELLYFISH,
    SHARK,
    LEVIATHAN,
    ANGLERFISH,
    SWORDFISH
} CreatureType;

typedef struct {
    int id;
    CreatureType type;
    Stats stats;
    int is_alive;
    Effect *active_effects;
    Effect *creature_effects;
} Creature;