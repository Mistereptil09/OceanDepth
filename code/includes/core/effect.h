//
// Created by Yasmine Slamani on 04/10/2025.
//

#ifndef OCEANDEPTH_EFFECT_H
#define OCEANDEPTH_EFFECT_H

typedef struct EntityBase EntityBase;
typedef struct Effect Effect;
typedef int (*FonctionEffect)(EntityBase *target);

typedef enum {
    EFFECT_POISON,
    EFFECT_PARALYSIS,
    EFFECT_DEFENSE_BOOST,
    EFFECT_ATTACK_BOOST,
    EFFECT_BLEED
} EffectType;

typedef struct Effect {
    char name[30];
    char *display_message;
    int turns_left;

    // DAMAGE
    int hp_cost;
    int oxygen_cost;
    int defense_cost;

    FonctionEffect on_tick;
    FonctionEffect on_remove; // Removes effect from target effect array/ is active = 0

    // HEALTH BOOST
    int attack_boost;
    int defense_boost;
    int oxygen_boost;
    int speed_boost;

    int is_active;
} Effect;


/**
 * @brief low-level function
 * @param type
 * @param display_message
 * @param turns
 * @param hp_cost
 * @param oxygen_cost
 * @param defense_cost
 * @param attack_boost
 * @param defense_boost
 * @param oxygen_boost
 * @param speed_boost
 * @param on_tick
 * @param on_remove
 * @return Effect
 */
Effect create_effect(EffectType type, char *display_message, int turns,
                     int hp_cost, int oxygen_cost, int defense_cost,
                     int attack_boost, int defense_boost, int oxygen_boost, int speed_boost,
                     FonctionEffect on_tick, FonctionEffect on_remove);

/**
 * @brief Copies an effect and duplicates its display_message (used to handle effect application)
 * @param src Source effect to copy
 * @return A new effect with duplicated display_message (message must be freed with free_effect_content)
 */
Effect effect_copy(const Effect* src);

/**
 * @brief Frees only the display_message content of an effect
 * @param effect
 */
void free_effect_content(Effect* effect);


int etreinte_tentaculaire_tick(EntityBase *target);
int etreinte_tentaculaire_remove(EntityBase *target);

int nuage_encre_tick(EntityBase *target);
int nuage_encre_remove(EntityBase *target);

int pince_ecrasante_tick(EntityBase *target);
int pince_ecrasante_remove(EntityBase *target);

int carapace_durcie_tick(EntityBase *target);
int carapace_durcie_remove(EntityBase *target);

int piqure_tentaculaire_tick(EntityBase *target);
int piqure_tentaculaire_remove(EntityBase *target);

int piqure_paralysante_tick(EntityBase *target);
int piqure_paralysante_remove(EntityBase *target);

int morsure_tick(EntityBase *target);
int morsure_remove(EntityBase *target);

int frenesie_sanguinaire_tick(EntityBase *target);
int frenesie_sanguinaire_remove(EntityBase *target);

int morsure_antique_tick(EntityBase *target);
int morsure_antique_remove(EntityBase *target);

int rugissement_abyssal_tick(EntityBase *target);
int rugissement_abyssal_remove(EntityBase *target);

int attaque_leurre_tick(EntityBase *target);
int attaque_leurre_remove(EntityBase *target);

int voile_obscurite_tick(EntityBase *target);
int voile_obscurite_remove(EntityBase *target);

int charge_perforante_tick(EntityBase *target);
int charge_perforante_remove(EntityBase *target);

int bonus_attaque_tick(EntityBase *target);
int bonus_attaque_remove(EntityBase *target);

int morsure_prehistorique_tick(EntityBase *target);
int morsure_prehistorique_remove(EntityBase *target);

int predateur_supreme_tick(EntityBase *target);
int predateur_supreme_remove(EntityBase *target);

// Fonctions de création pour chaque effet
Effect create_etreinte_tentaculaire_effect(void);
Effect create_nuage_encre_effect(void);
Effect create_pince_ecrasante_effect(void);
Effect create_carapace_durcie_effect(void);
Effect create_piqure_tentaculaire_effect(void);
Effect create_piqure_paralysante_effect(void);
Effect create_morsure_effect(void);
Effect create_frenesie_sanguinaire_effect(void);
Effect create_morsure_antique_effect(void);
Effect create_rugissement_abyssal_effect(void);
Effect create_attaque_leurre_effect(void);
Effect create_voile_obscurite_effect(void);
Effect create_charge_perforante_effect(void);
Effect create_bonus_attaque_effect(void);
Effect create_morsure_prehistorique_effect(void);
Effect create_predateur_supreme_effect(void);

#endif //OCEANDEPTH_EFFECT_H