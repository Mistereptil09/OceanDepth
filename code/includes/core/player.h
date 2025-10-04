//
// Created by Yasmine Slamani on 02/10/2025.
//
#ifndef OCEANDEPTH_PLAYER_H
#define OCEANDEPTH_PLAYER_H
#include "inventory.h"
#include "stats.h"
#include "effect.h"

#define MAX_FATIGUE 5

typedef struct {
    char *name;
    Stats stats;
    int oxygen_level;
    int max_oxygen_level;
    int fatigue_level;
    int pearls;
    Effect *active_effects;// loop through this
    Inventory inventory;
} Player;

void init_player(Player *p);
Player *create_player(char *name);
void free_player(Player *p);

void take_damage(Player *p, int damage); // HP
void consume_oxygen(Player *p, int amount); // OXYGEN
int increase_fatigue(Player *p, int amount); // FATIGUE
int recover_hp(Player *p, int hp);
int recover_oxygen(Player *p, int oxygen);
int recover_fatigue(Player *p, int fatigue);
void increase_pearls(Player *p, int amount);
int decrease_pearls(Player *p, int amount);

#endif //OCEANDEPTH_PLAYER_H