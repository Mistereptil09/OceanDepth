//
// Created by anton on 11/2/2025.
//

#include "core/item_data.h"
#include "core/item.h"
#include "core/effect.h"
#include <stdlib.h>
#include <string.h>

// ========== WEAPONS ==========

Item create_trident(void) {
    Action* actions = malloc(sizeof(Action) * 2);

    actions[0] = (Action){
            .type = PHYSICAL_ATTACK,
            .target_type = TARGET_OPPONENT,
            .cooldown_turns = 0,
            .cooldown_remaining = 0,
            .effect = create_effect(
                    "Triple frappe",
                    "Triple coup qui inflige 15 de degats!",
                    1,
                    15, 0, 0, 0, 0,
                    0, 0, 0, 0, 0,
                    NULL, 0
            )
    };
    strcpy(actions[0].name, "Triple frappe");

    actions[1] = (Action){
            .type = PHYSICAL_ATTACK,
            .target_type = TARGET_OPPONENT,
            .cooldown_turns = 3,
            .cooldown_remaining = 0,
            .effect = create_effect(
                    "Empalement",
                    "Degats massifs! -20 PV adversaire sur 2 tours",
                    2,
                    0, 0, -20, 0, 0,
                    0, 0, 0, 0, 0,
                    NULL, 0
            )
    };
    strcpy(actions[1].name, "Empalement");

    return create_item("Trident", ITEM_WEAPON, actions, 2, 0, 0, 0, 0, 50);
}

Item create_sonic_blade(void) {
    Action* actions = malloc(sizeof(Action) * 2);

    actions[0] = (Action){
            .type = SPECIAL_SKILL,
            .target_type = TARGET_SELF,
            .cooldown_turns = 1,
            .cooldown_remaining = 0,
            .effect = create_effect(
                    "Onde sonique",
                    "+15 attaque temporaire",
                    2,
                    15, 0, 0, 0, 0,
                    0, 0, 0, 0, 0,
                    NULL, 0
            )
    };
    strcpy(actions[0].name, "Onde sonique");

    actions[1] = (Action){
            .type = PHYSICAL_ATTACK,
            .target_type = TARGET_OPPONENT,
            .cooldown_turns = 4,
            .cooldown_remaining = 0,
            .effect = create_effect(
                    "Detonation",
                    "Explosion sonique! -25 PV adversaire et -5 defense",
                    3,
                    0, -5, -25, 0, 0,
                    0, 0, 0, 0, 0,
                    NULL, 0
            )
    };
    strcpy(actions[1].name, "Detonation");

    return create_item("Lame Sonique", ITEM_WEAPON, actions, 2, 0, 0, 0, 0, 45);
}

Item create_coral_staff(void) {
    Action* actions = malloc(sizeof(Action) * 2);

    actions[0] = (Action){
            .type = SPECIAL_SKILL,
            .target_type = TARGET_SELF,
            .cooldown_turns = 2,
            .cooldown_remaining = 0,
            .effect = create_effect(
                    "Regeneration corail",
                    "+15 PV de regeneration",
                    3,
                    0, 0, 15, 0, 0,
                    0, 0, 0, 0, 0,
                    NULL, 1
            )
    };
    strcpy(actions[0].name, "Regeneration");

    actions[1] = (Action){
            .type = SPECIAL_SKILL,
            .target_type = TARGET_OPPONENT,
            .cooldown_turns = 3,
            .cooldown_remaining = 0,
            .effect = create_effect(
                    "Poison corail",
                    "Empoisonne l'adversaire! -10 PV par tour",
                    4,
                    0, 0, -10, 0, 0,
                    0, 0, 0, 0, 0,
                    NULL, 0
            )
    };
    strcpy(actions[1].name, "Poison corail");

    return create_item("Baton de Corail", ITEM_WEAPON, actions, 2, 0, 0, 0, 0, 65);
}

Item create_abyssal_hammer(void) {
    Action* actions = malloc(sizeof(Action) * 1);

    actions[0] = (Action){
            .type = PHYSICAL_ATTACK,
            .target_type = TARGET_OPPONENT,
            .cooldown_turns = 5,
            .cooldown_remaining = 0,
            .effect = create_effect(
                    "Ecrasement abyssal",
                    "Coup devastateur! -35 PV et -10 attaque adversaire",
                    3,
                    -10, 0, -35, 0, 0,
                    0, 0, 0, 0, 0,
                    NULL, 0
            )
    };
    strcpy(actions[0].name, "Ecrasement");

    return create_item("Marteau Abyssal", ITEM_WEAPON, actions, 1, 0, 0, 0, 0, 35);
}

