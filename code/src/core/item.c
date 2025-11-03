//
// Created by Yasmine Slamani on 02/10/2025.
//

#include "../../includes/core/item.h"

#include <stdlib.h>
#include <string.h>

#include "../../includes/core/error_codes.h"
#include "../../includes/core/effect.h"

Item create_item(char* name, ItemType type, Action* actions, int action_count, int quantity, int oxygen_boost, int fatigue_relief, int hp_boost, int price) {
    Item item = {0};
    strncpy(item.name, name, sizeof(item.name) - 1);
    item.name[sizeof(item.name) - 1] = '\0';

    item.type = type;
    item.actions = actions;
    item.action_count = action_count;
    item.quantity = quantity;
    item.oxygen_boost = oxygen_boost;
    item.fatigue_relief = fatigue_relief;
    item.hp_boost = hp_boost;
    item.price = price;

    return item;
}

int free_item(Item* item) {
    if (!item) return POINTER_NULL;

    /* Only free effect content within actions if actions exist.
       Do NOT free the actions array itself - caller owns that memory. */
    if (item->actions != NULL) {
        for (int i = 0; i < item->action_count; i++) {
            free_effect_content(&item->actions[i].effect);
        }
    }

    if (!item) return POINTER_NULL;
    for (int j = 0; j < item->action_count; j++) {
        if (item->actions[j].cooldown_remaining <= 0) {
            break;
        }
    }
    return 0;
}

int item_on_cooldown(Item* item) {
    if (!item) return POINTER_NULL;
    int is_on_cooldown = 1;
    for (int j = 0; j < item->action_count; j++) {
        if (item->actions[j].cooldown_remaining <= 0) {
            is_on_cooldown = 0;
            break;
        }
    }
    return is_on_cooldown;
}