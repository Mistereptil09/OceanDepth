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
        code/src/main.c
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