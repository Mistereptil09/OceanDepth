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
            {"Étreinte tentaculaire", SPECIAL_SKILL, 0, 0,
                create_effect("Double attaque", "L'attaque du Kraken a été doublée !",
                     1, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, etreinte_tentaculaire_tick,0)},

            {"Nuage d'encre", SPECIAL_SKILL, 2, 0,
                create_effect("Boost d'attaque", "L'attaque du Kraken est augmenté de 5 !",
                     1, 5, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL,1)},

            {"Nuage d'encre", SPECIAL_SKILL, 2, 0,
                create_effect("Boost d'attaque", "L'attaque du Kraken est augmenté de 5 !",
                     1, 5, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL,1)},

            {"Nuage d'encre", SPECIAL_SKILL, 2, 0,
                create_effect("Boost d'attaque", "L'attaque du Kraken est augmenté de 5 !",
                     1, 5, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL,1)},
        }
    };

    // KING_CRAB - Tank with defense
    CREATURE_TEMPLATES[1] = (CreatureTemplate){
        KING_CRAB, CREATURE_MEDIUM, "Crabe Geant", 80, 120, 12, 20, 10, 2,
        {
            {"Pince écrasante", PHYSICAL_ATTACK, 0, 0,
                create_effect("Malus attaque joueur", "Le joueur subit -3 d'attaque aux 2 prochains tours!",
                     2, -3, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL,0)},

            {"Carapace durcie", SPECIAL_SKILL, 2, 0,
                create_effect("Defense boost", "Le Crabe a 20 points de défense de plus !",
                     1, 0, 20, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL,1)},

            {"Carapace durcie", SPECIAL_SKILL, 2, 0,
                create_effect("Defense boost", "Le Crabe a 20 points de défense de plus !",
                     1, 0, 20, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL,1)},

            {"Carapace durcie", SPECIAL_SKILL, 2, 0,
                create_effect("Defense boost", "Le Crabe a 20 points de défense de plus !",
                     1, 0, 20, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL,1)},
        }
    };

    // JELLYFISH - Poison specialist
    CREATURE_TEMPLATES[2] = (CreatureTemplate){
        JELLYFISH, CREATURE_EASY, "Meduse", 20, 40, 8, 15, 5, 3,
        {
            {"Piqûre tentaculaire", SPECIAL_SKILL, 0, 0,
                create_effect("Boost attaque de 4 ", "+4 points d'attaque pour la méduse !",
            1,
            4, 0, 0, 0, 0,  // flat modifiers
            0, 0, 0, 0, 0,  // percentage modifiers
            NULL,0)
    },
            {"Piqûre paralysante", PHYSICAL_ATTACK, 0, 0,
                create_effect("Malus attaque joueur", "L'attaque du joueur réduite de 1 au prochain tour!",
                     1, -1, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, NULL,0)},

            {"Piqûre paralysante", PHYSICAL_ATTACK, 0, 0,
                create_effect("Malus attaque joueur", "L'attaque du joueur réduite de 1 au prochain tour!!",
                     1, -1, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL,0)},

            {"Piqûre paralysante", PHYSICAL_ATTACK, 0, 0,
                create_effect("Malus attaque joueur", "L'attaque du joueur réduite de 1 au prochain tour!!",
                     1, -1, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL,0)},
        }
    };

    // SHARK - Berserker with bleeding and frenzy
    CREATURE_TEMPLATES[3] = (CreatureTemplate){
        SHARK, CREATURE_MEDIUM, "Requin", 60, 100, 15, 25, 20, 1,
        {
            {"Morsure", SPECIAL_SKILL, 0, 0,
                create_effect("Boost défense", " +2 de défense pour le Requin sur 2 tours !",
                     2, 0, 2, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL,1)},

            {"Frénésie sanguinaire", SPECIAL_SKILL, 0, 0,
                create_effect("Frénésie", "+30% de dégât si -50% en HP pour le Requin !",
                     1, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, frenesie_sanguinaire_tick,0)},

            {"Frénésie sanguinaire", SPECIAL_SKILL, 0, 0,
                create_effect("Frénésie", "+30% de dégât si -50% en HP pour le Requin !",
                     1, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, frenesie_sanguinaire_tick,0)},

            {"Frénésie sanguinaire", SPECIAL_SKILL, 0, 0,
                create_effect("Frénésie", "+30% de dégât si -50% en HP pour le Requin !",
                     1, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, frenesie_sanguinaire_tick,0)},
        }
    };

    // LEVIATHAN - Boss with massive damage
    CREATURE_TEMPLATES[4] = (CreatureTemplate){
        LEVIATHAN, CREATURE_BOSS, "Leviathan", 210, 230, 30, 45, 15, 1,
        {
            {"Morsure antique", SPECIAL_SKILL, 0, 0,
                create_effect("Boost attaque", "+ 8 d'attaque pour le Leviathan durant ce tour !",
                     1, 8, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL,0)},

            {"Rugissement abyssal", SPECIAL_SKILL, 0, 0,
                create_effect("Boost attaque++", "+ 10 d'attaque pour le Leviathan durant ce tour !",
                     1, 10, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL,0)},

            {"Rugissement abyssal", SPECIAL_SKILL, 0, 0,
                create_effect("Boost attaque++", "+ 10 d'attaque pour le Leviathan durant ce tour !",
                     1, 10, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL,0)},

            {"Rugissement abyssal", SPECIAL_SKILL, 0, 0,
                create_effect("Boost attaque++", "+ 10 d'attaque pour le Leviathan durant ce tour !",
                     1, 10, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL,0)},
        }
    };

    // ANGLERFISH - Ambush predator
    CREATURE_TEMPLATES[5] = (CreatureTemplate){
        ANGLERFISH, CREATURE_HARD, "Anglerfish", 180, 180, 12, 20, 15, 3,
        {
            {"Attaque leurre", SPECIAL_SKILL, 0, 0,
                create_effect("+0.1 attaque", "+0.1 atk durant un tour pour Anglerfish !",
                     1, 0, 0, 0, 0, 0,
                    0.1, 0, 0, 0, 0, NULL,0)},

            {"Voile d'obscurité", PHYSICAL_ATTACK, 0, 0,
                create_effect("Malus défense joueur", "Perdu dans l'obscurité, défense réduite de 5 !",
                     1, 0, -5, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL,0)},

            {"Voile d'obscurité", PHYSICAL_ATTACK, 0, 0,
                create_effect("Malus défense joueur", "Perdu dans l'obscurité, défense réduite de 5 !",
                     1, 0, -5, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL,0)},

            {"Voile d'obscurité", PHYSICAL_ATTACK, 0, 0,
                create_effect("Malus défense joueur", "Perdu dans l'obscurité, défense réduite de 5 !",
                     1, 0, -5, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL,0)},
        }
    };

    // SWORDFISH - Fast striker
    CREATURE_TEMPLATES[6] = (CreatureTemplate){
        SWORDFISH, CREATURE_MEDIUM, "Swordfish", 70, 90, 18, 28, 10, 1,
        {
            {"Charge perforante", PHYSICAL_ATTACK, 0, 0,
                create_effect("Malus défense joueur", "L'épée transperce vos défenses, -2 !",
                     1, 0, -2, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL,0)},

            {"Élan de vitesse", SPECIAL_SKILL, 2, 0,
                create_effect("Élan", "Montée d'adrénaline !",
                     0, 0, 0, 2, 0, 0,
                    0, 0, 0, 0, 0, NULL,1)},

            {"Élan de vitesse", SPECIAL_SKILL, 2, 0,
                create_effect("Élan", "Montée d'adrénaline !",
                     0, 0, 0, 2, 0, 0,
                    0, 0, 0, 0, 0, NULL,1)},

            {"Élan de vitesse", SPECIAL_SKILL, 2, 0,
                create_effect("Élan", "Montée d'adrénaline !",
                     0, 0, 0, 2, 0, 0,
                    0, 0, 0, 0, 0, NULL,1)},
        }
    };

    // MEGALODON - Ultimate boss
    CREATURE_TEMPLATES[7] = (CreatureTemplate){
        MEGALODON, CREATURE_BOSS, "Megalodon", 210, 230, 25, 45, 15, 1,
        {
            {"Prehistoric Bite", PHYSICAL_ATTACK, 2, 0, create_effect("Malus defense joueur sur 2 tours", "-7 de défense sur 2 tours pour le joueur!", 2,
                 0, -7, 0, 0, 0,
                 0, 0, 0, 0, 0, NULL,0)},
            {"Apex Predator", SPECIAL_SKILL, 0, 0, create_effect("Boost attaque Mégalodon", "+10 d'atk pour le Mégalodon !", 1,
                 10, 0, 0, 0, 0,
                 0, 0, 0, 0, 0, NULL,0)},
            {"Apex Predator", SPECIAL_SKILL, 0, 0, create_effect("Poison", "+10 d'atk pour le Mégalodon !", 1,
                 10, 0, 0, 0, 0,
                 0, 0, 0, 0, 0, NULL,0)},
            {"Apex Predator", SPECIAL_SKILL, 0, 0, create_effect("Poison", "+10 d'atk pour le Mégalodon !", 1,
                 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL,0)},
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
    int damage = stat_get_value(&self->attack) - stat_get_value(&ennemy->defense); // used cached instead
    if (damage < 1) damage = 1;
    if (damage >= ennemy->current_health_points) {
        ennemy->current_health_points = 0;
        ennemy->is_alive = 0;
    } else {
        ennemy->current_health_points -= damage;
    }
    printf("%s", "Les tentacules vous enserrent pour un double assaut !");
    return SUCCESS;
}

int frenesie_sanguinaire_tick(EntityBase* self, EntityBase* ennemy) {
    if (self == NULL) {
        return POINTER_NULL;
    }
    if (self->current_health_points < self->max_health_points * 0.5) {
        Effect boostAttack = create_effect("Frenesie en action", "Maximisez les dégâts ! ", 1,0,0,0,0,0,0.3,0,0,0,0, NULL,0);
        apply_effect_to_target(self, boostAttack);
    }

    printf("%s", "Le requin entre en frénésie!");

    return SUCCESS;
}

int create_piqure_paralysante_tick(EntityBase* self, EntityBase* ennemy) { // this one works fine
    if (ennemy == NULL) {
        return POINTER_NULL;
    }
    Effect malusAttack = create_effect("Piqûre paralysante en action","La piqûre paralysante vous affaibli !", 1, -1, 0,0,0,0,0,0,0,0,0,NULL,0);
    printf("%s", "Décharge électrique !");
    Effect *effect = apply_effect_to_target(ennemy, malusAttack);
    if (effect == NULL) {
        return POINTER_NULL;
    }
    return SUCCESS;
}

int create_carapace_durcie_tick(EntityBase* self, EntityBase* ennemy) {
    if (ennemy == NULL) {
        return POINTER_NULL;
    }

    // create on player an effect that will reduce attaqk by 20% if opponent is the crab
}
