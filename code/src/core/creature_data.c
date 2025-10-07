//
// Created by Yasmine Slamani on 07/10/2025.
//
#include "../../includes/core/creature_data.h"
#include <stdio.h>
#include <string.h>

// Helper function to create effects
Effect create_poison_effect(int turns) {
    Effect effect = {0};
    strcpy(effect.name, "Poison");
    effect.turns_left = turns;
    effect.on_tick = poison_tick;
    effect.on_remove = poison_remove;
    effect.hp_cost = 5;
    effect.is_active = 1;
    return effect;
}

Effect create_paralysis_effect(int turns) {
    Effect effect = {0};
    strcpy(effect.name, "Paralysis");
    effect.turns_left = turns;
    effect.on_tick = paralysis_tick;
    effect.on_remove = paralysis_remove;
    effect.speed_boost = -1;
    effect.is_active = 1;
    return effect;
}

Effect create_defense_boost_effect(int turns) {
    Effect effect = {0};
    strcpy(effect.name, "Defense Boost");
    effect.turns_left = turns;
    effect.on_tick = defense_boost_tick;
    effect.on_remove = defense_boost_remove;
    effect.defense_boost = 5;
    effect.is_active = 1;
    return effect;
}

Effect create_attack_boost_effect(int turns) {
    Effect effect = {0};
    strcpy(effect.name, "Attack Boost");
    effect.turns_left = turns;
    effect.on_tick = attack_boost_tick;
    effect.on_remove = attack_boost_remove;
    effect.attack_boost = 8;
    effect.is_active = 1;
    return effect;
}

Effect create_speed_boost_effect(int turns) {
    Effect effect = {0};
    strcpy(effect.name, "Speed Boost");
    effect.turns_left = turns;
    effect.on_tick = speed_boost_tick;
    effect.on_remove = speed_boost_remove;
    effect.speed_boost = 1;
    effect.is_active = 1;
    return effect;
}

Effect create_bleed_effect(int turns) {
    Effect effect = {0};
    strcpy(effect.name, "Bleeding");
    effect.turns_left = turns;
    effect.on_tick = bleed_tick;
    effect.on_remove = bleed_remove;
    effect.hp_cost = 8;
    effect.is_active = 1;
    return effect;
}

void init_creature_templates(void) {
    if (templates_initialized) return;

    // KRAKEN - Physical attacker with bleeding
    CREATURE_TEMPLATES[0] = (CreatureTemplate){
        KRAKEN, CREATURE_HARD, 120, 180, 25, 40, 10, 2,
        {
            {"Tentacle Strike", PHYSICAL_ATTACK, 0, create_bleed_effect(2), tentacle_strike},
            {"Ink Cloud", SPECIAL_SKILL, 2, create_paralysis_effect(1), ink_cloud}
        }
    };

    // KING_CRAB - Tank with defense
    CREATURE_TEMPLATES[1] = (CreatureTemplate){
        KING_CRAB, CREATURE_MEDIUM, 80, 120, 12, 20, 10, 2,
        {
            {"Claw Pinch", PHYSICAL_ATTACK, 0, {0}, claw_pinch},
            {"Shell Defense", SPECIAL_SKILL, 2, create_defense_boost_effect(3), shell_defense}
        }
    };

    // JELLYFISH - Poison specialist
    CREATURE_TEMPLATES[2] = (CreatureTemplate){
        JELLYFISH, CREATURE_EASY, 20, 40, 8, 15, 5, 3,
        {
            {"Tentacle Sting", PHYSICAL_ATTACK, 0, create_poison_effect(3), tentacle_sting},
            {"Electric Shock", SPECIAL_SKILL, 1, create_paralysis_effect(1), electric_shock}
        }
    };

    // SHARK - Berserker with bleeding and frenzy
    CREATURE_TEMPLATES[3] = (CreatureTemplate){
        SHARK, CREATURE_MEDIUM, 60, 100, 15, 25, 20, 1,
        {
            {"Bite", PHYSICAL_ATTACK, 0, create_bleed_effect(2), bite_attack},
            {"Blood Frenzy", SPECIAL_SKILL, 3, create_attack_boost_effect(3), blood_frenzy}
        }
    };

    // LEVIATHAN - Boss with massive damage
    CREATURE_TEMPLATES[4] = (CreatureTemplate){
        LEVIATHAN, CREATURE_BOSS, 210, 230, 45, 45, 15, 1,
        {
            {"Ancient Bite", PHYSICAL_ATTACK, 0, create_bleed_effect(4), ancient_bite},
            {"Abyssal Roar", SPECIAL_SKILL, 4, create_paralysis_effect(2), abyssal_roar}
        }
    };

    // ANGLERFISH - Ambush predator
    CREATURE_TEMPLATES[5] = (CreatureTemplate){
        ANGLERFISH, CREATURE_HARD, 180, 180, 12, 20, 15, 3,
        {
            {"Lure Attack", PHYSICAL_ATTACK, 0, {0}, lure_attack},
            {"Darkness Veil", SPECIAL_SKILL, 2, {0}, darkness_veil}
        }
    };

    // SWORDFISH - Fast striker
    CREATURE_TEMPLATES[6] = (CreatureTemplate){
        SWORDFISH, CREATURE_MEDIUM, 70, 90, 18, 28, 15, 1,
        {
            {"Sword Thrust", PHYSICAL_ATTACK, 0, create_bleed_effect(2), sword_thrust},
            {"Speed Boost", SPECIAL_SKILL, 2, create_speed_boost_effect(3), speed_boost}
        }
    };

    // MEGALODON - Ultimate boss
    CREATURE_TEMPLATES[7] = (CreatureTemplate){
        MEGALODON, CREATURE_BOSS, 210, 230, 45, 45, 15, 1,
        {
            {"Prehistoric Bite", PHYSICAL_ATTACK, 0, create_bleed_effect(5), prehistoric_bite},
            {"Apex Predator", SPECIAL_SKILL, 4, create_attack_boost_effect(4), apex_predator}
        }
    };

    templates_initialized = 1;
}

