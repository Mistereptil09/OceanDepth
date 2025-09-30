//
// Created by cantonio on 30/09/2025.
//
// No effect will not have a function since it does nothing

#include <stdio.h>
#include "core/effects.h"

#include <string.h>

#include "core/creatures.h"

// ============ FONCTIONS D'EFFETS DYNAMIQUES ============

// Poison - turn based damage
void effect_poison_tick(SeaCreature* target, Effect* effect) {
    printf("💚 %s subit %d dégâts de poison!\n",
           target->name, effect->power);
    target->health -= effect->power;

    if (target->health < 0) target->health = 0;
}
void effect_poison_apply(SeaCreature* target, Effect* effect) {
    printf("🧪 %s est empoisonné(e)!\n", target->name);
}
void effect_poison_remove(SeaCreature* target, Effect* effect) {
    printf("✅ %s n'est plus empoisonné(e).\n", target->name);
}


// ============ GESTION DES EFFETS ============

// Créer un effet configuré
Effect creer_effet_poison(int puissance, int duree) {
    Effect e = {0};
    e.type = EFFECT_POISON;
    strcpy(e.name, "Poison");
    e.duration = duree;
    e.power = puissance;

    // Fonctions dynamiques
    e.on_tick = effect_poison_tick;
    e.on_apply = effect_poison_apply;
    e.on_remove = effect_poison_remove;

    // Pas de modif de stats
    e.modif_attack = 0;
    e.modif_defense = 0;
    e.modif_speed = 0;

    e.est_actif = 1;
    return e;
}
void effect_poison_test(void) {
    printf("🧪 Testing poison effect...\n");

    // Create a test creature
    SeaCreature test_creature = {
        .name = "Test Shark",
        .health = 50,
        .max_health = 50
    };

    // Create poison effect
    Effect poison = {
        .type = EFFECT_POISON,
        .power = 5,
        .duration = 3,
        .on_tick = effect_poison_tick
    };

    // Test it
    printf("Before: %s has %d HP\n", test_creature.name, test_creature.health);
    effect_poison_tick(&test_creature, &poison);
    printf("After: %s has %d HP\n", test_creature.name, test_creature.health);
}