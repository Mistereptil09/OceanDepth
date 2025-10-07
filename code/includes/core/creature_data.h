//
// Created by Yasmine Slamani on 07/10/2025.
//

#ifndef OCEANDEPTH_CREATURE_DATA_H
#define OCEANDEPTH_CREATURE_DATA_H

#include "creature.h"

/**
 * @brief Initializes all creature templates with their stats and actions
 * @note Must be called once at program startup before using creature templates
 */
void init_creature_templates(void);

/**
 * @brief Gets the creature templates array
 * @return Pointer to the creature templates array
 */
const CreatureTemplate* get_creature_templates(void);

/**
 * @brief Gets the number of creature templates
 * @return Number of templates in the array
 */
int get_creature_template_count(void);

// Action functions declarations
void tentacle_strike(void *user, void *target);
void ink_cloud(void *user, void *target);

void claw_pinch(void *user, void *target);
void shell_defense(void *user, void *target);

void tentacle_sting(void *user, void *target);
void electric_shock(void *user, void *target);

void bite_attack(void *user, void *target);
void blood_frenzy(void *user, void *target);

void ancient_bite(void *user, void *target);
void abyssal_roar(void *user, void *target);

void lure_attack(void *user, void *target);
void darkness_veil(void *user, void *target);

void sword_thrust(void *user, void *target);
void speed_boost(void *user, void *target);

void prehistoric_bite(void *user, void *target);
void apex_predator(void *user, void *target);

// Effect functions declarations
void poison_tick(void *target);
void poison_remove(void *target);
void paralysis_tick(void *target);
void paralysis_remove(void *target);
void defense_boost_tick(void *target);
void defense_boost_remove(void *target);
void attack_boost_tick(void *target);
void attack_boost_remove(void *target);
void speed_boost_tick(void *target);
void speed_boost_remove(void *target);
void bleed_tick(void *target);
void bleed_remove(void *target);

// Global templates array
static CreatureTemplate CREATURE_TEMPLATES[8];
static int templates_initialized = 0;

#endif //OCEANDEPTH_CREATURE_DATA_H