const CreatureTemplate* get_creature_templates(void) {
    if (!templates_initialized) {
        init_creature_templates();
    }
    return CREATURE_TEMPLATES;
}

int get_creature_template_count(void) {
    return 8;
}

// ACTION IMPLEMENTATIONS
void tentacle_strike(void *user, void *target) {
    printf("Kraken lashes out with powerful tentacles, causing bleeding!\n");
}

void ink_cloud(void *user, void *target) {
    printf("Kraken releases a dark cloud of ink, paralyzing the target!\n");
}

void claw_pinch(void *user, void *target) {
    printf("King Crab snaps with massive claws!\n");
}

void shell_defense(void *user, void *target) {
    printf("King Crab retreats into its armored shell, boosting defense!\n");
}

void tentacle_sting(void *user, void *target) {
    printf("Jellyfish extends venomous tentacles, poisoning the target!\n");
}

void electric_shock(void *user, void *target) {
    printf("Jellyfish releases an electric discharge, paralyzing the target!\n");
}

void bite_attack(void *user, void *target) {
    printf("Shark delivers a devastating bite, causing bleeding!\n");
}

void blood_frenzy(void *user, void *target) {
    printf("Shark enters a blood-fueled frenzy, boosting its attack!\n");
}

void ancient_bite(void *user, void *target) {
    printf("Leviathan's ancient jaws snap with primordial force, causing severe bleeding!\n");
}

void abyssal_roar(void *user, void *target) {
    printf("Leviathan roars with the voice of the abyss, paralyzing all who hear it!\n");
}

void lure_attack(void *user, void *target) {
    printf("Anglerfish uses its luminous lure to deliver a surprise attack!\n");
}

void darkness_veil(void *user, void *target) {
    printf("Anglerfish shrouds the area in impenetrable darkness!\n");
}

void sword_thrust(void *user, void *target) {
    printf("Swordfish thrusts forward with its sharp bill, causing bleeding!\n");
}

void speed_boost(void *user, void *target) {
    printf("Swordfish accelerates to incredible speeds!\n");
}

void prehistoric_bite(void *user, void *target) {
    printf("Megalodon's prehistoric jaws deliver massive damage and severe bleeding!\n");
}

void apex_predator(void *user, void *target) {
    printf("Megalodon channels its apex predator instincts, massively boosting attack!\n");
}

// EFFECT IMPLEMENTATIONS
void poison_tick(void *target) {
    printf("Target takes poison damage!\n");
}

void poison_remove(void *target) {
    printf("Poison effect has worn off.\n");
}

void paralysis_tick(void *target) {
    printf("Target is paralyzed and cannot act!\n");
}

void paralysis_remove(void *target) {
    printf("Paralysis effect has worn off.\n");
}

void defense_boost_tick(void *target) {
    printf("Defense is boosted!\n");
}

void defense_boost_remove(void *target) {
    printf("Defense boost has expired.\n");
}

void attack_boost_tick(void *target) {
    printf("Attack is boosted!\n");
}

void attack_boost_remove(void *target) {
    printf("Attack boost has expired.\n");
}

void speed_boost_tick(void *target) {
    printf("Speed is boosted!\n");
}

void speed_boost_remove(void *target) {
    printf("Speed boost has expired.\n");
}

void bleed_tick(void *target) {
    printf("Target is bleeding and takes damage!\n");
}

void bleed_remove(void *target) {
    printf("Bleeding has stopped.\n");
}