# OceanDepth - Team Development Guidelines

*This is subject to change. Feel free to modify it if you want. 
The main purpose is to ensure we're all on the same page.*

## File Structure

```
oceandepths/
├── documentation/
│   ├── struct.md          # Data structures
│   ├── guidelines.md      # This file
│   └── api.md             # Function documentation
├── code/                  # Main code directory (matches submission)
│   ├── config.txt         # Configuration (optional)
│   ├── src/               # All .c files
│   │   ├── core/          # Game logic (SDL-independent)
│   │   │   ├── creatures.c
│   │   │   ├── player.c
│   │   │   ├── combat.c
│   │   │   ├── inventory.c
│   │   │   ├── map.c
│   │   │   └── save.c
│   │   ├── interface/     # Display abstraction
│   │   │   ├── display.c
│   │   │   ├── terminal_ui.c
│   │   │   └── sdl_ui.c   # (future, if we get there)
│   │   └── main.c
│   ├── include/           # All .h files
│   │   ├── core/
│   │   │   ├── creatures.h
│   │   │   ├── player.h
│   │   │   ├── combat.h
│   │   │   ├── inventory.h
│   │   │   ├── map.h
│   │   │   └── save.h
│   │   ├── interface/      
│   │   │   ├── display.h
│   │   │   ├── terminal_ui.h
│   │   │   └── sdl_ui.h
│   │   └── common.h       # Common types/constants
│   └── saves/             # Save game files
├── CMakeLists.txt     # CMake build configuration (CLion)
├── identifiants.txt      # Student names
├── README.md             # Documentation and instructions
└── PROGRESSION.md        # Progress steps with screenshots
```

**Goal**: Keep game logic separate from interface (`/core` vs `/interface`). 
Structure should primarily support game logic, not interfaces.

## Naming Conventions (C Language)

### Variables and Functions

// c++ is used to have the ide format support (even though we will do c...)
```c++
// Variables: snake_case in English
int max_health_points;
int oxygen_level;
char creature_name[30];

// Functions: verb_noun in English  
void initialize_player(Player* player);
int calculate_damage(int attack, int defense);
SeaCreature* generate_random_creature(CreatureType type);
```

### Constants
```c++
// Preprocessor constants: UPPERCASE with prefix
#define MAX_CREATURES 4
#define OXYGEN_CRITICAL_THRESHOLD 10
#define BASE_ATTACK_COST 2

// File-scope constants (when appropriate)
static const int MAX_INVENTORY_SIZE = 8;
```

### Types
```c++
// Structures: PascalCase in English
typedef struct {
    int id;
    char name[30];
} SeaCreature;

// Enumerations: TYPE_VALUE in English
typedef enum {
    KRAKEN,
    TIGER_SHARK,
    BLUE_JELLYFISH,
    SWORDFISH,
    GIANT_CRAB
} CreatureType;

typedef enum {
    ACTION_ATTACK = 1,
    ACTION_SKILL,
    ACTION_ITEM,
    ACTION_END_TURN
} PlayerAction;
```

## Essential Coding Patterns

### 1. Memory Management (Critical)
```c++
// ALWAYS check malloc returns
SeaCreature* creatures = malloc(sizeof(SeaCreature) * count);
if (!creatures) {
    return ERROR_MEMORY_ALLOCATION;  // or handle appropriately
}

// ALWAYS initialize allocated memory
memset(creatures, 0, sizeof(SeaCreature) * count);

// ALWAYS free what you allocate
free(creatures);
creatures = NULL;  // Prevent double-free
```

### 2. Input Validation
```c++
// Validate function parameters
int calculate_damage(int attack, int defense) {
    if (attack < 0 || defense < 0) {
        return 0;  // or appropriate error value
    }
    // ... rest of function
}

// Validate user input
int get_user_choice(int min, int max) {
    char buffer[100];
    int choice;
    
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        return -1;  // Error reading input
    }
    
    if (sscanf(buffer, "%d", &choice) != 1 || choice < min || choice > max) {
        return -1;  // Invalid input
    }
    
    return choice;
}
```

### 3. Error Handling
```c++
// Use consistent error codes
typedef enum {
    SUCCESS = 0,
    ERROR_MEMORY_ALLOCATION,
    ERROR_INVALID_INPUT,
    ERROR_FILE_IO
} ErrorCode;

// Return error codes, don't just exit()
ErrorCode save_game(const GameState* state, const char* filename) {
    if (!state || !filename) {
        return ERROR_INVALID_INPUT;
    }
    
    FILE* file = fopen(filename, "w");
    if (!file) {
        return ERROR_FILE_IO;
    }
    
    // ... save logic
    
    fclose(file);
    return SUCCESS;
}
```

