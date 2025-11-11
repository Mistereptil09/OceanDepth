//
// Created by Yasmine Slamani on 26/10/2025.
//

#include "core/inventory_data.h"
#include "core/item.h"
#include "core/effect.h"
#include <stdlib.h>
#include <string.h>

Item create_epee() {
    Action* actions = malloc(sizeof(Action) * 3);

    actions[0] = (Action){
        .type = PHYSICAL_ATTACK,
        .target_type = TARGET_SELF,
        .cooldown_turns = 0,
        .cooldown_remaining = 0,
        .effect = create_effect(
            "Boost de l'epee",
            "+15 points d'attaque pour toi",
            1,
            15, 0, 0, 0, 0,  // flat: +10 attack
            0, 0, 0, 0, 0,   // percent: none
            NULL, 0
        )
    };
    strcpy(actions[0].name, "Coup d'epee");

    actions[1] = (Action){
        .type = PHYSICAL_ATTACK,
        .target_type = TARGET_SELF,
        .cooldown_turns = 2,
        .cooldown_remaining = 0,
        .effect = create_effect(
            "Boost d'attaque",
            "Montee de puissance de 0.5 pour toi",
            1,
            0, 0, 0, 0, 0,     // flat: none
            0.5, 0, 0, 0, 0,   // percent: +40% attack
            NULL, 0
        )
    };

    strcpy(actions[1].name, "Boost d'attaque");

    actions[2] = (Action){
        .type = PHYSICAL_ATTACK,
        .target_type = TARGET_SELF,
        .cooldown_turns = 5,
        .cooldown_remaining = 0,
        .effect = create_effect(
            "Lames ultra tranchantes",
            "Triple ton attaque",
            1,
            0, 0, 0, 0, 0,     // flat: none
            2, 0, 0, 0, 0,   // percent: +40% attack
            NULL, 0
        )
    };

    strcpy(actions[2].name, "Lames ultra tranchantes");

    return create_item("epee", ITEM_WEAPON, actions, 3, 0, 0, 0, 0, 0);
}

Item create_harpon() {
    Action* actions = malloc(sizeof(Action) * 2);

    actions[0] = (Action){
        .type = PHYSICAL_ATTACK,
        .target_type = TARGET_OPPONENT,
        .cooldown_turns = 2,
        .cooldown_remaining = 0,
        .effect = create_effect(
            "Malus adversaire defense",
            "Ton adversaire est affaibli, sa defense est de -8 pour les 3 prochains tours!",
            3,
            0, -8, 0, 0, 0,
            0, 0, 0, 0, 0,
            NULL, 0
        )
    };
    strcpy(actions[0].name, "Harpon malus défense");

    actions[1] = (Action){
        .type = PHYSICAL_ATTACK,
        .target_type = TARGET_OPPONENT,
        .cooldown_turns = 2,
        .cooldown_remaining = 0,
        .effect = create_effect(
            "Malus adversaire attaque",
            "Ton adversaire est affaibli, son attaque est de -10 pour les 2 prochains tours!",
            2,
            0, -10, 0, 0, 0,
            0, 0, 0, 0, 0,
            NULL, 0
        )
    };
    strcpy(actions[1].name, "Harpon malus attaque");

    return create_item("Harpon", ITEM_WEAPON, actions, 2, 0, 0, 0, 0, 0);
}

Item create_combinaison() {
    Action* actions = malloc(sizeof(Action) * 2);

    actions[0] = (Action){
        .type = SPECIAL_SKILL,
        .target_type = TARGET_SELF,
        .cooldown_turns = 2,
        .cooldown_remaining = 0,
        .effect = create_effect(
            "Boost de defense",
            "+ 10 de defense pour toi !",
            2,
            0, 10, 0, 0, 0,
            0, 0, 0, 0, 0,
            NULL, 1
        )
    };
    strcpy(actions[0].name, "Boost de defense 2 tours");

    actions[1] = (Action){
        .type = SPECIAL_SKILL,
        .target_type = TARGET_SELF,
        .cooldown_turns = 4,
        .cooldown_remaining = 0,
        .effect = create_effect(
            "Boost de defense",
            "+8 de defense pour toi sur 3 tours !",
            3,
            0, 8, 0, 0, 0,
            0, 0, 0, 0, 0,
            NULL, 1
        )
    };
    strcpy(actions[1].name, "Boost de defense 3 tours");

    return create_item("Combinaison", ITEM_WEAPON, actions, 2, 0, 0, 0, 0, 0);
}

Item create_oxygen_capsule(int quantity) {
    return create_item("Capsule O2", ITEM_CONSUMABLE, NULL, 0, quantity, 40, 0, 5, 10);
}

Item create_fatigue_stimulant(int quantity) {
    return create_item("Stimulant Marin", ITEM_CONSUMABLE, NULL, 0, quantity, 5, 2, 0, 10);
}

Item create_health_kit(int quantity) {
    return create_item("Trousse de soin", ITEM_CONSUMABLE, NULL, 0, quantity, 5, 0, 25, 15);
}

Item* lookup_item_by_name(const char* name, int quantity) {
    if (!name) return NULL;

    Item* item = malloc(sizeof(Item));
    if (!item) return NULL;

    // Lookup weapons
    if (strcmp(name, "epee") == 0) {
        *item = create_epee();
    } else if (strcmp(name, "Harpon") == 0) {
        *item = create_harpon();
    } else if (strcmp(name, "Combinaison") == 0) {
        *item = create_combinaison();
    }
    // Lookup consumables
    else if (strcmp(name, "Capsule O2") == 0) {
        *item = create_oxygen_capsule(quantity);
    } else if (strcmp(name, "Stimulant Marin") == 0) {
        *item = create_fatigue_stimulant(quantity);
    } else if (strcmp(name, "Trousse de soin") == 0) {
        *item = create_health_kit(quantity);
    }
    // Unknown item
    else {
        free(item);
        return NULL;
    }

    return item;
}
