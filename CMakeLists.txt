cmake_minimum_required(VERSION 3.20)
project(OceanDepth C)

set(CMAKE_C_STANDARD 99)

# Debug flags for development
set(CMAKE_C_FLAGS_DEBUG "-g -Wall -Wextra -Wshadow -Wstrict-prototypes")

# Release flags for submission
set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")

# Include directories (headers)
include_directories(${CMAKE_SOURCE_DIR}/code/includes)

# Source files (tous les .c dans src)
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/code/src/*.c")

# Main executable
add_executable(OceanDepth ${SOURCES}
        code/src/core/inventory.c
        code/includes/core/inventory.h
        code/src/core/item.c
        code/includes/core/item.h
        code/src/core/stats.c
        code/includes/core/stats.h
        code/tests/player_test.c
        code/src/core/creature.c
        code/includes/core/creature.h
        code/src/core/action.c
        code/includes/core/action.h
        code/src/core/effect.c
        code/includes/core/effect.h
        code/src/core/creature_data.c
        code/includes/core/creature_data.h)

# Optional: SDL version (future)
if(ENABLE_SDL)
    find_package(SDL2 REQUIRED)
    target_link_libraries(OceanDepth ${SDL2_LIBRARIES})
    target_compile_definitions(OceanDepth PRIVATE USE_SDL)
endif()

# Testing (optional but recommended)
if(ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