### 4. Header Guards
```c++
// In every .h file
#ifndef CREATURES_H
#define CREATURES_H

// ... header content

#endif
```

## Function Documentation
```c++
/**
 * @brief Generates sea creatures for combat
 * @param depth Current ocean depth (affects difficulty)
 * @param creature_count Pointer to store number of creatures generated
 * @return Array of creatures, or NULL on error
 * @note Caller must free returned array with free()
 */
SeaCreature* generate_creatures(int depth, int* creature_count);
```

## CLion Development Workflow

### Project Setup
1. **Open CLion** and create new C project from existing sources
2. **CMake configuration** will be auto-detected
3. **Enable** Valgrind integration: `Settings → Build → Valgrind Memcheck`

### CLion-Specific Features

#### Memory Analysis (Built-in)
- **Run with Valgrind**: Click dropdown next to Run button → Choose "Run with Valgrind Memcheck"
- **Address Sanitizer**: Add `-fsanitize=address` to CMake flags for real-time leak detection
- **View results** directly in CLion's Problems panel

#### Code Quality Tools
- **Static analysis**: CLion highlights potential issues in real-time
- **Code inspection**: `Code → Inspect Code` for full project analysis
- **Refactoring**: `Ctrl+Shift+Alt+T` for safe code restructuring

#### Debugging
- **Integrated GDB**: Set breakpoints, inspect variables visually
- **Memory view**: Debug → View Memory to inspect allocated blocks
- **Call stack**: Full visualization of function calls

### Build Configuration (CMakeLists.txt)

Find `code/CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.20)
project(oceanDepths C)

set(CMAKE_C_STANDARD 99)

# Debug flags for development
set(CMAKE_C_FLAGS_DEBUG "-g -Wall -Wextra -Wshadow -Wstrict-prototypes")

# Release flags for submission
set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")

# Include directories
include_directories(include)

# Source files
file(GLOB_RECURSE CORE_SOURCES "src/core/*.c")
file(GLOB_RECURSE INTERFACE_SOURCES "src/interface/*.c")

set(SOURCES 
    src/main.c
    ${CORE_SOURCES}
    ${INTERFACE_SOURCES}
)

# Main executable
add_executable(oceandepths ${SOURCES})

# Optional: SDL version (future)
if(ENABLE_SDL)
    find_package(SDL2 REQUIRED)
    target_link_libraries(oceandepths ${SDL2_LIBRARIES})
    target_compile_definitions(oceandepths PRIVATE USE_SDL)
endif()

# Testing (optional but recommended)
if(ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
```

### Memory Testing (Integrated)
```bash
# From terminal (backup method)
cd code && mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make
valgrind --leak-check=full ./oceandepths
```

### Team Coordination with CLion

#### Version Control (Git Integration)
- **Built-in Git**: VCS menu for all operations
- **Merge conflicts**: Visual resolution tool
- **Commit templates**: Enforce commit message format
- **Code review**: Compare branches visually

#### Shared Configuration
Add to `.idea/codeStyles/Project.xml` (auto-generated):
```xml
<code_scheme name="Project" version="173">
  <option name="C_BRACE_STYLE" value="Next line if wrapped" />
  <option name="C_INDENT_SIZE" value="4" />
</code_scheme>
```

## Team Coordination

### File Ownership (Suggested)
- **Person A**: `creatures.c/h`, `combat.c/h`
- **Person B**: `player.c/h`, `inventory.c/h`
- **Person C**: `map.c/h`, `save.c/h`
- **Everyone**: `main.c`, `terminal_ui.c/h`

### Integration Points
- Define interfaces in header files first
- Use `common.h` for shared types/constants
- Test integration frequently
- Communicate changes to shared structures

## Quick Reference

### Checklist Before Push
- [ ] Code compiles without warnings in CLion
- [ ] No memory leaks (CLion's Valgrind integration shows clean)
- [ ] Functions have basic documentation
- [ ] Error cases handled (malloc failures, invalid input)
- [ ] Code automatically formatted (`Ctrl+Alt+L` in CLion)

### Submission Preparation
- [ ] **Create Makefile** for submission compatibility (graders may not use CMake)
- [ ] **Test compilation** with both CMake and Makefile
- [ ] **Final Valgrind run** outside CLion to be sure

### Red Flags
- `malloc()` without checking return value
- Missing `free()` for every `malloc()`
- Using uninitialized variables
- No input validation
- Crashing on invalid input

**Remember**: The goal is working, leak-free code that the team can understand and maintain. When in doubt, prioritize simplicity and safety over cleverness.