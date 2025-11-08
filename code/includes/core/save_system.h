//
// Created for OceanDepth save system - Modular approach
//

#ifndef OCEANDEPTH_SAVE_SYSTEM_H
#define OCEANDEPTH_SAVE_SYSTEM_H

#include "player.h"

#define SAVE_FILE_PATH "../saves/oceandepth_save.dat"
#define SAVE_VERSION 1

// Component flags for modular saving
#define SAVE_PLAYER_DATA    (1 << 0)  // 0x01
#define SAVE_INVENTORY_DATA (1 << 1)  // 0x02
#define SAVE_EFFECTS_DATA   (1 << 2)  // 0x04
#define SAVE_PROGRESS_DATA  (1 << 3)  // 0x08
#define SAVE_MAP_DATA       (1 << 4)  // 0x10 - Map tiles and dimensions
#define SAVE_ALL_DATA       0xFF       // All flags

/**
 * @struct SaveData
 * @brief Contains all game state information for saving/loading
 */
typedef struct {
    int version;                    // Save file version for compatibility

    // Player basic info
    char player_name[30];
    int current_hp;
    int max_hp;
    int oxygen_level;
    int max_oxygen;
    int fatigue_level;
    int pearls;

    // Player stats
    int base_attack;
    int base_defense;
    int base_speed;

    // Inventory
    int inventory_count;
    struct {
        char name[30];
        ItemType type;
        int quantity;
        int oxygen_boost;
        int fatigue_relief;
        int hp_boost;
        int price;
        int action_count;
    } inventory_items[INVENTORY_SIZE];

    // Active effects
    int effects_count;
    struct {
        char name[30];
        int turns_left;
        int applies_next_turn;
        int attack_boost_flat;
        int defense_boost_flat;
        int speed_boost_flat;
        int oxygen_max_boost_flat;
        int hp_max_boost_flat;
        float attack_boost_percent;
        float defense_boost_percent;
        float speed_boost_percent;
        float oxygen_max_boost_percent;
        float hp_max_boost_percent;
    } effects[MAX_EFFECTS];

    // Game progression
    int current_difficulty;
    int battles_won;

    // Map and position data
    int map_seed;                  // Seed used to generate the map
    int player_current_row;        // Current player position (row)
    int player_current_col;        // Current player position (col)
    int player_max_row;            // Maximum unlocked position (row)
    int player_max_col;            // Maximum unlocked position (col)

    // Map data (legacy - for the generic tile system)
    int has_map_data;              // Flag: 1 if map data is present, 0 otherwise
    int map_width;                 // Map width
    int map_height;                // Map height
    char map_tiles[100][100];      // Map tiles (supports up to 100x100)
} SaveData;

/**
 * @brief Checks if a save file exists
 * @return 1 if save file exists, 0 otherwise
 */
int save_file_exists(void);

/**
 * @brief Deletes the save file
 * @return SUCCESS on success, error code on failure
 */
int delete_save_file(void);

// ============================================================================
// MODULAR SAVE FUNCTIONS - Save different components separately
// ============================================================================

/**
 * @brief Saves only player basic data (HP, oxygen, stats, pearls)
 * @param player Pointer to the player
 * @param save_data Pointer to SaveData structure to fill
 * @return SUCCESS on success, error code on failure
 */
int save_player_data(Player* player, SaveData* save_data);

/**
 * @brief Saves only inventory data
 * @param player Pointer to the player
 * @param save_data Pointer to SaveData structure to fill
 * @return SUCCESS on success, error code on failure
 */
int save_inventory_data(Player* player, SaveData* save_data);

/**
 * @brief Saves only active effects
 * @param player Pointer to the player
 * @param save_data Pointer to SaveData structure to fill
 * @return SUCCESS on success, error code on failure
 */
int save_effects_data(Player* player, SaveData* save_data);

/**
 * @brief Saves game progression data
 * @param difficulty Current difficulty level
 * @param battles_won Number of battles won
 * @param save_data Pointer to SaveData structure to fill
 * @return SUCCESS on success, error code on failure
 */
int save_progress_data(int difficulty, int battles_won, SaveData* save_data);

/**
 * @brief Saves map seed and player position data
 * @param player Pointer to the player (for position data)
 * @param map_seed The seed used to generate the map
 * @param save_data Pointer to SaveData structure to fill
 * @return SUCCESS on success, error code on failure
 */
int save_position_data(Player* player, int map_seed, SaveData* save_data);

/**
 * @brief Saves map data (tiles and dimensions)
 * @param width Map width
 * @param height Map height
 * @param map_tiles 2D array of map tiles
 * @param save_data Pointer to SaveData structure to fill
 * @return SUCCESS on success, error code on failure
 *
 * @example
 * char my_map[100][100];
 * // ... fill map ...
 * save_map_data(50, 50, my_map, &save_data);
 */
int save_map_data(int width, int height, char map_tiles[100][100], SaveData* save_data);

