//
// Created by Yasmine Slamani on 07/10/2025.
//

#ifndef OCEANDEPTH_CREATURE_DATA_H
#define OCEANDEPTH_CREATURE_DATA_H

#include "creature.h"
#include "effect.h"

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


// Global templates array
static CreatureTemplate CREATURE_TEMPLATES[8];
static int templates_initialized = 0;

#endif //OCEANDEPTH_CREATURE_DATA_H