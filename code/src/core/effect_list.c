//
// Created by cantonio on 07/10/2025.
//

#include <stdio.h>
#include <string.h>
#include "core/entity.h"
#include "core/effect.h"

// No effect will not have a function since it does nothing


// ============ Function effects ============

// Poison - turn based damage

void effect_poison_tick(EntityBase* target, Effect* effect) {
    printf("💚 %s subit %d dégâts de poison!\n",
           target->name, effect->hp_cost);
    target->current_health_points -= effect->hp_cost;

    if (target->current_health_points < 0) target->current_health_points = 0;
}

void effect_poison_apply(EntityBase* target, Effect* effect) {
    printf("🧪 %s est empoisonné(e)!\n", target->name);
}

void effect_poison_remove(EntityBase* target, Effect* effect) {
    printf("✅ %s n'est plus empoisonné(e).\n", target->name);
}


// ============ Effect Creation ============

// Créer un effet configuré
Effect create_effect_poison(int puissance, int duree) {
    Effect e = {0};
    e.type = EFFECT_POISON;
    strcpy(e.name, "Poison");
    e.turns_left = duree;
    e.hp_cost = puissance;

    // dynamic functions
    e.on_tick = effect_poison_tick;
    e.on_apply = effect_poison_apply;
    e.on_remove = effect_poison_remove;

    // No stat modifier
    e.attack_boost = 0;
    e.defense_boost = 0;
    e.oxygen_boost = 0;
    e.speed_boost = 0;

    e.is_active = 1;

    return e;
}