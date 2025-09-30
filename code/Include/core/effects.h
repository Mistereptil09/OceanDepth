//
// Created by cantonio on 30/09/2025.
//

#ifndef OCEANDEPTHS_EFFECTS_H
#define OCEANDEPTHS_EFFECTS_H

typedef enum { // We start by effect that way the ide can list the enums by tapping EFFECT...
    EFFECT_NOTHING,
    EFFECT_POISON,
} EffectType;

// we forward the declaration of creature since we can't import it.
typedef struct SeaCreature SeaCreature; // we say that it exists without definition
typedef struct Effect Effect;
typedef void (*FonctionEffet)(SeaCreature* cible, Effect* effect);

struct Effect{
    EffectType type;
    char name[30];
    int duration;
    int power;           // Effect potency e.g : damage per turn, multiplier for power boost etc...

    // Pour les effets dynamiques (poison, saignement...)
    FonctionEffet on_tick;   // Appelé à chaque tour
    FonctionEffet on_apply;  // Appelé à l'application
    FonctionEffet on_remove; // Appelé à la fin de l'effet

    // Pour les effets de stats (rage, bouclier...)
    int modif_attack;
    int modif_defense;
    int modif_speed;

    int est_actif;
};

// functions of effects
void effect_poison_tick(SeaCreature* target, Effect* effect);
void effect_poison_apply(SeaCreature* target, Effect* effet);
void effect_poison_remove(SeaCreature* target, Effect* effet);
void effect_poison_test(void);

#endif //OCEANDEPTHS_EFFECTS_H