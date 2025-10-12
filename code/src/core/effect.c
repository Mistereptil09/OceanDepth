//
// Created by Yasmine Slamani on 04/10/2025.
//

#include "core/effect.h"

#include <stdlib.h>
#include <string.h>

#include "core/entity.h"
#include "core/error_codes.h"


// todo include effect triggering here

Effect create_effect(EffectType type, char *display_message, int turns,
                    int hp_cost, int oxygen_cost, int defense_cost,
                    int attack_boost, int defense_boost, int oxygen_boost, int speed_boost,
                    FonctionEffect on_tick, FonctionEffect on_remove) {
    Effect effect = {0};

    switch (type) {
        case EFFECT_POISON:
            strcpy(effect.name, "Poison");
            break;
        case EFFECT_PARALYSIS:
            strcpy(effect.name, "Paralysie");
            break;
        case EFFECT_DEFENSE_BOOST:
            strcpy(effect.name, "Bonus Défense");
            break;
        case EFFECT_ATTACK_BOOST:
            strcpy(effect.name, "Bonus Attaque");
            break;
        case EFFECT_BLEED:
            strcpy(effect.name, "Saignement");
            break;
    }

    if (display_message != NULL) {
        effect.display_message = malloc(strlen(display_message) + 1);
        if (effect.display_message != NULL) {
            strcpy(effect.display_message, display_message);
        }
    } else {
        effect.display_message = NULL;
    }

    effect.turns_left = turns;
    effect.hp_cost = hp_cost;
    effect.oxygen_cost = oxygen_cost;
    effect.defense_cost = defense_cost;
    effect.attack_boost = attack_boost;
    effect.defense_boost = defense_boost;
    effect.oxygen_boost = oxygen_boost;
    effect.speed_boost = speed_boost;
    effect.on_tick = on_tick;
    effect.on_remove = on_remove;
    effect.is_active = 1;

    return effect;
}

Effect effect_copy(const Effect* src) {
    if (src == NULL) {
        Effect empty = {0};
        return empty;
    }

    Effect copy = *src;  // Copy all fields

    // Duplicate the display_message
    if (src->display_message != NULL) {
        copy.display_message = malloc(strlen(src->display_message) + 1);
        if (copy.display_message != NULL) {
            strcpy(copy.display_message, src->display_message);
        }
    } else {
        copy.display_message = NULL;
    }

    return copy;
}


void free_effect_content(Effect *effect) {
    if (effect == NULL) return;
    if (effect->display_message != NULL) {
        free(effect->display_message);
        effect->display_message = NULL;
    }
}

// Toutes les fonctions tick (retournent SUCCESS pour l'instant)
int etreinte_tentaculaire_tick(EntityBase *target) { return SUCCESS; }
int etreinte_tentaculaire_remove(EntityBase *target) { return SUCCESS; }

int nuage_encre_tick(EntityBase *target) { return SUCCESS; }
int nuage_encre_remove(EntityBase *target) { return SUCCESS; }

int pince_ecrasante_tick(EntityBase *target) { return SUCCESS; }
int pince_ecrasante_remove(EntityBase *target) { return SUCCESS; }

int carapace_durcie_tick(EntityBase *target) { return SUCCESS; }
int carapace_durcie_remove(EntityBase *target) { return SUCCESS; }

int piqure_tentaculaire_tick(EntityBase *target) { return SUCCESS; }
int piqure_tentaculaire_remove(EntityBase *target) { return SUCCESS; }

int piqure_paralysante_tick(EntityBase *target) { return SUCCESS; }
int piqure_paralysante_remove(EntityBase *target) { return SUCCESS; }

int morsure_tick(EntityBase *target) { return SUCCESS; }
int morsure_remove(EntityBase *target) { return SUCCESS; }

int frenesie_sanguinaire_tick(EntityBase *target) { return SUCCESS; }
int frenesie_sanguinaire_remove(EntityBase *target) { return SUCCESS; }

int morsure_antique_tick(EntityBase *target) { return SUCCESS; }
int morsure_antique_remove(EntityBase *target) { return SUCCESS; }

int rugissement_abyssal_tick(EntityBase *target) { return SUCCESS; }
int rugissement_abyssal_remove(EntityBase *target) { return SUCCESS; }

int attaque_leurre_tick(EntityBase *target) { return SUCCESS; }
int attaque_leurre_remove(EntityBase *target) { return SUCCESS; }

int voile_obscurite_tick(EntityBase *target) { return SUCCESS; }
int voile_obscurite_remove(EntityBase *target) { return SUCCESS; }

int charge_perforante_tick(EntityBase *target) { return SUCCESS; }
int charge_perforante_remove(EntityBase *target) { return SUCCESS; }

