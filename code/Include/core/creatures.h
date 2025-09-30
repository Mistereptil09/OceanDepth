//
// Created by cantonio on 30/09/2025.
//

#ifndef OCEANDEPTHS_CREATURES_H
#define OCEANDEPTHS_CREATURES_H
#include "effects.h"

typedef struct SeaCreature {
    int id;
    char name[30];

    // Base stats
    int max_health;
    int health;
    int min_attack;
    int max_attack;
    int defense;
    int speed;

    // Effective stats for multiplicators/ modificators etc...
    int effective_attack;
    int effective_defense;
    int effective_speed;

    // Special effect data
    EffectType effect_type;
    int effect_chance;        // chance to apply effect in %
    int effect_power;  // a number to modify the potency of said effect

    // Current active effects
    Effect effets_actives[10];  // Max 10 simultaneous effects is an array of struct (we copy the effect)
    int nb_effets;

    int is_alive;

} SeaCreature;
#endif //OCEANDEPTHS_CREATURES_H