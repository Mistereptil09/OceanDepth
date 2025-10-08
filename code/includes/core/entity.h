//
// Created by Yasmine Slamani on 02/10/2025.
//

#ifndef OCEANDEPTH_ENTITY_H
#define OCEANDEPTH_ENTITY_H
#define MAX_EFFECTS 10

typedef struct Effect Effect;
typedef enum {
    ENTITY_PLAYER,
    ENTITY_CREATURE
} EntityType;

typedef struct EntityBase{
    EntityType type;
    char name[30];

    int base_attack; // the attack raw's atk points
    int current_attack; // after applying effects

    int base_defense;
    int current_defense;

    int max_health_points;
    int current_health_points;

    int effects_number;
    Effect* effects;

    int is_alive;
} EntityBase;

// create function for creatures

/**
 * @brief Applies damage to the Entity's HP.
 * @param base Pointer to the Entity base.
 * @param hp Amount of damage to apply (must be >= 0).
 * @note HP cannot drop below 0.
 */
void entity_take_damage(EntityBase *base, int hp);

/**
 * @brief Recovers Entity HP.
 * @param base Pointer to the Entity base.
 * @param hp Amount of HP recovered (must be >= 0).
 * @return 0 if recovered normally, -1 if already at max.
 */
int entity_recover_hp(EntityBase *base, int hp);

EntityBase create_entity_base(EntityType type, char* name, int max_hp,int base_defense);

#endif //OCEANDEPTH_ENTITY_H
