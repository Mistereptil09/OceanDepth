
//
// Created for OceanDepth
// Interface API Implementation - Data access layer for interfaces
//

#include "interface/interface_api.h"
#include "core/entity.h"
#include <stddef.h>

// Static combat state that can be accessed by interfaces
static CombatState current_combat_state = {0};

// ========== COMBAT STATE MANAGEMENT ==========

void set_combat_state(Player* player, Creature** creatures, int creature_count, int round) {
    current_combat_state.player = player;
    current_combat_state.creatures = creatures;
    current_combat_state.creature_count = creature_count;
    current_combat_state.round_number = round;

    // Count alive creatures
    int alive = 0;
    for (int i = 0; i < creature_count; i++) {
        if (creatures[i] && creatures[i]->base.is_alive) {
            alive++;
        }
    }
    current_combat_state.alive_count = alive;
}

const CombatState* get_combat_state(void) {
    return &current_combat_state;
}

int get_alive_creature_count(void) {
    return current_combat_state.alive_count;
}

Creature* get_alive_creature_at(int alive_index) {
    if (alive_index < 1 || !current_combat_state.creatures) {
        return NULL;
    }

    int current_alive = 0;
    for (int i = 0; i < current_combat_state.creature_count; i++) {
        if (current_combat_state.creatures[i] &&
            current_combat_state.creatures[i]->base.is_alive) {
            current_alive++;
            if (current_alive == alive_index) {
                return current_combat_state.creatures[i];
            }
        }
    }

    return NULL;
}

// ========== PLAYER QUERIES ==========

const char* get_player_name(Player* player) {
    return player ? player->base.name : "Unknown";
}

int get_player_hp(Player* player) {
    return player ? player->base.current_health_points : 0;
}

int get_player_max_hp(Player* player) {
    return player ? player->base.max_health_points : 0;
}

int get_player_oxygen(Player* player) {
    return player ? player->base.oxygen_level : 0;
}

int get_player_max_oxygen(Player* player) {
    return player ? player->base.max_oxygen_level : 0;
}

int get_player_attack(Player* player) {
    return player ? stat_get_value(&player->base.attack) : 0;
}

int get_player_defense(Player* player) {
    return player ? stat_get_value(&player->base.defense) : 0;
}

int get_player_pearls(Player* player) {
    return player ? player->pearls : 0;
}
int get_player_fatigue(Player* player) {
    return player ? player->base.fatigue_level : 0;
}

// ========== CREATURE QUERIES ==========

const char* get_creature_name(Creature* creature) {
    return creature ? creature->base.name : "Unknown";
}

int get_creature_hp(Creature* creature) {
    return creature ? creature->base.current_health_points : 0;
}

int get_creature_max_hp(Creature* creature) {
    return creature ? creature->base.max_health_points : 0;
}

int get_creature_attack(Creature* creature) {
    return creature ? stat_get_value(&creature->base.attack) : 0;
}

int get_creature_defense(Creature* creature) {
    return creature ? stat_get_value(&creature->base.defense) : 0;
}

int get_creature_speed(Creature* creature) {
    return creature ? stat_get_value(&creature->base.speed) : 0;
}

int get_creature_id(Creature* creature) {
    return creature ? creature->id : -1;
}

int is_creature_alive(Creature* creature) {
    return creature ? creature->base.is_alive : 0;
}

const char* get_creature_action_name(Creature* creature, int action_index) {
    if (!creature || action_index < 0 || action_index >= MAX_ACTIONS) {
        return NULL;
    }
    return creature->base.actions[action_index].name;
}

