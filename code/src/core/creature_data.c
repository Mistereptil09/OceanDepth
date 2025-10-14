//
// Created by Yasmine Slamani on 07/10/2025.
//
#include "../../includes/core/creature_data.h"
#include <stdio.h>
#include <stdlib.h>

// Global templates array
static CreatureTemplate CREATURE_TEMPLATES[8];
static int templates_initialized = 0;

int random_range(int min, int max) {
    if (min > max) return min;
    if (min == max) return min;
    return min + rand() % (max - min + 1);
}

void init_creature_templates(void) {
    if (templates_initialized) return;

    // KRAKEN - Physical attacker with bleeding
    CREATURE_TEMPLATES[0] = (CreatureTemplate){
        KRAKEN, CREATURE_HARD, "Kraken", 120, 180, 25, 40, 10, 2,
        {
            {"Tentacle Strike", PHYSICAL_ATTACK, 0, 0, create_effect("Bleed", "Tentacles tear flesh!", 1,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL)},
            {"Ink Cloud", PHYSICAL_ATTACK, 2, 0, create_effect("Paralysis", "Engulfed in darkness!", 2,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL)},
            {"Ink Cloud", PHYSICAL_ATTACK, 2, 0, create_effect("Paralysis", "Engulfed in darkness!", 2,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL)},
            {"Ink Cloud", PHYSICAL_ATTACK, 2, 0, create_effect("Paralysis", "Engulfed in darkness!", 2,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL)},
        }
    };

    // KING_CRAB - Tank with defense
    CREATURE_TEMPLATES[1] = (CreatureTemplate){
        KING_CRAB, CREATURE_MEDIUM, "King Crab", 80, 120, 12, 20, 10, 2,
        {
            {"Claw Pinch", PHYSICAL_ATTACK, 0, 0, create_effect("Paralysis", "Pinned by mighty claws!", 1,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL)},
            {"Shell Defense", SPECIAL_SKILL, 2, 0, create_effect("Defense boost", "Shell hardens!", 2,
                0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, NULL)},
            {"Shell Defense", SPECIAL_SKILL, 2, 0, create_effect("Defense boost", "Shell hardens!", 2,
                0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, NULL)},
            {"Shell Defense", SPECIAL_SKILL, 2, 0, create_effect("Defense boost", "Shell hardens!", 2,
                0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, NULL)},
        }
    };

    // JELLYFISH - Poison specialist
    CREATURE_TEMPLATES[2] = (CreatureTemplate){
        JELLYFISH, CREATURE_EASY, "Jellyfish", 20, 40, 8, 15, 5, 3,
        {
            {"Tentacle Sting", PHYSICAL_ATTACK, 0, 0, create_effect("Poison", "Venomous sting!", 1,
                5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL)},
            {"Electric Shock", PHYSICAL_ATTACK, 0, 0, create_effect("Paralysis", "Electric jolt!", 1,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL)},
            {"Electric Shock", PHYSICAL_ATTACK, 0, 0, create_effect("Paralysis", "Electric jolt!", 1,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL)},
            {"Electric Shock", PHYSICAL_ATTACK, 0, 0, create_effect("Paralysis", "Electric jolt!", 1,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL)},
        }
    };

    // SHARK - Berserker with bleeding and frenzy
    CREATURE_TEMPLATES[3] = (CreatureTemplate){
        SHARK, CREATURE_MEDIUM, "Shark", 60, 100, 15, 25, 20, 1,
        {
            {"Bite", PHYSICAL_ATTACK, 0, 0, create_effect("Bleed", "Razor teeth bite deep!", 1,
                10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL)},
            {"Blood Frenzy", PHYSICAL_ATTACK, 0, 0, create_effect("Bleed", "Frenzied bloodlust!", 1,
                15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL)},
            {"Blood Frenzy", PHYSICAL_ATTACK, 0, 0, create_effect("Bleed", "Frenzied bloodlust!", 1,
                15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL)},
            {"Blood Frenzy", PHYSICAL_ATTACK, 0, 0, create_effect("Bleed", "Frenzied bloodlust!", 1,
                15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL)},
        }
    };

    // LEVIATHAN - Boss with massive damage
    CREATURE_TEMPLATES[4] = (CreatureTemplate){
        LEVIATHAN, CREATURE_BOSS, "Leviathan", 210, 230, 30, 45, 15, 1,
        {
            {"Ancient Bite", PHYSICAL_ATTACK, 0, 0, create_effect("Bleed", "Primordial jaws rend!", 1,
                10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL)},
            {"Abyssal Roar", PHYSICAL_ATTACK, 0, 0, create_effect("Paralysis", "Terror from the deep!", 1,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL)},
            {"Abyssal Roar", PHYSICAL_ATTACK, 0, 0, create_effect("Paralysis", "Terror from the deep!", 1,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL)},
            {"Abyssal Roar", PHYSICAL_ATTACK, 0, 0, create_effect("Paralysis", "Terror from the deep!", 1,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL)},
        }
    };

    // ANGLERFISH - Ambush predator
    CREATURE_TEMPLATES[5] = (CreatureTemplate){
        ANGLERFISH, CREATURE_HARD, "Anglerfish", 180, 180, 12, 20, 15, 3,
        {
            {"Lure Attack", PHYSICAL_ATTACK, 0, 0, create_effect("Bleed", "Ambushed from shadows!", 1,
                10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL)},
            {"Darkness Veil", PHYSICAL_ATTACK, 0, 0, create_effect("Paralysis", "Lost in darkness!", 1,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL)},
            {"Darkness Veil", PHYSICAL_ATTACK, 0, 0, create_effect("Paralysis", "Lost in darkness!", 1,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL)},
            {"Darkness Veil", PHYSICAL_ATTACK, 0, 0, create_effect("Paralysis", "Lost in darkness!", 1,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL)},
        }
    };

    // SWORDFISH - Fast striker
    CREATURE_TEMPLATES[6] = (CreatureTemplate){
        SWORDFISH, CREATURE_MEDIUM, "Swordfish", 70, 90, 18, 28, 10, 1,
        {
            {"Sword Thrust", PHYSICAL_ATTACK, 0, 0, create_effect("Bleed", "Pierced by blade!", 1,
                10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL)},
            {"Attack Boost", SPECIAL_SKILL, 2, 0, create_effect("Attack boost", "Speed surge!", 2,
                0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL)},
            {"Attack Boost", SPECIAL_SKILL, 2, 0, create_effect("Attack boost", "Speed surge!", 2,
                0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL)},
            {"Attack Boost", SPECIAL_SKILL, 2, 0, create_effect("Attack boost", "Speed surge!", 2,
                0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL)},
        }
    };

    // MEGALODON - Ultimate boss
    CREATURE_TEMPLATES[7] = (CreatureTemplate){
        MEGALODON, CREATURE_BOSS, "Megalodon", 210, 230, 25, 45, 15, 1,
        {
            {"Prehistoric Bite", PHYSICAL_ATTACK, 2, 0, create_effect("Bleed", "Ancient fury unleashed!", 2,
                10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL)},
            {"Apex Predator", PHYSICAL_ATTACK, 0, 0, create_effect("Poison", "Apex venom!", 1,
                10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL)},
            {"Apex Predator", PHYSICAL_ATTACK, 0, 0, create_effect("Poison", "Apex venom!", 1,
                10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL)},
            {"Apex Predator", PHYSICAL_ATTACK, 0, 0, create_effect("Poison", "Apex venom!", 1,
                10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL)},
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

void cleanup_creature_templates(void) {
    if (!templates_initialized) return;

    // Free all display_messages from effects in all creature templates
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < MAX_ACTIONS; j++) {
            free_effect_content(&CREATURE_TEMPLATES[i].actions[j].effect);
        }
    }

    templates_initialized = 0;
}