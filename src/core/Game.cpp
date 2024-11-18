#include "Game.hpp"
#include "../screens/MainMenuScreen.hpp"
#include <spdlog/spdlog.h>

namespace game {

Game::Game() : window(
    sf::VideoMode(
        ConfigManager::getInstance().getWindowWidth(),
        ConfigManager::getInstance().getWindowHeight()
    ),
    ConfigManager::getInstance().getWindowTitle()
) {
    spdlog::info("Initializing game...");

    // Initialize managers
    auto& configManager = ConfigManager::getInstance();
    auto& inputManager = InputManager::getInstance();
    inputManager.init();

    // Load initial screen
    ScreenManager::getInstance().pushScreen(std::make_unique<MainMenuScreen>(*this));

    spdlog::info("Game initialized successfully");
}

void Game::run() {
    sf::Clock clock;

    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        handleEvents();
        update(deltaTime.asSeconds());
        render();
    }
}

void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        // Let current screen handle the event
        if (!ScreenManager::getInstance().isEmpty()) {
            ScreenManager::getInstance().handleInput(event);
        }
    }
}

void Game::update(float deltaTime) {
    InputManager::getInstance().update();
    if (!ScreenManager::getInstance().isEmpty()) {
        ScreenManager::getInstance().update(deltaTime);
    }
}

void Game::render() {
    window.clear(sf::Color::Black);
    if (!ScreenManager::getInstance().isEmpty()) {
        ScreenManager::getInstance().render(window);
    }
    window.display();
}

void Game::quit() {
    window.close();
}

sf::RenderWindow& Game::getWindow() {
    return window;
}

} // namespace game
