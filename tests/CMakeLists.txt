# Tests configuration for OceanDepth

# We assume ENABLE_TESTS and enable_testing() were handled by parent CMakeLists.

# Use parent include directories and source lists:
# - CORE_SOURCES, HELPER_SOURCES, INTERFACE_SOURCES are defined in the top-level CMakeLists.

set(TEST_DIR ${CMAKE_SOURCE_DIR}/code/tests)

file(GLOB TEST_INTERFACE_SOURCES "${TEST_DIR}/interface/*_test.c")

function(od_add_test target_name source_file)
    add_executable(${target_name}
            ${TEST_DIR}/${source_file}
            ${CORE_SOURCES}
            ${HELPER_SOURCES}
            ${INTERFACE_SOURCES}
            ${TEST_INTERFACE_SOURCES}
    )
    # Ensure C standard matches the project
    set_property(TARGET ${target_name} PROPERTY C_STANDARD 99)
    add_test(NAME ${target_name} COMMAND ${target_name})
endfunction()

# Automatically add all *_test.c from code/tests as CTest tests
file(GLOB TEST_SOURCES "${TEST_DIR}/*_test.c")
foreach(TEST_SRC ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)
    add_executable(${TEST_NAME}
            ${TEST_SRC}
            ${CORE_SOURCES}
            ${HELPER_SOURCES}
            ${INTERFACE_SOURCES}
            ${TEST_INTERFACE_SOURCES}
    )
    set_property(TARGET ${TEST_NAME} PROPERTY C_STANDARD 99)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()