//
// Created by Yasmine Slamani on 02/10/2025.
//
#ifndef OCEANDEPTH_ITEM_H
#define OCEANDEPTH_ITEM_H
#include "action.h"

typedef enum {
    ITEM_WEAPON,
    ITEM_CONSUMABLE
} ItemType;

typedef struct {
    char name[30];
    ItemType type;

    Action* actions;
    int action_count;

    int quantity;
    int oxygen_boost;
    int fatigue_relief;
    int hp_boost;

    int price;
} Item;

Item create_item(char* name, ItemType type, Action* actions, int action_count, int quantity, int oxygen_boost, int fatigue_relief, int hp_boost, int price);
int free_item(Item* item);
int item_on_cooldown(Item* item);

#endif //OCEANDEPTH_ITEM_H