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

    // KRAKEN - Attaquant physique avec étreinte
    CREATURE_TEMPLATES[0] = (CreatureTemplate){
        KRAKEN, CREATURE_HARD, 120, 180, 25, 40, 10, 2,
        {
            {"Étreinte Tentaculaire", PHYSICAL_ATTACK, 0, create_etreinte_tentaculaire_effect()},
            {"Nuage d'Encre", PHYSICAL_ATTACK, 2, create_nuage_encre_effect()}
        }
    };

    // KING_CRAB - Tank avec défense
    CREATURE_TEMPLATES[1] = (CreatureTemplate){
        KING_CRAB, CREATURE_MEDIUM, 80, 120, 12, 20, 10, 2,
        {
            {"Pince Écrasante", PHYSICAL_ATTACK, 0, create_pince_ecrasante_effect()},
            {"Carapace Durcie", SPECIAL_SKILL, 2, create_carapace_durcie_effect()}
        }
    };

    // JELLYFISH - Spécialiste du poison
    CREATURE_TEMPLATES[2] = (CreatureTemplate){
        JELLYFISH, CREATURE_EASY, 20, 40, 8, 15, 5, 3,
        {
            {"Piqûre Tentaculaire", PHYSICAL_ATTACK, 0, create_piqure_tentaculaire_effect()},
            {"Piqûre Paralysante", PHYSICAL_ATTACK, 0, create_piqure_paralysante_effect()}
        }
    };

    // SHARK - Berserker avec frénésie
    CREATURE_TEMPLATES[3] = (CreatureTemplate){
        SHARK, CREATURE_MEDIUM, 60, 100, 15, 25, 20, 1,
        {
            {"Morsure", PHYSICAL_ATTACK, 0, create_morsure_effect()},
            {"Frénésie Sanguinaire", PHYSICAL_ATTACK, 0, create_frenesie_sanguinaire_effect()}
        }
    };

    // LEVIATHAN - Boss avec dégâts massifs
    CREATURE_TEMPLATES[4] = (CreatureTemplate){
        LEVIATHAN, CREATURE_BOSS, 210, 230, 30, 45, 15, 1,
        {
            {"Morsure Antique", PHYSICAL_ATTACK, 0, create_morsure_antique_effect()},
            {"Rugissement Abyssal", PHYSICAL_ATTACK, 0, create_rugissement_abyssal_effect()}
        }
    };

    // ANGLERFISH - Prédateur d'embuscade
    CREATURE_TEMPLATES[5] = (CreatureTemplate){
        ANGLERFISH, CREATURE_HARD, 180, 180, 12, 20, 15, 3,
        {
            {"Attaque Leurre", PHYSICAL_ATTACK, 0, create_attaque_leurre_effect()},
            {"Voile d'Obscurité", PHYSICAL_ATTACK, 0, create_voile_obscurite_effect()}
        }
    };

    // SWORDFISH - Attaquant rapide
    CREATURE_TEMPLATES[6] = (CreatureTemplate){
        SWORDFISH, CREATURE_MEDIUM, 70, 90, 18, 28, 10, 1,
        {
            {"Charge Perforante", PHYSICAL_ATTACK, 0, create_charge_perforante_effect()},
            {"Bonus d'Attaque", SPECIAL_SKILL, 2, create_bonus_attaque_effect()}
        }
    };

    // MEGALODON - Boss ultime
    CREATURE_TEMPLATES[7] = (CreatureTemplate){
        MEGALODON, CREATURE_BOSS, 210, 230, 25, 45, 15, 1,
        {
            {"Morsure Préhistorique", PHYSICAL_ATTACK, 2, create_morsure_prehistorique_effect()},
            {"Prédateur Suprême", PHYSICAL_ATTACK, 0, create_predateur_supreme_effect()}
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