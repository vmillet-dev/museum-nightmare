#include "../src/input/InputManager.hpp"
#include "../src/config/ConfigManager.hpp"
#include <spdlog/spdlog.h>
#include <SFML/Window/Event.hpp>
#include <chrono>
#include <thread>

void simulateKeyPress(sf::Keyboard::Key key) {
    auto& inputManager = game::InputManager::getInstance();
    sf::Event event;
    event.type = sf::Event::KeyPressed;
    event.key.code = key;
    inputManager.handleInput(event);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    event.type = sf::Event::KeyReleased;
    inputManager.handleInput(event);
}

int main() {
    spdlog::set_level(spdlog::level::debug);
    spdlog::info("Starting input test");

    auto& inputManager = game::InputManager::getInstance();
    inputManager.init();

    // Test keyboard input (ZQSD)
    spdlog::info("Testing keyboard input (ZQSD)...");
    simulateKeyPress(sf::Keyboard::Z);  // Up
    simulateKeyPress(sf::Keyboard::Q);  // Left
    simulateKeyPress(sf::Keyboard::S);  // Down
    simulateKeyPress(sf::Keyboard::D);  // Right

    // Test controller button simulation
    spdlog::info("Testing controller button simulation...");

    // Simulate controller connected
    sf::Event event;
    event.type = sf::Event::JoystickConnected;
    event.joystickConnect.joystickId = 0;
    inputManager.handleInput(event);

    // Test controller buttons
    for (int i = 0; i < 8; ++i) {
        event.type = sf::Event::JoystickButtonPressed;
        event.joystickButton.joystickId = 0;
        event.joystickButton.button = i;
        inputManager.handleInput(event);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        event.type = sf::Event::JoystickButtonReleased;
        inputManager.handleInput(event);
    }

    spdlog::info("Input test completed");
    return 0;
}
