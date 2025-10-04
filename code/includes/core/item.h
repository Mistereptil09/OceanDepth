//
// Created by Yasmine Slamani on 02/10/2025.
//
#include "action.h"
#ifndef OCEANDEPTH_ITEM_H
#define OCEANDEPTH_ITEM_H

#endif //OCEANDEPTH_ITEM_H

typedef enum {
    ITEM_WEAPON,
    ITEM_CONSUMABLE,
    ITEM_OTHER,
} ItemType;

typedef struct {
    char *name;
    Action action;
    int max_uses;
    int current_uses;
} Item;