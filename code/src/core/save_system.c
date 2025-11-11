//
// Created for OceanDepth save system
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "core/save_system.h"
#include "core/error_codes.h"
#include "core/player.h"
#include "core/entity.h"
#include "core/inventory_data.h"

// Platform-specific includes for directory creation
#ifdef _WIN32
    #include <direct.h>
    #define MKDIR(path) _mkdir(path)
#else
    #include <sys/stat.h>
    #include <sys/types.h>
    #define MKDIR(path) mkdir((path), 0755)
#endif

/**
 * @brief Creates the directory for the save file path if it doesn't exist
 * @return 0 on success, -1 on failure
 */
static int ensure_save_directory_exists(void) {
    // Extract directory path from SAVE_FILE_PATH
    char dir_path[256];
    strncpy(dir_path, SAVE_FILE_PATH, sizeof(dir_path) - 1);
    dir_path[sizeof(dir_path) - 1] = '\0';

    // Find last slash or backslash
    char *last_slash = strrchr(dir_path, '/');
    char *last_backslash = strrchr(dir_path, '\\');
    char *separator = (last_backslash > last_slash) ? last_backslash : last_slash;

    if (!separator) {
        // No directory in path, save in current directory
        return 0;
    }

    *separator = '\0';  // Terminate string at directory

    // Try to create each directory in the path
    for (char *p = dir_path; *p; p++) {
        if (*p == '/' || *p == '\\') {
            char temp = *p;
            *p = '\0';

            // Skip empty path components (like leading slash)
            if (p > dir_path) {
                MKDIR(dir_path);  // Ignore errors - might already exist
            }

            *p = temp;
        }
    }

    // Create the final directory
    if (MKDIR(dir_path) != 0 && errno != EEXIST) {
        return -1;  // Failed to create directory
    }

    return 0;
}

/**
 * @brief Converts SaveData structure to player data
 */
static Player* save_data_to_player(SaveData* save_data) {
    if (!save_data) return NULL;

    // Restore positions from save data
    Position current_pos = {save_data->player_current_row, save_data->player_current_col};
    Position max_pos = {save_data->player_max_row, save_data->player_max_col};

    Player* player = create_player(
        save_data->player_name,
        save_data->max_hp,
        save_data->base_defense,
        save_data->max_oxygen,
        current_pos,
        max_pos
    );

    if (!player) return NULL;

    // Restore player state
    player->base.current_health_points = save_data->current_hp;
    player->base.oxygen_level = save_data->oxygen_level;
    player->base.fatigue_level = save_data->fatigue_level;
    player->pearls = save_data->pearls;

    // Restore session-wide limits
    player->heal_uses_left = save_data->heal_uses_left;
    player->has_used_cave = save_data->has_used_cave;

    // Restore stats
    player->base.attack.base_value = save_data->base_attack;
    player->base.defense.base_value = save_data->base_defense;
    player->base.speed.base_value = save_data->base_speed;

    // Restore inventory by looking up items from inventory_data
    player->inventory.count = save_data->inventory_count;
    for (int i = 0; i < save_data->inventory_count && i < INVENTORY_SIZE; i++) {
        const char* item_name = save_data->inventory_items[i].name;
        int quantity = save_data->inventory_items[i].quantity;

        // Lookup the item to get its full definition including actions
        Item* looked_up_item = lookup_item_by_name(item_name, quantity);

        if (looked_up_item) {
            // Copy the fully initialized item (including actions)
            player->inventory.items[i] = *looked_up_item;
            free(looked_up_item);  // Free the temporary lookup result
        } else {
            // Item not found in lookup - restore what we can from save data
            Item* item = &player->inventory.items[i];
            strncpy(item->name, item_name, sizeof(item->name) - 1);
            item->name[sizeof(item->name) - 1] = '\0';
            item->type = save_data->inventory_items[i].type;
            item->quantity = quantity;
            item->oxygen_boost = save_data->inventory_items[i].oxygen_boost;
            item->fatigue_relief = save_data->inventory_items[i].fatigue_relief;
            item->hp_boost = save_data->inventory_items[i].hp_boost;
            item->price = save_data->inventory_items[i].price;
            item->actions = NULL;
            item->action_count = 0;
            printf("Warning: Item '%s' not found in item database\n", item_name);
        }
    }

    // Restore effects (simplified - doesn't restore function pointers)
    player->base.effects_number = save_data->effects_count;
    for (int i = 0; i < save_data->effects_count && i < MAX_EFFECTS; i++) {
        Effect* effect = &player->base.effects[i];
        strncpy(effect->name, save_data->effects[i].name, sizeof(effect->name) - 1);
        effect->name[sizeof(effect->name) - 1] = '\0';
        effect->turns_left = save_data->effects[i].turns_left;
        effect->applies_next_turn = save_data->effects[i].applies_next_turn;
        effect->attack_boost_flat = save_data->effects[i].attack_boost_flat;
        effect->defense_boost_flat = save_data->effects[i].defense_boost_flat;
        effect->speed_boost_flat = save_data->effects[i].speed_boost_flat;
        effect->oxygen_max_boost_flat = save_data->effects[i].oxygen_max_boost_flat;
        effect->hp_max_boost_flat = save_data->effects[i].hp_max_boost_flat;
        effect->attack_boost_percent = save_data->effects[i].attack_boost_percent;
        effect->defense_boost_percent = save_data->effects[i].defense_boost_percent;
        effect->speed_boost_percent = save_data->effects[i].speed_boost_percent;
        effect->oxygen_max_boost_percent = save_data->effects[i].oxygen_max_boost_percent;
        effect->hp_max_boost_percent = save_data->effects[i].hp_max_boost_percent;
        effect->is_active = 1;
        effect->on_tick = NULL; // Function pointers cannot be saved
        effect->display_message = NULL;
    }

    return player;
}

