# Test configuration
enable_testing()

# Add test executable
add_executable(${PROJECT_NAME}_test
    tests/main.cpp
    tests/game/ParallaxLayerTest.cpp
)

# Link test dependencies
target_link_libraries(${PROJECT_NAME}_test PRIVATE
    ${PROJECT_NAME}_lib
    GTest::gtest
    GTest::gtest_main
)

# Include directories for tests
target_include_directories(${PROJECT_NAME}_test PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${GTEST_INCLUDE_DIRS}
)

# Register test
add_test(NAME ${PROJECT_NAME}_test COMMAND ${PROJECT_NAME}_test)

# Set test properties for xvfb
set_tests_properties(${PROJECT_NAME}_test PROPERTIES
    ENVIRONMENT "DISPLAY=:99"
)
