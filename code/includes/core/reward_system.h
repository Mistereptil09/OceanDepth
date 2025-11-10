// Created by anton on 11/2/2025.
#ifndef OCEANDEPTH_REWARD_SYSTEM_H
#define OCEANDEPTH_REWARD_SYSTEM_H

#include "player.h"
#include "creature.h" // for Difficulty enum

/**
 * @brief Orchestrates post-battle reward selection and inventory handling.
 * Uses the Item loot pool and the active interface to present choices.
 *
 * @param player Pointer to the player
 * @param difficulty Difficulty of the battle, affects number of options
 */
void award_post_battle_rewards(Player* player, Difficulty difficulty);

#endif // OCEANDEPTH_REWARD_SYSTEM_H

