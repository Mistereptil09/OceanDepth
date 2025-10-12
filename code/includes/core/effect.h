//
// Created by Yasmine Slamani on 04/10/2025.
//

#ifndef OCEANDEPTH_EFFECT_H
#define OCEANDEPTH_EFFECT_H

typedef struct EntityBase EntityBase;
typedef struct Effect Effect;

typedef struct Effect {
    char name[30];
    char *display_message;
    int turns_left;

    // Per-turn costs
    int hp_cost;
    int oxygen_cost;

    // FLAT modifiers (once, for duration)
    int attack_boost_flat;
    int defense_boost_flat;
    int speed_boost_flat;
    int oxygen_max_boost_flat;
    int hp_max_boost_flat;

    // PERCENTAGE modifiers (once, for duration)
    float attack_boost_percent;   // e.g., 0.20 = +20%
    float defense_boost_percent;
    float speed_boost_percent;
    float oxygen_max_boost_percent;
    float hp_max_boost_percent;

    int is_active;
} Effect;

/**
 * @brief Applies all active effects on a given entity
 * @param target Entity base pointer
 */
void effect_tick(EntityBase* target, Effect* effect);

/**
 * Apply effects, removes effects and make them tick each turn. Clear up decayed effects
 * @param base Base to make effect tick
 */
void all_effects_tick(EntityBase* base);



/**
 * @brief Remove all inactive effects on a given entity
 * @param target Entity base pointer
 * @param effect effect to remove
 */
void effect_remove(EntityBase* target, Effect* effect);

/**
 * @brief creates an effect with the given parameters, you can make a ressource parameter negative to instead give
 * (for "cost" variables)
 * @param name name of the effect
 * @param display_message the message to display when the effect is triggered
 * @param turns the number of turns the effect will be active
 * @param hp_cost the amount of hp to remove from the entity affected
 * @param oxygen_cost the amount of oxygen to remove from the entity affected (won't affect the creatures)
 * @param attack_boost_flat attack boost in flat (+10 to base for example)
 * @param defense_boost_flat defense boost in flat
 * @param speed_boost_flat speed boost in flat
 * @param oxygen_max_boost_flat max oxygen boost in flat (won't affect creatures)
 * @param hp_max_boost_flat max health points boost in flat
 * @param attack_boost_percent attack boost in percentage (multiplies the base attack of an entity so base * 0.3 + base for example)
 * @param defense_boost_percent defense boost in percentage
 * @param speed_boost_percent speed boost in percentage
 * @param oxygen_max_boost_percent max oxygen boost in percentage
 * @param hp_max_boost_percent max health points boost in percentage
 * @return
 */
Effect create_effect(const char* name, const char *display_message, const int turns,
                     // ressources
                     const int hp_cost, const int oxygen_cost,
                     // flat modifiers
                     const int attack_boost_flat, const int defense_boost_flat, const int speed_boost_flat,
                     const int oxygen_max_boost_flat, const int hp_max_boost_flat,
                     // percent modifiers
                     const float attack_boost_percent, const float defense_boost_percent,const float speed_boost_percent,
                     const float oxygen_max_boost_percent, const float hp_max_boost_percent);

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