// ============================================================================
// MODULAR SAVE FUNCTIONS IMPLEMENTATION
// ============================================================================

int save_player_data(Player* player, SaveData* save_data) {
    if (!player || !save_data) return POINTER_NULL;

    // Copy player basic info
    strncpy(save_data->player_name, player->base.name, sizeof(save_data->player_name) - 1);
    save_data->player_name[sizeof(save_data->player_name) - 1] = '\0';

    save_data->current_hp = player->base.current_health_points;
    save_data->max_hp = player->base.max_health_points;
    save_data->oxygen_level = player->base.oxygen_level;
    save_data->max_oxygen = player->base.max_oxygen_level;
    save_data->fatigue_level = player->base.fatigue_level;
    save_data->pearls = player->pearls;

    // Copy session-wide limits
    save_data->heal_uses_left = player->heal_uses_left;
    save_data->has_used_cave = player->has_used_cave;

    // Copy player stats
    save_data->base_attack = player->base.attack.base_value;
    save_data->base_defense = player->base.defense.base_value;
    save_data->base_speed = player->base.speed.base_value;

    return SUCCESS;
}

int save_inventory_data(Player* player, SaveData* save_data) {
    if (!player || !save_data) return POINTER_NULL;

    // Copy inventory
    save_data->inventory_count = player->inventory.count;
    for (int i = 0; i < player->inventory.count && i < INVENTORY_SIZE; i++) {
        Item* item = &player->inventory.items[i];
        strncpy(save_data->inventory_items[i].name, item->name, sizeof(save_data->inventory_items[i].name) - 1);
        save_data->inventory_items[i].name[sizeof(save_data->inventory_items[i].name) - 1] = '\0';
        save_data->inventory_items[i].type = item->type;
        save_data->inventory_items[i].quantity = item->quantity;
        save_data->inventory_items[i].oxygen_boost = item->oxygen_boost;
        save_data->inventory_items[i].fatigue_relief = item->fatigue_relief;
        save_data->inventory_items[i].hp_boost = item->hp_boost;
        save_data->inventory_items[i].price = item->price;
        save_data->inventory_items[i].action_count = item->action_count;
    }

    return SUCCESS;
}

