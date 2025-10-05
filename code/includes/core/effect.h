//
// Created by Yasmine Slamani on 04/10/2025.
//

#ifndef OCEANDEPTH_EFFECT_H
#define OCEANDEPTH_EFFECT_H


typedef enum {
    EFFECT_POISON,
    EFFECT_PARALYSIS,
    EFFECT_RAGE,
    EFFECT_SHIELD,
    EFFECT_HEAL_POISON,
    EFFECT_HEAL_PARALYSIS,
} TypeEffect;

typedef void (*FonctionEffect)(void *target);

typedef struct {
    TypeEffect type;
    char name[30];
    int turns_left;

    FonctionEffect on_tick;
    FonctionEffect on_remove; // Removes effect from target effect array/ is active = 0

    // DAMAGE
    int hp_cost;
    int oxygen_cost;
    int defense_cost;

    // HEALTH BOOST
    int attack_boost;
    int defense_boost;
    int oxygen_boost;
    int speed_boost;

    int is_active;
} Effect;

// create effect
// free effect

#endif //OCEANDEPTH_EFFECT_H