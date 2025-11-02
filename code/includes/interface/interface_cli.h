//
// Created by cantonio on 02/10/2025.
//

#ifndef OCEANDEPTHS_INTERFACE_CLI_H
#define OCEANDEPTHS_INTERFACE_CLI_H


#include "interface_table.h"

// Declare the CLI vtable instance
extern InterfaceVTable cli_interface;

// You can also declare individual functions if you want to call them directly

// ========== MAP INTERFACE ==========
void cli_display_map(Map* map);

// ========== COMBAT INTERFACE ==========
void cli_display_combat_state(void);
void cli_display_combat_intro(Creature** creatures, int count);
void cli_display_round_header(int round_number);
void cli_display_victory(void);
void cli_display_defeat(void);
void cli_display_battle_start(void);
void cli_show_attack(EntityBase* attacker, EntityBase* target, int damage);
void cli_show_action(const char* entity_name, const char* action_name);
void cli_show_defeat_by(const char* enemy_name);

// ========== INVENTORY INTERFACE ==========
void cli_show_inventory(Inventory* inv);

// ========== INPUT INTERFACE ==========
void cli_wait_for_enter(const char* prompt);
int cli_get_choice(const char* prompt, int min, int max);
void cli_get_input(const char* prompt, char* result, size_t result_size);

// ========== SHOP INTERFACE ==========
void cli_display_shop(const char* shop_name, int player_gold, int refresh_cost,
                      const char** items, const int* prices, const int* stocks,
                      const int* rarities, const int* can_afford, int item_count);
void cli_show_purchase_success(const char* item_name, int price, int quantity);
void cli_show_purchase_failed(const char* reason);
void cli_show_sell_success(const char* item_name, int gold_received);
void cli_show_sell_failed(const char* reason);
void cli_show_shop_refreshed(void);
void cli_show_refresh_failed(int cost, int player_gold);
void cli_show_discount_applied(int discount_percent);
void cli_show_shop_restocked(void);

#endif //OCEANDEPTHS_INTERFACE_CLI_H