int save_effects_data(Player* player, SaveData* save_data) {
    if (!player || !save_data) return POINTER_NULL;

    // Copy active effects
    save_data->effects_count = player->base.effects_number;
    for (int i = 0; i < player->base.effects_number && i < MAX_EFFECTS; i++) {
        Effect* effect = &player->base.effects[i];
        if (!effect->is_active) continue;

        strncpy(save_data->effects[i].name, effect->name, sizeof(save_data->effects[i].name) - 1);
        save_data->effects[i].name[sizeof(save_data->effects[i].name) - 1] = '\0';
        save_data->effects[i].turns_left = effect->turns_left;
        save_data->effects[i].applies_next_turn = effect->applies_next_turn;
        save_data->effects[i].attack_boost_flat = effect->attack_boost_flat;
        save_data->effects[i].defense_boost_flat = effect->defense_boost_flat;
        save_data->effects[i].speed_boost_flat = effect->speed_boost_flat;
        save_data->effects[i].oxygen_max_boost_flat = effect->oxygen_max_boost_flat;
        save_data->effects[i].hp_max_boost_flat = effect->hp_max_boost_flat;
        save_data->effects[i].attack_boost_percent = effect->attack_boost_percent;
        save_data->effects[i].defense_boost_percent = effect->defense_boost_percent;
        save_data->effects[i].speed_boost_percent = effect->speed_boost_percent;
        save_data->effects[i].oxygen_max_boost_percent = effect->oxygen_max_boost_percent;
        save_data->effects[i].hp_max_boost_percent = effect->hp_max_boost_percent;
    }

    return SUCCESS;
}

int save_progress_data(int difficulty, int battles_won, SaveData* save_data) {
    if (!save_data) return POINTER_NULL;

    save_data->current_difficulty = difficulty;
    save_data->battles_won = battles_won;

    return SUCCESS;
}

int save_position_data(Player* player, int map_seed, SaveData* save_data) {
    if (!player || !save_data) return POINTER_NULL;

    save_data->map_seed = map_seed;
    save_data->player_current_row = player->current_position.row;
    save_data->player_current_col = player->current_position.col;
    save_data->player_max_row = player->max_position.row;
    save_data->player_max_col = player->max_position.col;

    return SUCCESS;
}

int save_map_data(int width, int height, char map_tiles[100][100], SaveData* save_data) {
    if (!save_data || !map_tiles) return POINTER_NULL;

    // Validate dimensions
    if (width < 1 || width > 100 || height < 1 || height > 100) {
        fprintf(stderr, "Invalid map dimensions: %dx%d (must be 1-100)\n", width, height);
        return UNPROCESSABLE_REQUEST_ERROR;
    }

    // Mark that map data is present
    save_data->has_map_data = 1;
    save_data->map_width = width;
    save_data->map_height = height;

    // Copy map tiles
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            save_data->map_tiles[y][x] = map_tiles[y][x];
        }
    }

    return SUCCESS;
}

int load_map_data(SaveData* save_data, int* width, int* height, char map_tiles[100][100]) {
    if (!save_data || !width || !height || !map_tiles) return POINTER_NULL;

    // Check if map data exists
    if (!save_data->has_map_data) {
        fprintf(stderr, "No map data in save file\n");
        return UNPROCESSABLE_REQUEST_ERROR;
    }

    // Validate dimensions
    if (save_data->map_width < 1 || save_data->map_width > 100 ||
        save_data->map_height < 1 || save_data->map_height > 100) {
        fprintf(stderr, "Invalid saved map dimensions: %dx%d\n",
                save_data->map_width, save_data->map_height);
        return UNPROCESSABLE_REQUEST_ERROR;
    }

    // Return dimensions
    *width = save_data->map_width;
    *height = save_data->map_height;

    // Copy map tiles
    for (int y = 0; y < save_data->map_height; y++) {
        for (int x = 0; x < save_data->map_width; x++) {
            map_tiles[y][x] = save_data->map_tiles[y][x];
        }
    }

    return SUCCESS;
}

