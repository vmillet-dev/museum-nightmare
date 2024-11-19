#include "../src/input/InputManager.hpp"
#include "../src/config/ConfigManager.hpp"
#include "../src/game/objects/GameObjectManager.hpp"
#include "../src/game/levels/Level1.hpp"
#include <spdlog/spdlog.h>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <chrono>
#include <thread>

void simulateKeyPress(sf::Keyboard::Key key) {
    auto& inputManager = game::InputManager::getInstance();
    sf::Event event;
    event.type = sf::Event::KeyPressed;
    event.key.code = key;
    inputManager.handleInput(event);
    spdlog::debug("Keyboard key {} pressed", static_cast<int>(key));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    event.type = sf::Event::KeyReleased;
    inputManager.handleInput(event);
    spdlog::debug("Keyboard key {} released", static_cast<int>(key));
}

int main() {
    spdlog::set_level(spdlog::level::debug);
    spdlog::info("Starting input test");

    // Initialize game objects and level
    game::GameObjectManager gameObjects;
    game::Level1::loadLevel(gameObjects);
    spdlog::info("Level 1 loaded successfully");

    // Verify level objects were loaded correctly
    const int EXPECTED_OBJECTS = 12;  // 4 border walls + 6 display shelves + 2 pedestals
    auto logGameState = [&gameObjects](const std::string& action) {
        spdlog::info("Game state after {}: Objects in level: {}",
                     action,
                     std::to_string(EXPECTED_OBJECTS));
    };
    logGameState("initial load");

    // Create a headless rendering context for testing
    sf::RenderWindow window(sf::VideoMode(800, 600), "Input Test", sf::Style::None);
    window.setVisible(false);

    auto& inputManager = game::InputManager::getInstance();
    inputManager.init();

    // Function to log game state after input
    auto logGameState = [&gameObjects](const std::string& action) {
        spdlog::info("Game state after {}: Objects in level: {}",
                     action,
                     std::to_string(EXPECTED_OBJECTS));
    };

    // Test keyboard input with level context
    spdlog::info("Testing keyboard input (ZQSD) with level loaded...");

    simulateKeyPress(sf::Keyboard::Z);  // Up
    gameObjects.update(0.016f);
    logGameState("Move Up");

    simulateKeyPress(sf::Keyboard::Q);  // Left
    gameObjects.update(0.016f);
    logGameState("Move Left");

    simulateKeyPress(sf::Keyboard::S);  // Down
    gameObjects.update(0.016f);
    logGameState("Move Down");

    simulateKeyPress(sf::Keyboard::D);  // Right
    gameObjects.update(0.016f);
    logGameState("Move Right");

    // Test controller button simulation with level context
    spdlog::info("Testing controller button simulation with level loaded...");

    // Simulate controller connected
    sf::Event event;
    event.type = sf::Event::JoystickConnected;
    event.joystickConnect.joystickId = 0;
    inputManager.handleInput(event);
    spdlog::info("Controller connected, testing input with level context");

    // Test controller buttons
    const char* buttonActions[] = {"A", "B", "X", "Y", "LB", "RB", "Select", "Start"};
    for (int i = 0; i < 8; ++i) {
        event.type = sf::Event::JoystickButtonPressed;
        event.joystickButton.joystickId = 0;
        event.joystickButton.button = i;
        inputManager.handleInput(event);
        gameObjects.update(0.016f);
        logGameState(std::string("Controller ") + buttonActions[i]);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        event.type = sf::Event::JoystickButtonReleased;
        inputManager.handleInput(event);
        gameObjects.update(0.016f);
    }

    spdlog::info("Input test with level context completed");
    return 0;
}
