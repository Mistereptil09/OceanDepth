//
// Created by Yasmine Slamani on 02/10/2025.
//
#ifndef OCEANDEPTH_ITEM_H
#define OCEANDEPTH_ITEM_H
#include "action.h"

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

// create
// free
// use
#endif //OCEANDEPTH_ITEM_H
