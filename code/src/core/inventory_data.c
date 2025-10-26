//
// Created by Yasmine Slamani on 26/10/2025.
//

#include "core/inventory_data.h"
#include "core/effect.h"
#include <stdlib.h>
#include <string.h>

Item create_epee() {
    Item epee = {0};
    strcpy(epee.name, "Épée");
    epee.type = ITEM_WEAPON;
    epee.price = 0;

    // Allocate 2 actions
    epee.actions = malloc(sizeof(Action) * 2);
    epee.action_count = 2;

    // Action 0: Coup d'épée (same as player.c actions[0])
    epee.actions[0] = (Action){
        .type = PHYSICAL_ATTACK,
        .target_type = TARGET_SELF,
        .cooldown_turns = 0,
        .cooldown_remaining = 0,
        .effect = create_effect(
            "Boost de l'épée",
            "+10 points d'attaque pour toi",
            1,
            10, 0, 0, 0, 0,  // flat: +10 attack
            0, 0, 0, 0, 0,   // percent: none
            NULL, 0          // no special effect, applies this turn
        )
    };
    strcpy(epee.actions[0].name, "Coup d'épée");

    // Action 1: Boost d'attaque (same as player.c actions[1])
    epee.actions[1] = (Action){
        .type = PHYSICAL_ATTACK,
        .target_type = TARGET_SELF,
        .cooldown_turns = 2,
        .cooldown_remaining = 0,
        .effect = create_effect(
            "Boost d'attaque",
            "Montée de puissance de 0.4 pour toi!",
            1,
            0, 0, 0, 0, 0,     // flat: none
            0.4, 0, 0, 0, 0,   // percent: +40% attack
            NULL, 0
        )
    };
    strcpy(epee.actions[1].name, "Boost d'attaque");

    return epee;
}

Item create_harpon() {
    Item harpon = {0};
    strcpy(harpon.name, "Harpon");
    harpon.type = ITEM_WEAPON;
    harpon.price = 0;

    // Allocate 1 action
    harpon.actions = malloc(sizeof(Action) * 1);
    harpon.action_count = 1;

    // Action 0: Harpon (same as player.c actions[3])
    harpon.actions[0] = (Action){
        .type = PHYSICAL_ATTACK,
        .target_type = TARGET_OPPONENT,
        .cooldown_turns = 2,
        .cooldown_remaining = 0,
        .effect = create_effect(
            "Malus adversaire",
            "Ton adversaire est affaibli, sa défense est de -3 pour les 3 prochains tours!",
            3,
            0, -3, 0, 0, 0,    // flat: -3 defense
            0, 0, 0, 0, 0,     // percent: none
            NULL, 0
        )
    };
    strcpy(harpon.actions[0].name, "Harpon");

    return harpon;
}

Item create_combinaison() {
    Item combinaison = {0};
    strcpy(combinaison.name, "Combinaison");
    combinaison.type = ITEM_WEAPON;
    combinaison.price = 0;

    // Allocate 1 action
    combinaison.actions = malloc(sizeof(Action) * 1);
    combinaison.action_count = 1;

    // Action 0: Boost de défense (same as player.c actions[2])
    combinaison.actions[0] = (Action){
        .type = SPECIAL_SKILL,
        .target_type = TARGET_SELF,
        .cooldown_turns = 2,
        .cooldown_remaining = 0,
        .effect = create_effect(
            "Boost de défense",
            "+ 10 de défense pour toi !",
            2,
            0, 10, 0, 0, 0,    // flat: +10 defense
            0, 0, 0, 0, 0,     // percent: none
            NULL, 1            // applies next turn
        )
    };
    strcpy(combinaison.actions[0].name, "Boost de défense");

    return combinaison;
}

Item create_oxygen_capsule(int quantity) {
    Item capsule = {0};
    strcpy(capsule.name, "Capsule O2");
    capsule.type = ITEM_CONSUMABLE;
    capsule.price = 10;

    capsule.quantity = quantity;
    capsule.oxygen_boost = 40;
    capsule.fatigue_relief = 0;
    capsule.hp_boost = 0;

    capsule.actions = NULL;
    capsule.action_count = 0;

    return capsule;
}

Item create_fatigue_stimulant(int quantity) {
    Item stimulant = {0};
    strcpy(stimulant.name, "Stimulant Marin");
    stimulant.type = ITEM_CONSUMABLE;
    stimulant.price = 10;

    stimulant.quantity = quantity;
    stimulant.oxygen_boost = 0;
    stimulant.fatigue_relief = 2;
    stimulant.hp_boost = 0;

    stimulant.actions = NULL;
    stimulant.action_count = 0;

    return stimulant;
}

Item create_health_kit(int quantity) {
    Item kit = {0};
    strcpy(kit.name, "Trousse de soin");
    kit.type = ITEM_CONSUMABLE;
    kit.price = 15;

    kit.quantity = quantity;
    kit.oxygen_boost = 0;
    kit.fatigue_relief = 0;
    kit.hp_boost = 25;

    kit.actions = NULL;
    kit.action_count = 0;

    return kit;
}
