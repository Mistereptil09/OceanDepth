//
// Created by Yasmine Slamani on 26/10/2025.
//

#ifndef OCEANDEPTH_INVENTORY_DATA_H
#define OCEANDEPTH_INVENTORY_DATA_H
#include "item.h"

Item create_epee();

Item create_harpon();

Item create_combinaison();

Item create_oxygen_capsule(int quantity);

Item create_fatigue_stimulant(int quantity);

Item create_health_kit(int quantity);

/**
 * @brief Lookup an item by name and return a new instance with full actions
 * @param name The item name to look up
 * @param quantity Quantity for consumables (ignored for weapons/equipment)
 * @return Pointer to newly allocated item, or NULL if not found. Caller must free.
 */
Item* lookup_item_by_name(const char* name, int quantity);

#endif //OCEANDEPTH_INVENTORY_DATA_H