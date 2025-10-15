//
// Created by Yasmine Slamani on 07/10/2025.
//
#include "../../includes/core/creature_data.h"
#include "../../includes/core/error_codes.h"
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
            {"Étreinte tentaculaire", PHYSICAL_ATTACK, 0, 0,
                create_effect("Étreinte tentaculaire", "Les tentacules vous enserrent pour un double assaut !",
                    1, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, etreinte_tentaculaire_tick)},

            {"Nuage d'encre", PHYSICAL_ATTACK, 2, 0,
                create_effect("Paralysie", "Englouti dans les ténèbres !",
                    2, 0, 5, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL)},

            {"Nuage d'encre", PHYSICAL_ATTACK, 2, 0,
                create_effect("Paralysie", "Englouti dans les ténèbres !",
                    2, 0, 5, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL)},

            {"Nuage d'encre", PHYSICAL_ATTACK, 2, 0,
                create_effect("Paralysie", "Englouti dans les ténèbres !",
                    2, 0, 5, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL)},
        }
    };

    // KING_CRAB - Tank with defense
    CREATURE_TEMPLATES[1] = (CreatureTemplate){
        KING_CRAB, CREATURE_MEDIUM, "Crabe Geant", 80, 120, 12, 20, 10, 2,
        {
            {"Pince écrasante", PHYSICAL_ATTACK, 0, 0,
                create_effect("Paralysis", "Broyé par des pinces puissantes !",
                    1, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL)},

            {"Carapace durcie", SPECIAL_SKILL, 2, 0,
                create_effect("Defense boost", "La carapace se durcit, réduisant les dégâts !",
                    2, 0, 0, 0, 10, 0, 0,
                    0, 0, 0, 0, 0, NULL)},

            {"Carapace durcie", SPECIAL_SKILL, 2, 0,
                create_effect("Defense boost", "La carapace se durcit, réduisant les dégâts !",
                    2, 0, 0, 0, 10, 0, 0,
                    0, 0, 0, 0, 0, NULL)},

            {"Carapace durcie", SPECIAL_SKILL, 2, 0,
                create_effect("Defense boost", "La carapace se durcit, réduisant les dégâts !",
                    2, 0, 0, 0, 10, 0, 0,
                    0, 0, 0, 0, 0, NULL)},
        }
    };

    // JELLYFISH - Poison specialist
    CREATURE_TEMPLATES[2] = (CreatureTemplate){
        JELLYFISH, CREATURE_EASY, "Meduse", 20, 40, 8, 15, 5, 3,
        {
            {"Piqûre tentaculaire", PHYSICAL_ATTACK, 0, 0,
                create_effect("Poison", "Piqûre venimeuse !",
                    1, 5, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL)},

            {"Piqûre paralysante", PHYSICAL_ATTACK, 0, 0,
                create_effect("Paralysie", "Décharge électrique !",
                    1, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, create_piqure_paralysante_tick)},

            {"Piqûre paralysante", PHYSICAL_ATTACK, 0, 0,
                create_effect("Paralysie", "Décharge électrique !",
                    1, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, create_piqure_paralysante_tick)},

            {"Piqûre paralysante", PHYSICAL_ATTACK, 0, 0,
                create_effect("Paralysie", "Décharge électrique !",
                    1, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, create_piqure_paralysante_tick)},
        }
    };

    // SHARK - Berserker with bleeding and frenzy
    CREATURE_TEMPLATES[3] = (CreatureTemplate){
        SHARK, CREATURE_MEDIUM, "Requin", 60, 100, 15, 25, 20, 1,
        {
            {"Morsure", PHYSICAL_ATTACK, 0, 0,
                create_effect("Saignement", "Déchiré par des dents acérées !",
                    2, 5, 0, 2, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL)},

            {"Frénésie sanguinaire", PHYSICAL_ATTACK, 0, 0,
                create_effect("Frénésie", "Le requin entre en frénésie !",
                    1, 15, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, frenesie_sanguinaire_tick)},

            {"Frénésie sanguinaire", PHYSICAL_ATTACK, 0, 0,
                create_effect("Frénésie", "Le requin entre en frénésie !",
                    1, 15, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, frenesie_sanguinaire_tick)},

            {"Frénésie sanguinaire", PHYSICAL_ATTACK, 0, 0,
                create_effect("Frénésie", "Le requin entre en frénésie !",
                    1, 15, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, frenesie_sanguinaire_tick)},
        }
    };

    // LEVIATHAN - Boss with massive damage
    CREATURE_TEMPLATES[4] = (CreatureTemplate){
        LEVIATHAN, CREATURE_BOSS, "Leviathan", 210, 230, 30, 45, 15, 1,
        {
            {"Morsure antique", PHYSICAL_ATTACK, 0, 0,
                create_effect("Saignement", "Les mâchoires primordiales déchirent !",
                    1, 8, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL)},

            {"Rugissement abyssal", PHYSICAL_ATTACK, 0, 0,
                create_effect("Terreur", "Terreur des profondeurs !",
                    2, 0, 10, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL)},

            {"Rugissement abyssal", PHYSICAL_ATTACK, 0, 0,
                create_effect("Terreur", "Terreur des profondeurs !",
                    2, 0, 10, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL)},

            {"Rugissement abyssal", PHYSICAL_ATTACK, 0, 0,
                create_effect("Terreur", "Terreur des profondeurs !",
                    2, 0, 10, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL)},
        }
    };

    // ANGLERFISH - Ambush predator
    CREATURE_TEMPLATES[5] = (CreatureTemplate){
        ANGLERFISH, CREATURE_HARD, "Anglerfish", 180, 180, 12, 20, 15, 3,
        {
            {"Attaque leurre", PHYSICAL_ATTACK, 0, 0,
                create_effect("Saignement", "Embuscade depuis les ombres !",
                    2, 6, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL)},

            {"Voile d'obscurité", PHYSICAL_ATTACK, 0, 0,
                create_effect("Cécité", "Perdu dans l'obscurité !",
                    2, 0, 0, 5, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL)},

            {"Voile d'obscurité", PHYSICAL_ATTACK, 0, 0,
                create_effect("Cécité", "Perdu dans l'obscurité !",
                    2, 0, 0, 5, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL)},

            {"Voile d'obscurité", PHYSICAL_ATTACK, 0, 0,
                create_effect("Cécité", "Perdu dans l'obscurité !",
                    2, 0, 0, 5, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL)},
        }
    };

    // SWORDFISH - Fast striker
    CREATURE_TEMPLATES[6] = (CreatureTemplate){
        SWORDFISH, CREATURE_MEDIUM, "Swordfish", 70, 90, 18, 28, 10, 1,
        {
            {"Charge perforante", PHYSICAL_ATTACK, 0, 0,
                create_effect("Perforation", "L'épée transperce vos défenses !",
                    1, 0, 2, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL)},

            {"Élan de vitesse", SPECIAL_SKILL, 2, 0,
                create_effect("Élan", "Montée d'adrénaline !",
                    2, 0, 2, 0, 2, 0, 0,
                    0, 0, 0, 0, 0, NULL)},

            {"Élan de vitesse", SPECIAL_SKILL, 2, 0,
                create_effect("Élan", "Montée d'adrénaline !",
                    2, 0, 2, 0, 2, 0, 0,
                    0, 0, 0, 0, 0, NULL)},

            {"Élan de vitesse", SPECIAL_SKILL, 2, 0,
                create_effect("Élan", "Montée d'adrénaline !",
                    2, 0, 2, 0, 2, 0, 0,
                    0, 0, 0, 0, 0, NULL)},
        }
    };

    // MEGALODON - Ultimate boss
    CREATURE_TEMPLATES[7] = (CreatureTemplate){
        MEGALODON, CREATURE_BOSS, "Megalodon", 210, 230, 25, 45, 15, 1,
        {
            {"Prehistoric Bite", PHYSICAL_ATTACK, 2, 0, create_effect("Bleed", "Ancient fury unleashed!", 2,
                10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL)},
            {"Apex Predator", PHYSICAL_ATTACK, 0, 0, create_effect("Poison", "Apex venom!", 1,
                10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL)},
            {"Apex Predator", PHYSICAL_ATTACK, 0, 0, create_effect("Poison", "Apex venom!", 1,
                10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL)},
            {"Apex Predator", PHYSICAL_ATTACK, 0, 0, create_effect("Poison", "Apex venom!", 1,
                10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL)},
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

int etreinte_tentaculaire_tick(EntityBase* self, EntityBase* ennemy) {
    if (self == NULL || ennemy == NULL) {
        return POINTER_NULL;
    }
    int damage = self->attack.current_value - ennemy->defense.current_value;
    if (damage < 1) damage = 1;
    if (damage >= ennemy->current_health_points) {
        ennemy->current_health_points = 0;
        ennemy->is_alive = 0;
    } else {
        ennemy->current_health_points -= damage;
    }
    // probably call display
    return SUCCESS;
}

int frenesie_sanguinaire_tick(EntityBase* self, EntityBase* ennemy) {
    if (self == NULL) {
        return POINTER_NULL;
    }
    if (self->current_health_points < self->max_health_points * 0.5) {
        self->attack.current_value += (int)(self->attack.current_value * 0.3);
    }
    // probably call display
    return SUCCESS;
}

int create_piqure_paralysante_tick(EntityBase* self, EntityBase* ennemy) {
    if (ennemy == NULL) {
        return POINTER_NULL;
    }
    Effect malusAttack = create_effect("Piqûre paralysante en action","La piqûre paralysante vous affaibli !", 2, -1, 0, 0,0,0,0,0,0,0,0,0,NULL);
    return apply_effect_to_target(ennemy, malusAttack);
}


