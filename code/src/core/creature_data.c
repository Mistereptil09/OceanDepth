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
        KRAKEN, CREATURE_HARD, "Kraken", 120, 180, 25, 40, 10, 5,
        {
            {"etreinte tentaculaire", PHYSICAL_ATTACK, TARGET_SELF, 2, 0,
                create_effect("ATTACK X2", "L'attaque du Kraken a ete doublee !",
                     1, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, etreinte_tentaculaire_tick,0)},

            {"Nuage d'encre", PHYSICAL_ATTACK, TARGET_SELF, 0, 0,
                create_effect("BOOST ATK 5", "L'attaque du Kraken est augmente de 5 !",
                     1, 5, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL,0)},

            {"Tourbillon", SPECIAL_SKILL, TARGET_SELF, 1, 0,
                create_effect("BOOST DEF +2", "Le Kraken se defend!",
                     2, 0, 2, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL,1)},

            {"Ventouses infernale", PHYSICAL_ATTACK, TARGET_OPPONENT,2, 0,
                create_effect("MALUS DEF -2", "Le Kraken vous piege avec ses ventouses!",
                     1, 0, -2, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL,0)},
        }
    };

    // KING_CRAB - Tank with defense
    CREATURE_TEMPLATES[1] = (CreatureTemplate){
        KING_CRAB, CREATURE_MEDIUM, "Crabe Geant", 80, 120, 12, 20, 10, 3,
        {
            {"Pince ecrasante", PHYSICAL_ATTACK, TARGET_OPPONENT, 0, 0,
                create_effect("MALUS ATK -3", "Le joueur subit -3 d'attaque au prochain tour!",
                     1, -3, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL,0)},

            {"Carapace durcie", SPECIAL_SKILL,TARGET_SELF, 2, 0,
                create_effect("BOOST DEF +20", "Le Crabe a 20 points de defense de plus !",
                     1, 0, 20, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL,1)},

            {"Cisaillement", PHYSICAL_ATTACK,TARGET_SELF, 0, 0,
                create_effect("BOOST ATK +2", "Le Crabe sort de sa carapace !",
                     1, 2, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL,0)},

            {"Bulles Protectrices", SPECIAL_SKILL, TARGET_SELF, 1, 0,
                create_effect("DEF BOOST +2", "Le Crabe a 2 points de defense de plus !",
                     1, 0, 2, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL,1)},
        }
    };

    // JELLYFISH - Poison specialist
    CREATURE_TEMPLATES[2] = (CreatureTemplate){
        JELLYFISH, CREATURE_EASY, "Meduse", 20, 40, 8, 15, 5, 0,
        {
            {"Piqûre tentaculaire", PHYSICAL_ATTACK, TARGET_SELF, 0, 0,
                create_effect("ATK BOOST +4", "+4 points d'attaque pour la meduse !",
            1,
            4, 0, 0, 0, 0,  // flat modifiers
            0, 0, 0, 0, 0,  // percentage modifiers
            NULL,0)
    },
            {"Piqûre paralysante", PHYSICAL_ATTACK, TARGET_OPPONENT, 1, 0,
                create_effect("ATK MALUS -1", "L'attaque du joueur reduite de 1 au prochain tour!",
                     2, -1, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, NULL,0)},

            {"Shock electrique", SPECIAL_SKILL,TARGET_SELF, 2, 0,
                create_effect("SPEED BOOST +5", "La Meduse prend en vitesse!",
                     1, 0, 0, 5, 0, 0,
                    0, 0, 0, 0, 0, NULL,1)},

            {"Venin Foudroyant", PHYSICAL_ATTACK, TARGET_OPPONENT,1, 0,
                create_effect("DEF MALUS -1", "La defense du joueur reduite de 1 au 2 prochains tours!",
                     2, -0, -1, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL,0)},
        }
    };

    // SHARK - Berserker with bleeding and frenzy
    CREATURE_TEMPLATES[3] = (CreatureTemplate){
        SHARK, CREATURE_MEDIUM, "Requin", 60, 100, 15, 25, 20, 4,
        {
            {"Morsure", SPECIAL_SKILL,TARGET_SELF, 2, 0,
                create_effect("DEF BOOST +2", " +2 de defense pour le Requin sur 2 tours !",
                     2, 0, 2, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL,1)},

            {"Frenesie sanguinaire", PHYSICAL_ATTACK, TARGET_SELF, 0, 0,
                create_effect("CONDI ATK +30% BOOST", "+30% de degat si -50% en HP pour le Requin !",
                     1, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, frenesie_sanguinaire_tick,0)},

            {"Machoires d'Acier", PHYSICAL_ATTACK, TARGET_SELF, 0, 0,
                create_effect("ATK BOOST +3", "+3 d'attaque pour le Requin !",
                     1, 3, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL,0)},

            {"Rage du Predateur", PHYSICAL_ATTACK,TARGET_OPPONENT, 3, 0,
                create_effect("ATK MALUS -5", "-5% d'attaque pour le joueur au prochain tour!",
                     1, 0, 0, 0, 0, 0,
                    0.05, 0, 0, 0, 0, NULL,0)},
        }
    };

    // LEVIATHAN - Boss with massive damage
    CREATURE_TEMPLATES[4] = (CreatureTemplate){
        LEVIATHAN, CREATURE_BOSS, "Leviathan", 210, 230, 30, 45, 15, 3,
        {
            {"Morsure antique", PHYSICAL_ATTACK, TARGET_SELF, 1, 0,
                create_effect("ATK BOOST +8", "+ 8 d'attaque pour le Leviathan durant ce tour !",
                     1, 8, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL,0)},

            {"Rugissement abyssal", PHYSICAL_ATTACK, TARGET_SELF, 0, 0,
                create_effect("ATK BOOST +5", "+ 5 d'attaque pour le Leviathan durant ce tour !",
                     1, 5, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL,0)},

            {"Griffes legendaires", SPECIAL_SKILL, TARGET_SELF, 2, 0,
                create_effect("DEF BOOST +6", "+ 6 de defense pour le Leviathan sur 2 tours !",
                     2, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL,1)},

            {"Encerclement mortel", PHYSICAL_ATTACK, TARGET_OPPONENT, 0, 0,
                create_effect("ATK MALUS -4", "-4 d'attaque pour vous !",
                     1, -4, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL,0)},
        }
    };

    // ANGLERFISH - Ambush predator
    CREATURE_TEMPLATES[5] = (CreatureTemplate){
        ANGLERFISH, CREATURE_HARD, "Anglerfish", 180, 180, 12, 20, 15, 3,
        {
            {"Leurre hypnotique", PHYSICAL_ATTACK, TARGET_SELF, 1, 0,
                create_effect("ATK BOOST +10%", "+10% d'attaque pour l'Anglerfish durant ce tour !",
                     1, 0, 0, 0, 0, 0,
                    0.1, 0, 0, 0, 0, NULL,0)},

            {"Voile d'obscurite", PHYSICAL_ATTACK, TARGET_OPPONENT,2, 0,
                create_effect("DEF MALUS -5", "Perdu dans l'obscurite, defense reduite de 5 !",
                     1, 0, -5, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL,0)},

            {"Machoires des profondeurs", PHYSICAL_ATTACK, TARGET_SELF, 0, 0,
                create_effect("ATK BOOST +6", "+6 d'attaque pour l'Anglerfish durant ce tour !",
                     1, 6, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL,0)},

            {"Camouflage des abysses", SPECIAL_SKILL,TARGET_SELF, 2, 0,
                create_effect("DEF BOOST +6", "+6 de defense pour l'Anglerfish sur 2 tours !",
                     2, 0, 6, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL,1)},
        }
    };

    // SWORDFISH - Fast striker
    CREATURE_TEMPLATES[6] = (CreatureTemplate){
        SWORDFISH, CREATURE_MEDIUM, "Swordfish", 70, 90, 18, 28, 10, 4,
        {
            {"Charge perforante", PHYSICAL_ATTACK, TARGET_OPPONENT, 0, 0,
                create_effect("DEF MALUS -2", "L'epee transperce vos defenses, -2 !",
                     1, 0, -2, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL,0)},

            {"Elan de vitesse", SPECIAL_SKILL, TARGET_SELF, 2, 0,
                create_effect("SPEED BOOST +2", "Montee d'adrenaline pour le Swordfish !",
                     1, 0, 0, 2, 0, 0,
                    0, 0, 0, 0, 0, NULL,1)},

            {"Lame argentee", PHYSICAL_ATTACK, TARGET_SELF, 1, 0,
                create_effect("ATK BOOST +5", "+5 d'attaque pour le Swordfish durant ce tour !",
                     1, 5, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL,0)},

            {"Tourbillon tranchant", PHYSICAL_ATTACK, TARGET_OPPONENT, 2, 0,
                create_effect("ATK MALUS -3", "Pris dans le tourbillon, -3 d'attaque !",
                     2, -3, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL,0)},
        }
    };

    // MEGALODON - Ultimate boss
    CREATURE_TEMPLATES[7] = (CreatureTemplate){
        MEGALODON, CREATURE_BOSS, "Megalodon", 210, 230, 25, 45, 15, 1,
        {
            {"Morsure prehistorique", PHYSICAL_ATTACK,TARGET_OPPONENT, 2, 0,
                create_effect("DEF MALUS -7", "-7 de defense sur 2 tours pour le joueur !",
                     1, 0, -7, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL,0)},

            {"Instinct de predateur", PHYSICAL_ATTACK, TARGET_SELF, 0, 0,
                create_effect("ATK BOOST +10", "+10 d'attaque pour le Megalodon durant ce tour !",
                     1, 10, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL,0)},

            {"Rage primordiale", PHYSICAL_ATTACK, TARGET_SELF, 0, 0,
                create_effect("ATK BOOST +15%", "+15% d'attaque pour le Megalodon durant ce tour !",
                     1, 0, 0, 0, 0, 0,
                    0.15, 0, 0, 0, 0, NULL,0)},

            {"Ecailles titanesques", SPECIAL_SKILL, TARGET_SELF, 6, 0,
                create_effect("DEF BOOST +10", "+10 de defense pour le Megalodon sur 3 tours !",
                     3, 0, 10, 0, 0, 0,
                    0, 0, 0, 0, 0, NULL,1)},
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
    printf("Les tentacules vous enserrent pour un double assaut !");
    return SUCCESS;
}

int frenesie_sanguinaire_tick(EntityBase* self, EntityBase* ennemy) {
    if (self == NULL) {
        return POINTER_NULL;
    }
    if (self->current_health_points < self->max_health_points * 0.5) {
        Effect boostAttack = create_effect("Frenesie en action", "Maximisez les degats ! ", 1,0,0,0,0,0,0.3,0,0,0,0, NULL,0);
        apply_effect_to_target(self, boostAttack);
        printf("Le requin entre en frenesie!\n");
    } else {
        printf("Le requin n'est pas assez faible pour la frenesie!\n");
    }

    return SUCCESS;
}

int create_piqure_paralysante_tick(EntityBase* self, EntityBase* ennemy) { // this one works fine
    if (ennemy == NULL) {
        return POINTER_NULL;
    }
    Effect malusAttack = create_effect("Piqûre paralysante en action","La piqûre paralysante vous affaibli !", 1, -1, 0,0,0,0,0,0,0,0,0,NULL,0);
    printf("%s", "Decharge electrique !");
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
