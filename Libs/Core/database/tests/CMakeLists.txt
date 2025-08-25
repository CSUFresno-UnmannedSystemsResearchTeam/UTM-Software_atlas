# =============================================================================
# Database Module Tests
# =============================================================================

# Collect test files
file(GLOB TEST_SOURCES *.cpp)

# Create test executable for each test file
foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    set(TEST_TARGET test_${TEST_NAME})
    
    add_executable(${TEST_TARGET} ${TEST_SOURCE})
    
    target_link_libraries(${TEST_TARGET}
        PRIVATE
            utm_database
            GTest::GTest
            GTest::Main
            Qt6::Test
    )
    
    # Add include path to parent module's include directory for testing private headers if needed
    target_include_directories(${TEST_TARGET}
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/../include
            ${CMAKE_CURRENT_SOURCE_DIR}/../src  # If testing internals
    )
    
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_TARGET})
    set_tests_properties(${TEST_NAME} PROPERTIES
        LABELS "unit;database"
        TIMEOUT 30
    )
endforeach()
