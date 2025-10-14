cmake_minimum_required(VERSION 3.20)
project(OceanDepth C)

# Option to enable tests (default ON)
option(ENABLE_TESTS "Build and run tests" ON)

set(CMAKE_C_STANDARD 99)

# Debug flags for development
set(CMAKE_C_FLAGS_DEBUG "-g -Wall -Wextra -Wshadow -Wstrict-prototypes")

# Release flags for submission
set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/code/includes)
# Source files
file(GLOB_RECURSE CORE_SOURCES "${CMAKE_SOURCE_DIR}/code/src/core/*.c")
file(GLOB_RECURSE INTERFACE_SOURCES "${CMAKE_SOURCE_DIR}/code/src/interface/*.c")
file(GLOB_RECURSE HELPER_SOURCES "${CMAKE_SOURCE_DIR}/code/src/helper/*.c")

# Debug statement
message(STATUS "Core sources found: ${CORE_SOURCES}")
message(STATUS "Interface sources found: ${INTERFACE_SOURCES}")
message(STATUS "Helper sources found: ${HELPER_SOURCES}")

set(SOURCES
        code/src/main.c
        ${CORE_SOURCES}
        ${INTERFACE_SOURCES}
        ${HELPER_SOURCES}
)

# Main executable
add_executable(oceandepths ${SOURCES})

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
