//
// Created by anton on 11/2/2025.
//

#ifndef OCEANDEPTH_ITEM_DATA_H
#define OCEANDEPTH_ITEM_DATA_H

#include "item.h"

// ========== WEAPONS ==========

/**
 * @brief Powerful three-pronged weapon with massive damage potential
 * @return Trident weapon item
 */
Item create_trident(void);

/**
 * @brief High-tech blade that uses sonic waves for devastating attacks
 * @return Sonic Blade weapon item
 */
Item create_sonic_blade(void);

/**
 * @brief Mystical coral staff with healing and poison abilities
 * @return Coral Staff weapon item
 */
Item create_coral_staff(void);

/**
 * @brief Legendary hammer from the abyss with crushing power
 * @return Abyssal Hammer weapon item
 */
Item create_abyssal_hammer(void);

/**
 * @brief Electric net launcher that paralyzes enemies
 * @return Net Launcher weapon item
 */
Item create_net_launcher(void);

// ========== CONSUMABLES ==========

/**
 * @brief Enhanced oxygen capsule with double capacity
 * @param quantity Number of capsules
 * @return Mega Oxygen consumable item
 */
Item create_mega_oxygen(int quantity);

/**
 * @brief Powerful stimulant that drastically reduces fatigue
 * @param quantity Number of shots
 * @return Adrenaline Shot consumable item
 */
Item create_adrenaline_shot(int quantity);

/**
 * @brief Advanced medical kit with high healing capacity
 * @param quantity Number of kits
 * @return Emergency Medkit consumable item
 */
Item create_emergency_medkit(int quantity);

/**
 * @brief Ultimate consumable that restores everything
 * @param quantity Number of restore items
 * @return Full Restore consumable item
 */
Item create_full_restore(int quantity);

/**
 * @brief Energy drink that provides minor boosts to all stats
 * @param quantity Number of drinks
 * @return Energy Drink consumable item
 */
Item create_energy_drink(int quantity);

/**
 * @brief Antidote that cures poison and minor ailments
 * @param quantity Number of antidotes
 * @return Antidote consumable item
 */
Item create_antidote(int quantity);

/**
 * @brief Advanced medical nanobots for healing and repair
 * @param quantity Number of nanobot doses
 * @return Nanobots consumable item
 */
Item create_nanobots(int quantity);

// ========== ARMOR/EQUIPMENT ==========

/**
 * @brief Upgraded diving suit with enhanced defense capabilities
 * @return Reinforced Suit equipment item
 */
Item create_reinforced_suit(void);

/**
 * @brief Advanced armor with switchable power and defense modes
 * @return Power Armor equipment item
 */
Item create_power_armor(void);

/**
 * @brief Stealth suit that provides camouflage and evasion
 * @return Stealth Suit equipment item
 */
Item create_stealth_suit(void);

#endif //OCEANDEPTH_ITEM_DATA_H
