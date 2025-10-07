//
// Created by cantonio on 07/10/2025.
//

#ifndef OCEANDEPTH_EFFECT_LIST_H
#define OCEANDEPTH_EFFECT_LIST_H

#include "core/entity.h"
#include "core/effect.h"

// functions of effects
void effect_poison_tick(EntityBase* target, Effect* effect);
void effect_poison_apply(EntityBase* target, Effect* effet);
void effect_poison_remove(EntityBase* target, Effect* effet);

#endif //OCEANDEPTH_EFFECT_LIST_H