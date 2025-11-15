//
// Created by Yasmine Slamani on 02/10/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core/player.h"
#include "core/error_codes.h"
#include "core/entity.h"
/*
 */

Player *create_player(char *name, int max_hp, int base_defense, int max_oxygen, Position current, Position max) {
    Player *p = calloc(1,sizeof(Player));
    if (p == NULL) return NULL;

    p->base = create_entity_base(ENTITY_PLAYER, name, max_hp, base_defense, 0, 15);

    p->base.oxygen_level = max_oxygen;
    p->base.max_oxygen_level = max_oxygen;
    p->base.fatigue_level = 0;

    p->base.action_count = 0;

    p->pearls = 10;

    p->current_position = current;
    p->max_position = max;

    p->heal_uses_left = 2;  // Can use heal center 2 times per game
    p->has_used_cave = 0;   // Haven't used cave yet

    Inventory* inv = create_inventory();
    if (inv == NULL) {
        free(p);
        return NULL;
    }
    p->inventory = *inv;
    free(inv);
    return p;
}


void free_player(Player *p) {
    if (p == NULL) return;

    free_inventory(&p->inventory);

    free_entity_base(&p->base);

    free(p);
}

int consume_oxygen(Player *p, int amount) {
    if (p == NULL) return POINTER_NULL;
    p->base.oxygen_level -= amount;
    if (p->base.oxygen_level < 0) {
        p->base.oxygen_level = 0;
    }
    return SUCCESS;
}

int increase_fatigue(Player *p, int amount) {
    if (p == NULL) return POINTER_NULL;
    int new_value = p->base.fatigue_level + amount;
    if (new_value > MAX_FATIGUE) {
        p->base.fatigue_level = MAX_FATIGUE;
        return SUCCESS_SATURATED;
    }
    p->base.fatigue_level = new_value;
    return SUCCESS;
}


int recover_oxygen(Player *p, int oxygen) {
    if (p == NULL) return POINTER_NULL;
    int new_value = p->base.oxygen_level + oxygen;
    int max_oxygen = p->base.max_oxygen_level;
    if (new_value > max_oxygen) {
        p->base.oxygen_level = max_oxygen;
        return SUCCESS_SATURATED;
    }
    p->base.oxygen_level = new_value;
    return SUCCESS;
}

int recover_fatigue(Player *p, int fatigue) {
    if (p == NULL) return POINTER_NULL;
    int new_value = p->base.fatigue_level - fatigue;
    if (new_value < 0) {
        p->base.fatigue_level = 0;
        return SUCCESS_SATURATED;
    }
    p->base.fatigue_level = new_value;
    return SUCCESS;
}

int increase_pearls(Player *p, int amount) {
    if (p == NULL) return POINTER_NULL;
    p->pearls += amount;
    return SUCCESS;
}

int decrease_pearls(Player *p, int amount) {
    if (p == NULL) return POINTER_NULL;
    if (p->pearls < amount) {
        p->pearls = 0;
        return SUCCESS_SATURATED;
    }
    p->pearls -= amount;
    return SUCCESS;
}

void use_consumable(Player *p, Item *item) {
    if (p == NULL || item == NULL) return;
    if (item->type != ITEM_CONSUMABLE) return;
    if (item->quantity <= 0) return;

    printf("\nVous avez utilise %s!\n", item->name);

    if (item->oxygen_boost > 0) {
        int result = recover_oxygen(p, item->oxygen_boost);
        if (result == SUCCESS_SATURATED) {
            printf("Oxygene restaure au maximum (%d/%d)!\n", p->base.oxygen_level, p->base.max_oxygen_level);
        } else {
            printf("Oxygene recupere de %d! (%d/%d)\n", item->oxygen_boost, p->base.oxygen_level, p->base.max_oxygen_level);
        }
    }

    if (item->fatigue_relief > 0) {
        int result = recover_fatigue(p, item->fatigue_relief);
        if (result == SUCCESS_SATURATED) {
            printf("Fatigue completement recuperee!\n");
        } else {
            printf("Fatigue reduite de %d!\n", item->fatigue_relief);
        }
    }

    if (item->hp_boost > 0) {
        int result = entity_recover_hp(&p->base, item->hp_boost);
        if (result == SUCCESS_SATURATED) {
            printf("PV restaures au maximum (%d/%d)!\n", p->base.current_health_points, p->base.max_health_points);
        } else {
            printf("PV recuperes de %d! (%d/%d)\n", item->hp_boost, p->base.current_health_points, p->base.max_health_points);
        }
    }

    // decrement quantity
    item->quantity--;
    if (item->quantity == 0) {
        printf("Vous avez utilise votre dernier %s.\n", item->name);
    } else {
        printf("Restant: %d\n", item->quantity);
    }
}

int unlock_new_position(Player *player) {
    if (player == NULL) return POINTER_NULL;

    // player must be at the max_position to unlock
    if (player->current_position.row == player->max_position.row &&
        player->current_position.col == player->max_position.col) {

        if (player->max_position.col < 3) {
            player->max_position.col++;
            return NEW_CELL;
        } else if (player->max_position.row < 3) {
            player->max_position.row++;
            player->max_position.col = 0;
            return NEW_ROW;
        }
        return WIN;
    }

    return INVALID_INPUT;
}

int player_move_to(Player* player, Position new_position) {
    if (!player) return POINTER_NULL;
    player->current_position = new_position;
    return SUCCESS;
}