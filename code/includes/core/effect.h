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
 * @brief Applies all active effects on a given entity
 * @param target Entity base pointer
 */
void effect_tick(EntityBase* target);

/**
 * @brief Remove all inactive effects on a given entity
 * @param target Entity base pointer
 */
void effect_remove(EntityBase* target);

/**
 * @brief Creates an effect with the given parameters. (put negative values like -2 for health or
 * boost to inverse the effect, -2 at health will make it regenarate health instead of costing it)
 * @param name the effect name expects a char of length smaller than 30
 * @param display_message the message to display on effect tick
 * @param turns the duration of the effect
 * @param hp_cost the damage of the effect
 * @param attack_boost how much we boost the attack
 * @param defense_boost how much we boost the defense
 * @param oxygen_boost  how much we give oxygen to the entity
 * @param speed_boost  how much we boost the speed of the entity
 * @return
 */
Effect create_effect(const char* name, const char *display_message, int turns, int hp_cost,
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