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

/**
 * @brief Cleans up all allocated memory in creature templates (effects' display_messages)
 * @note Should be called at program shutdown to free all effect display messages
 */
void cleanup_creature_templates(void);


int random_range(int min, int max);

#endif //OCEANDEPTH_CREATURE_DATA_H