int save_game_modular(Player* player, int difficulty, int battles_won, int save_flags) {
    if (!player) return POINTER_NULL;

    SaveData save_data = {0};
    save_data.version = SAVE_VERSION;

    // Save components based on flags
    if (save_flags & SAVE_PLAYER_DATA) {
        save_player_data(player, &save_data);
    }

    if (save_flags & SAVE_INVENTORY_DATA) {
        save_inventory_data(player, &save_data);
    }

    if (save_flags & SAVE_EFFECTS_DATA) {
        save_effects_data(player, &save_data);
    }

    if (save_flags & SAVE_PROGRESS_DATA) {
        save_progress_data(difficulty, battles_won, &save_data);
    }

    // Ensure save directory exists before writing
    if (ensure_save_directory_exists() != 0) {
        fprintf(stderr, "Error: Could not create save directory\n");
        return UNPROCESSABLE_REQUEST_ERROR;
    }

    // Write to file
    FILE* file = fopen(SAVE_FILE_PATH, "wb");
    if (!file) {
        perror("Error opening save file for writing");
        return UNPROCESSABLE_REQUEST_ERROR;
    }

    size_t written = fwrite(&save_data, sizeof(SaveData), 1, file);
    fclose(file);

    if (written != 1) {
        perror("Error writing save data");
        return UNPROCESSABLE_REQUEST_ERROR;
    }

    printf("Game saved successfully to %s\n", SAVE_FILE_PATH);
    return SUCCESS;
}

int load_game_modular(Player** player, int* difficulty, int* battles_won, int load_flags) {
    if (!player || !difficulty || !battles_won) return POINTER_NULL;

    // Open and read file
    FILE* file = fopen(SAVE_FILE_PATH, "rb");
    if (!file) {
        perror("Error opening save file for reading");
        return UNPROCESSABLE_REQUEST_ERROR;
    }

    SaveData save_data = {0};
    size_t read = fread(&save_data, sizeof(SaveData), 1, file);
    fclose(file);

    if (read != 1) {
        perror("Error reading save data");
        return UNPROCESSABLE_REQUEST_ERROR;
    }

    // Check version
    if (save_data.version != SAVE_VERSION) {
        fprintf(stderr, "Save file version mismatch! Expected %d, got %d\n",
                SAVE_VERSION, save_data.version);
        return UNPROCESSABLE_REQUEST_ERROR;
    }

    // Create player from saved data (always load player if data exists)
    *player = save_data_to_player(&save_data);
    if (!*player) {
        return MEMORY_ALLOCATION_ERROR;
    }

    // Load progression data if requested
    if (load_flags & SAVE_PROGRESS_DATA) {
        *difficulty = save_data.current_difficulty;
        *battles_won = save_data.battles_won;
    }

    printf("Game loaded successfully from %s\n", SAVE_FILE_PATH);
    return SUCCESS;
}

int save_game_extended(Player* player, int difficulty, int battles_won,
                       int map_width, int map_height, char map_tiles[100][100],
                       int save_flags) {
    if (!player) return POINTER_NULL;

    SaveData save_data = {0};
    save_data.version = SAVE_VERSION;

    // Save components based on flags
    if (save_flags & SAVE_PLAYER_DATA) {
        save_player_data(player, &save_data);
    }

    if (save_flags & SAVE_INVENTORY_DATA) {
        save_inventory_data(player, &save_data);
    }

    if (save_flags & SAVE_EFFECTS_DATA) {
        save_effects_data(player, &save_data);
    }

    if (save_flags & SAVE_PROGRESS_DATA) {
        save_progress_data(difficulty, battles_won, &save_data);
    }

    // Save map data if provided and flag is set
    if ((save_flags & SAVE_MAP_DATA) && map_tiles && map_width > 0 && map_height > 0) {
        save_map_data(map_width, map_height, map_tiles, &save_data);
    }

    // Write to file
    FILE* file = fopen(SAVE_FILE_PATH, "wb");
    if (!file) {
        perror("Error opening save file for writing");
        return UNPROCESSABLE_REQUEST_ERROR;
    }

    size_t written = fwrite(&save_data, sizeof(SaveData), 1, file);
    fclose(file);

    if (written != 1) {
        perror("Error writing save data");
        return UNPROCESSABLE_REQUEST_ERROR;
    }

    printf("Game saved successfully to %s", SAVE_FILE_PATH);
    if (save_data.has_map_data) {
        printf(" (including map: %dx%d)", save_data.map_width, save_data.map_height);
    }
    printf("\n");

    return SUCCESS;
}