/**
 * @brief Loads map data from SaveData
 * @param save_data Pointer to SaveData structure to read from
 * @param width Pointer to store map width
 * @param height Pointer to store map height
 * @param map_tiles 2D array to store map tiles
 * @return SUCCESS on success, error code on failure
 *
 * @example
 * char my_map[100][100];
 * int w, h;
 * load_map_data(&save_data, &w, &h, my_map);
 */
int load_map_data(SaveData* save_data, int* width, int* height, char map_tiles[100][100]);

/**
 * @brief Gets the map seed from a save file
 * @param map_seed Pointer to store the map seed
 * @return SUCCESS on success, error code on failure
 */
int get_map_seed_from_save(int* map_seed);

/**
 * @brief Complete game save including map seed and positions
 * @param player Pointer to the player
 * @param difficulty Current difficulty level
 * @param battles_won Number of battles won
 * @param map_seed The seed used to generate the map
 * @return SUCCESS on success, error code on failure
 */
int save_game_complete(Player* player, int difficulty, int battles_won, int map_seed);

/**
 * @brief Modular save - save specific components using flags
 * @param player Pointer to the player
 * @param difficulty Current difficulty level
 * @param battles_won Number of battles won
 * @param save_flags Bitmask of what to save (SAVE_PLAYER_DATA | SAVE_INVENTORY_DATA | etc.)
 * @return SUCCESS on success, error code on failure
 *
 * @example
 * // Save only player and inventory
 * save_game_modular(player, difficulty, battles, SAVE_PLAYER_DATA | SAVE_INVENTORY_DATA);
 *
 * // Save everything
 * save_game_modular(player, difficulty, battles, SAVE_ALL_DATA);
 */
int save_game_modular(Player* player, int difficulty, int battles_won, int save_flags);

/**
 * @brief Loads specific components using flags
 * @param player Pointer to store the loaded player
 * @param difficulty Pointer to store difficulty
 * @param battles_won Pointer to store battles won
 * @param load_flags Bitmask of what to load
 * @return SUCCESS on success, error code on failure
 */
int load_game_modular(Player** player, int* difficulty, int* battles_won, int load_flags);

/**
 * @brief Extended save with map support
 * @param player Pointer to the player
 * @param difficulty Current difficulty level
 * @param battles_won Number of battles won
 * @param map_width Width of the map (0 if no map)
 * @param map_height Height of the map (0 if no map)
 * @param map_tiles Map tiles array (can be NULL if no map)
 * @param save_flags Bitmask of what to save
 * @return SUCCESS on success, error code on failure
 *
 * @example
 * // Save everything including map
 * char my_map[100][100];
 * save_game_extended(player, diff, battles, 50, 50, my_map, SAVE_ALL_DATA);
 *
 * // Save without map (pass 0, 0, NULL)
 * save_game_extended(player, diff, battles, 0, 0, NULL, SAVE_ALL_DATA);
 */
int save_game_extended(Player* player, int difficulty, int battles_won,
                       int map_width, int map_height, char map_tiles[100][100],
                       int save_flags);

/**
 * @brief Extended load with map support
 * @param player Pointer to store the loaded player
 * @param difficulty Pointer to store difficulty
 * @param battles_won Pointer to store battles won
 * @param map_width Pointer to store map width (can be NULL)
 * @param map_height Pointer to store map height (can be NULL)
 * @param map_tiles Map tiles array to load into (can be NULL)
 * @param load_flags Bitmask of what to load
 * @return SUCCESS on success, error code on failure
 *
 * @example
 * // Load everything including map
 * char my_map[100][100];
 * int w, h;
 * load_game_extended(&player, &diff, &battles, &w, &h, my_map, SAVE_ALL_DATA);
 *
 * // Load without map (pass NULL for map params)
 * load_game_extended(&player, &diff, &battles, NULL, NULL, NULL, SAVE_ALL_DATA);
 */
int load_game_extended(Player** player, int* difficulty, int* battles_won,
                       int* map_width, int* map_height, char map_tiles[100][100],
                       int load_flags);

// ============================================================================
// CONVENIENCE WRAPPERS - Simple interface for common use
// ============================================================================

/**
 * @brief Simple save - saves everything (convenience wrapper)
 * @param player Pointer to the player
 * @param difficulty Current difficulty level
 * @param battles_won Number of battles won
 * @return SUCCESS on success, error code on failure
 *
 * @note This is a convenience function that calls save_game_modular with SAVE_ALL_DATA
 */
static inline int save_game(Player* player, int difficulty, int battles_won) {
    return save_game_modular(player, difficulty, battles_won, SAVE_ALL_DATA);
}

/**
 * @brief Simple load - loads everything (convenience wrapper)
 * @param player Pointer to store the loaded player
 * @param difficulty Pointer to store difficulty
 * @param battles_won Pointer to store battles won
 * @return SUCCESS on success, error code on failure
 *
 * @note This is a convenience function that calls load_game_modular with SAVE_ALL_DATA
 */
static inline int load_game(Player** player, int* difficulty, int* battles_won) {
    return load_game_modular(player, difficulty, battles_won, SAVE_ALL_DATA);
}

#endif // OCEANDEPTH_SAVE_SYSTEM_H

