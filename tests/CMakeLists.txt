# Enable testing
enable_testing()

# Find GTest package
find_package(GTest REQUIRED)

# Create test executable
add_executable(input_tests
    input/PlayerInputTest.cpp
    input/EnemyAITest.cpp
    input/ControllerHotplugTest.cpp
)

# Link against GTest and our game library
target_link_libraries(input_tests
    PRIVATE
    GTest::GTest
    GTest::Main
    sfml-system
    sfml-window
    sfml-graphics
    game_lib  # Assuming this is the name of your main game library
)

# Add tests to CTest
add_test(NAME input_tests COMMAND input_tests)

# Set working directory for tests
set_tests_properties(input_tests PROPERTIES
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Copy test resources if needed
file(COPY ${CMAKE_SOURCE_DIR}/assets/config.toml
     DESTINATION ${CMAKE_BINARY_DIR}/assets)