Item create_net_launcher(void) {
    Action* actions = malloc(sizeof(Action) * 1);

    actions[0] = (Action){
            .type = SPECIAL_SKILL,
            .target_type = TARGET_OPPONENT,
            .cooldown_turns = 3,
            .cooldown_remaining = 0,
            .effect = create_effect(
                    "Filet electrique",
                    "Paralyse l'adversaire! -8 attaque et -6 defense",
                    3,
                    -8, -6, 0, 0, 0,
                    0, 0, 0, 0, 0,
                    NULL, 0
            )
    };
    strcpy(actions[0].name, "Filet");

    return create_item("Lance-filet", ITEM_WEAPON, actions, 1, 0, 0, 0, 0, 40);
}

// ========== CONSUMABLES ==========

Item create_mega_oxygen(int quantity) {
    return create_item("Mega O2", ITEM_CONSUMABLE, NULL, 0, quantity, 80, 0, 0, 25);
}

Item create_adrenaline_shot(int quantity) {
    return create_item("Adrenaline", ITEM_CONSUMABLE, NULL, 0, quantity, 0, 5, 0, 20);
}

Item create_emergency_medkit(int quantity) {
    return create_item("Kit Urgence", ITEM_CONSUMABLE, NULL, 0, quantity, 0, 0, 50, 30);
}

Item create_full_restore(int quantity) {
    return create_item("Restauration Complete", ITEM_CONSUMABLE, NULL, 0, quantity, 100, 5, 75, 50);
}

Item create_energy_drink(int quantity) {
    return create_item("Boisson Energie", ITEM_CONSUMABLE, NULL, 0, quantity, 20, 3, 10, 15);
}

Item create_antidote(int quantity) {
    return create_item("Antidote", ITEM_CONSUMABLE, NULL, 0, quantity, 0, 0, 20, 12);
}

Item create_nanobots(int quantity) {
    return create_item("Nanobots", ITEM_CONSUMABLE, NULL, 0, quantity, 30, 2, 40, 35);
}

// ========== ARMOR/EQUIPMENT ==========

Item create_reinforced_suit(void) {
    Action* actions = malloc(sizeof(Action) * 1);

    actions[0] = (Action){
            .type = SPECIAL_SKILL,
            .target_type = TARGET_SELF,
            .cooldown_turns = 3,
            .cooldown_remaining = 0,
            .effect = create_effect(
                    "Armure renforcee",
                    "+15 defense pour 3 tours!",
                    3,
                    0, 15, 0, 0, 0,
                    0, 0, 0, 0, 0,
                    NULL, 1
            )
    };
    strcpy(actions[0].name, "Renforcement");

    return create_item("Combinaison Renforcee", ITEM_WEAPON, actions, 1, 0, 0, 0, 0, 75);
}

Item create_power_armor(void) {
    Action* actions = malloc(sizeof(Action) * 2);

    actions[0] = (Action){
            .type = SPECIAL_SKILL,
            .target_type = TARGET_SELF,
            .cooldown_turns = 4,
            .cooldown_remaining = 0,
            .effect = create_effect(
                    "Mode puissance",
                    "+20 attaque pour 2 tours!",
                    2,
                    20, 0, 0, 0, 0,
                    0, 0, 0, 0, 0,
                    NULL, 0
            )
    };
    strcpy(actions[0].name, "Mode Puissance");

    actions[1] = (Action){
            .type = SPECIAL_SKILL,
            .target_type = TARGET_SELF,
            .cooldown_turns = 4,
            .cooldown_remaining = 0,
            .effect = create_effect(
                    "Mode defense",
                    "+25 defense pour 2 tours!",
                    2,
                    0, 25, 0, 0, 0,
                    0, 0, 0, 0, 0,
                    NULL, 0
            )
    };
    strcpy(actions[1].name, "Mode Defense");

    return create_item("Armure Energetique", ITEM_WEAPON, actions, 2, 0, 0, 0, 0, 150);
}

Item create_stealth_suit(void) {
    Action* actions = malloc(sizeof(Action) * 1);

    actions[0] = (Action){
            .type = SPECIAL_SKILL,
            .target_type = TARGET_SELF,
            .cooldown_turns = 5,
            .cooldown_remaining = 0,
            .effect = create_effect(
                    "Camouflage",
                    "+30% attaque, -20% defense pendant 3 tours!",
                    3,
                    0, 0, 0, 0, 0,
                    0.3f, -0.2f, 0, 0, 0,
                    NULL, 0
            )
    };
    strcpy(actions[0].name, "Camouflage");

    return create_item("Tenue Furtive", ITEM_WEAPON, actions, 1, 0, 0, 0, 0, 100);
}
