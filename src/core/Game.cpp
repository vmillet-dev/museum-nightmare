#include "Game.hpp"
#include "../screens/MainMenuScreen.hpp"
#include "../screens/GameScreen.hpp"
#include <spdlog/spdlog.h>
#include <vector>

namespace game {

Game::Game() : window(
    sf::VideoMode(ConfigManager::getInstance().getWindowWidth(),ConfigManager::getInstance().getWindowHeight()),
    ConfigManager::getInstance().getWindowTitle()
), soundManager(resourceManager) {  // Initialize soundManager with resourceManager
    sf::err().rdbuf(nullptr);  // Disable SFML error output
    spdlog::info("Game initialized successfully");
}

void Game::run() {
    sf::Clock clock;

    while (window.isOpen()) {
        handleEvents();
        update(clock.restart().asSeconds());
        render();
    }
}

void Game::handleEvent(const sf::Event& event) {
    // Handle window close
    if (event.type == sf::Event::Closed) {
        spdlog::info("Window close requested");
        quit();
        return;
    }

    // Handle all input events through InputManager
    inputManager.handleEvent(event);
}

void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        handleEvent(event);
    }
}

void Game::update(float deltaTime) {
    screenManager.update(deltaTime);
    inputManager.update();
}

void Game::render() {
    window.clear(sf::Color::Black);
    screenManager.render(window);
    window.display();
}

void Game::quit() {
    window.close();
}

} // namespace game