int bonus_attaque_tick(EntityBase *target) { return SUCCESS; }
int bonus_attaque_remove(EntityBase *target) { return SUCCESS; }

int morsure_prehistorique_tick(EntityBase *target) { return SUCCESS; }
int morsure_prehistorique_remove(EntityBase *target) { return SUCCESS; }

int predateur_supreme_tick(EntityBase *target) { return SUCCESS; }
int predateur_supreme_remove(EntityBase *target) { return SUCCESS; }

// Fonctions de création (toutes avec 0 pour hp_cost etc.)
Effect create_etreinte_tentaculaire_effect(void) {
    return create_effect(EFFECT_BLEED, "Étreinte tentaculaire !", 1, 0, 0, 0, 0, 0, 0, 0,
                        etreinte_tentaculaire_tick, etreinte_tentaculaire_remove);
}

Effect create_nuage_encre_effect(void) {
    return create_effect(EFFECT_PARALYSIS, "Englouti dans les ténèbres !", 2, 0, 0, 0, 0, 0, 0, 0,
                        nuage_encre_tick, nuage_encre_remove);
}

Effect create_pince_ecrasante_effect(void) {
    return create_effect(EFFECT_PARALYSIS, "Pris dans les pinces !", 1, 0, 0, 0, 0, 0, 0, 0,
                        pince_ecrasante_tick, pince_ecrasante_remove);
}

Effect create_carapace_durcie_effect(void) {
    return create_effect(EFFECT_DEFENSE_BOOST, "Carapace durcie !", 3, 0, 0, 0, 0, 0, 0, 0,
                        carapace_durcie_tick, carapace_durcie_remove);
}

Effect create_piqure_tentaculaire_effect(void) {
    return create_effect(EFFECT_POISON, "Piqûre venimeuse !", 1, 0, 0, 0, 0, 0, 0, 0,
                        piqure_tentaculaire_tick, piqure_tentaculaire_remove);
}

Effect create_piqure_paralysante_effect(void) {
    return create_effect(EFFECT_PARALYSIS, "Piqûre paralysante !", 1, 0, 0, 0, 0, 0, 0, 0,
                        piqure_paralysante_tick, piqure_paralysante_remove);
}

Effect create_morsure_effect(void) {
    return create_effect(EFFECT_BLEED, "Dents acérées !", 1, 0, 0, 0, 0, 0, 0, 0,
                        morsure_tick, morsure_remove);
}

Effect create_frenesie_sanguinaire_effect(void) {
    return create_effect(EFFECT_ATTACK_BOOST, "Frénésie sanguinaire !", 1, 0, 0, 0, 0, 0, 0, 0,
                        frenesie_sanguinaire_tick, frenesie_sanguinaire_remove);
}

Effect create_morsure_antique_effect(void) {
    return create_effect(EFFECT_BLEED, "Mâchoires primordiales !", 1, 0, 0, 0, 0, 0, 0, 0,
                        morsure_antique_tick, morsure_antique_remove);
}

Effect create_rugissement_abyssal_effect(void) {
    return create_effect(EFFECT_PARALYSIS, "Terreur des profondeurs !", 1, 0, 0, 0, 0, 0, 0, 0,
                        rugissement_abyssal_tick, rugissement_abyssal_remove);
}

Effect create_attaque_leurre_effect(void) {
    return create_effect(EFFECT_BLEED, "Embuscade des ombres !", 1, 0, 0, 0, 0, 0, 0, 0,
                        attaque_leurre_tick, attaque_leurre_remove);
}

Effect create_voile_obscurite_effect(void) {
    return create_effect(EFFECT_PARALYSIS, "Perdu dans l'obscurité !", 1, 0, 0, 0, 0, 0, 0, 0,
                        voile_obscurite_tick, voile_obscurite_remove);
}

Effect create_charge_perforante_effect(void) {
    return create_effect(EFFECT_BLEED, "Charge perforante !", 1, 0, 0, 0, 0, 0, 0, 0,
                        charge_perforante_tick, charge_perforante_remove);
}

Effect create_bonus_attaque_effect(void) {
    return create_effect(EFFECT_ATTACK_BOOST, "Bonus d'attaque !", 2, 0, 0, 0, 0, 0, 0, 0,
                        bonus_attaque_tick, bonus_attaque_remove);
}

Effect create_morsure_prehistorique_effect(void) {
    return create_effect(EFFECT_BLEED, "Fureur antique libérée !", 2, 0, 0, 0, 0, 0, 0, 0,
                        morsure_prehistorique_tick, morsure_prehistorique_remove);
}

Effect create_predateur_supreme_effect(void) {
    return create_effect(EFFECT_POISON, "Venin suprême !", 1, 0, 0, 0, 0, 0, 0, 0,
                        predateur_supreme_tick, predateur_supreme_remove);
}