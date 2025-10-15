#ifndef OCEANDEPTH_COMBAT_LOOP_H
#define OCEANDEPTH_COMBAT_LOOP_H

#include "entity.h"
#include "action.h"
#include "player.h"
#include "creature.h"

// Result of a combat step/loop
typedef enum {
    COMBAT_ONGOING = 0,
    COMBAT_A_WON = 1,
    COMBAT_B_WON = 2
} CombatResult;

/**
 * Compute the raw damage of a basic physical attack using attacker and defender stats.
 * damage = max(0, attacker.attack - defender.defense)
 */
int compute_physical_damage(EntityBase* attacker, EntityBase* defender);

/**
 * @brief Main battle loop - handles combat between player and multiple creatures
 * @param player Pointer to the player
 * @param difficulty Difficulty level to generate creatures
 * @return 1 if player won, 0 if player lost
 */
int battle_loop(Player* player, Difficulty difficulty);

#endif // OCEANDEPTH_COMBAT_LOOP_H
