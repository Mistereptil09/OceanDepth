//
// Created by Yasmine Slamani on 04/10/2025.
//

#ifndef OCEANDEPTH_EFFECT_H
#define OCEANDEPTH_EFFECT_H

typedef struct EntityBase EntityBase;
typedef struct Effect Effect;

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

    // HEALTH BOOST
    int attack_boost;
    int defense_boost;
    int oxygen_boost;
    int speed_boost;

    int is_active;
} Effect;

/**
 * @brief Applies and remove effect on target
 * @param target Entity base pointer
 * @param effect Effect pointer
 */
int generic_tick(EntityBase* target, Effect* effect);
int generic_remove(EntityBase* target, Effect* effect);

Effect create_effect(EffectType type, char *display_message, int turns,
                    int hp_cost, int oxygen_cost, int defense_cost,
                    int attack_boost, int defense_boost, int oxygen_boost, int speed_boost);

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
#endif //OCEANDEPTH_EFFECT_H