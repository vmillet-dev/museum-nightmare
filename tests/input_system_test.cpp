#include "../src/input/InputManager.hpp"
#include "../src/game/objects/GameObjectManager.hpp"
#include "../src/game/objects/Player.hpp"
#include "../src/config/ConfigManager.hpp"
#include <SFML/Graphics.hpp>
#include <spdlog/spdlog.h>

int main() {
    spdlog::set_level(spdlog::level::debug);
    spdlog::info("Starting input system test");

    // Initialize window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Input System Test");
    window.setFramerateLimit(60);

    // Initialize managers
    auto& inputManager = game::InputManager::getInstance();
    auto& gameObjectManager = game::GameObjectManager::getInstance();
    auto& configManager = game::ConfigManager::getInstance();

    // Load configuration
    configManager.loadConfig();

    // Initialize input system
    inputManager.init();

    // Create player
    auto player = std::make_unique<game::Player>(400.f, 300.f);
    gameObjectManager.addObject(std::move(player));

    sf::Clock clock;
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            inputManager.handleInput(event);
        }

        // Update
        inputManager.update();
        gameObjectManager.update(deltaTime);

        // Render
        window.clear(sf::Color::Black);
        gameObjectManager.render(window);
        window.display();

        // Debug info
        if (sf::Joystick::isConnected(0)) {
            spdlog::debug("Controller connected - X:{:.2f} Y:{:.2f}",
                sf::Joystick::getAxisPosition(0, sf::Joystick::X),
                sf::Joystick::getAxisPosition(0, sf::Joystick::Y));
        }
        window.quit();
    }

    return 0;
}
