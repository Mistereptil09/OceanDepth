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
    effect.hp_cost = 5;
    effect.is_active = 1;
    return effect;
}

Effect create_paralysis_effect(int turns) {
    Effect effect = {0};
    strcpy(effect.name, "Paralysis");
    effect.turns_left = turns;
    effect.speed_boost = -1;
    effect.is_active = 1;
    return effect;
}

Effect create_defense_boost_effect(int turns) {
    Effect effect = {0};
    strcpy(effect.name, "Defense Boost");
    effect.turns_left = turns;
    effect.defense_boost = 5;
    effect.is_active = 1;
    return effect;
}

Effect create_attack_boost_effect(int turns) {
    Effect effect = {0};
    strcpy(effect.name, "Attack Boost");
    effect.turns_left = turns;
    effect.attack_boost = 8;
    effect.is_active = 1;
    return effect;
}

Effect create_speed_boost_effect(int turns) {
    Effect effect = {0};
    strcpy(effect.name, "Speed Boost");
    effect.turns_left = turns;
    effect.speed_boost = 1;
    effect.is_active = 1;
    return effect;
}

Effect create_bleed_effect(int turns) {
    Effect effect = {0};
    strcpy(effect.name, "Bleeding");
    effect.turns_left = turns;
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
            {"Tentacle Strike", PHYSICAL_ATTACK, 0, create_bleed_effect(2)},
            {"Ink Cloud", SPECIAL_SKILL, 2, create_paralysis_effect(1)}
        }
    };

    // KING_CRAB - Tank with defense
    CREATURE_TEMPLATES[1] = (CreatureTemplate){
        KING_CRAB, CREATURE_MEDIUM, 80, 120, 12, 20, 10, 2,
        {
            {"Claw Pinch", PHYSICAL_ATTACK, 0, {0}},
            {"Shell Defense", SPECIAL_SKILL, 2, create_defense_boost_effect(3)}
        }
    };

    // JELLYFISH - Poison specialist
    CREATURE_TEMPLATES[2] = (CreatureTemplate){
        JELLYFISH, CREATURE_EASY, 20, 40, 8, 15, 5, 3,
        {
            {"Tentacle Sting", PHYSICAL_ATTACK, 0, create_poison_effect(3)},
            {"Electric Shock", SPECIAL_SKILL, 1, create_paralysis_effect(1)}
        }
    };

    // SHARK - Berserker with bleeding and frenzy
    CREATURE_TEMPLATES[3] = (CreatureTemplate){
        SHARK, CREATURE_MEDIUM, 60, 100, 15, 25, 20, 1,
        {
            {"Bite", PHYSICAL_ATTACK, 0, create_bleed_effect(2)},
            {"Blood Frenzy", SPECIAL_SKILL, 3, create_attack_boost_effect(3)}
        }
    };

    // LEVIATHAN - Boss with massive damage
    CREATURE_TEMPLATES[4] = (CreatureTemplate){
        LEVIATHAN, CREATURE_BOSS, 210, 230, 45, 45, 15, 1,
        {
            {"Ancient Bite", PHYSICAL_ATTACK, 0, create_bleed_effect(4),},
            {"Abyssal Roar", SPECIAL_SKILL, 4, create_paralysis_effect(2)}
        }
    };

    // ANGLERFISH - Ambush predator
    CREATURE_TEMPLATES[5] = (CreatureTemplate){
        ANGLERFISH, CREATURE_HARD, 180, 180, 12, 20, 15, 3,
        {
            {"Lure Attack", PHYSICAL_ATTACK, 0, {0}},
            {"Darkness Veil", SPECIAL_SKILL, 2, {0}}
        }
    };

    // SWORDFISH - Fast striker
    CREATURE_TEMPLATES[6] = (CreatureTemplate){
        SWORDFISH, CREATURE_MEDIUM, 70, 90, 18, 28, 15, 1,
        {
            {"Sword Thrust", PHYSICAL_ATTACK, 0, create_bleed_effect(2)},
            {"Speed Boost", SPECIAL_SKILL, 2, create_speed_boost_effect(3)}
        }
    };

    // MEGALODON - Ultimate boss
    CREATURE_TEMPLATES[7] = (CreatureTemplate){
        MEGALODON, CREATURE_BOSS, 210, 230, 45, 45, 15, 1,
        {
            {"Prehistoric Bite", PHYSICAL_ATTACK, 0, create_bleed_effect(5)},
            {"Apex Predator", SPECIAL_SKILL, 4, create_attack_boost_effect(4)}
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