int load_game_extended(Player** player, int* difficulty, int* battles_won,
                       int* map_width, int* map_height, char map_tiles[100][100],
                       int load_flags) {
    if (!player || !difficulty || !battles_won) return POINTER_NULL;

    // Open and read file
    FILE* file = fopen(SAVE_FILE_PATH, "rb");
    if (!file) {
        perror("Error opening save file for reading");
        return UNPROCESSABLE_REQUEST_ERROR;
    }

    SaveData save_data = {0};
    size_t read = fread(&save_data, sizeof(SaveData), 1, file);
    fclose(file);

    if (read != 1) {
        perror("Error reading save data");
        return UNPROCESSABLE_REQUEST_ERROR;
    }

    if (save_data.version != SAVE_VERSION) {
        fprintf(stderr, "Save file version mismatch! Expected %d, got %d\n",
                SAVE_VERSION, save_data.version);
        return UNPROCESSABLE_REQUEST_ERROR;
    }

    *player = save_data_to_player(&save_data);
    if (!*player) {
        return MEMORY_ALLOCATION_ERROR;
    }

    // Load progression data if requested
    if (load_flags & SAVE_PROGRESS_DATA) {
        *difficulty = save_data.current_difficulty;
        *battles_won = save_data.battles_won;
    }

    // Load map data if requested and provided
    if ((load_flags & SAVE_MAP_DATA) && map_width && map_height && map_tiles) {
        if (save_data.has_map_data) {
            int result = load_map_data(&save_data, map_width, map_height, map_tiles);
            if (result != SUCCESS) {
                fprintf(stderr, "Warning: Failed to load map data\n");
            } else {
                printf("Map loaded: %dx%d\n", *map_width, *map_height);
            }
        } else {
            printf("No map data in save file\n");
            *map_width = 0;
            *map_height = 0;
        }
    }

    printf("Game loaded successfully from %s\n", SAVE_FILE_PATH);
    return SUCCESS;
}

int save_file_exists(void) {
    FILE* file = fopen(SAVE_FILE_PATH, "rb");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}

int delete_save_file(void) {
    if (remove(SAVE_FILE_PATH) == 0) {
        printf("Save file deleted successfully.\n");
        return SUCCESS;
    }
    perror("Error deleting save file");
    return UNPROCESSABLE_REQUEST_ERROR;
}

int get_map_seed_from_save(int* map_seed) {
    if (!map_seed) return POINTER_NULL;

    FILE* file = fopen(SAVE_FILE_PATH, "rb");
    if (!file) {
        perror("Error opening save file for reading");
        return UNPROCESSABLE_REQUEST_ERROR;
    }

    SaveData save_data = {0};
    size_t read = fread(&save_data, sizeof(SaveData), 1, file);
    fclose(file);

    if (read != 1) {
        perror("Error reading save data");
        return UNPROCESSABLE_REQUEST_ERROR;
    }

    if (save_data.version != SAVE_VERSION) {
        fprintf(stderr, "Save file version mismatch! Expected %d, got %d\n",
                SAVE_VERSION, save_data.version);
        return UNPROCESSABLE_REQUEST_ERROR;
    }

    *map_seed = save_data.map_seed;
    return SUCCESS;
}

int save_game_complete(Player* player, int difficulty, int battles_won, int map_seed) {
    if (!player) return POINTER_NULL;

    SaveData save_data = {0};
    save_data.version = SAVE_VERSION;

    save_player_data(player, &save_data);
    save_inventory_data(player, &save_data);
    save_effects_data(player, &save_data);
    save_progress_data(difficulty, battles_won, &save_data);
    save_position_data(player, map_seed, &save_data);

    if (ensure_save_directory_exists() != 0) {
        fprintf(stderr, "Error: Could not create save directory\n");
        return UNPROCESSABLE_REQUEST_ERROR;
    }

    FILE* file = fopen(SAVE_FILE_PATH, "wb");
    if (!file) {
        perror("Error opening save file for writing");
        return UNPROCESSABLE_REQUEST_ERROR;
    }

    size_t written = fwrite(&save_data, sizeof(SaveData), 1, file);
    fclose(file);

    if (written != 1) {
        perror("Error writing save data");
        return UNPROCESSABLE_REQUEST_ERROR;
    }

    printf("Game saved successfully to %s\n", SAVE_FILE_PATH);
    return SUCCESS;
}
