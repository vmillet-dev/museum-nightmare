#include "Game.hpp"
#include "../screens/MainMenuScreen.hpp"
#include "../screens/GameScreen.hpp"
#include <spdlog/spdlog.h>
#include <vector>

namespace game {

Game::Game() : window(
    sf::VideoMode({static_cast<unsigned int>(ConfigManager::getInstance().getWindowWidth()), 
                           static_cast<unsigned int>(ConfigManager::getInstance().getWindowHeight())}),
    ConfigManager::getInstance().getWindowTitle()
) {
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
    if (event.is<sf::Event::Closed>()) {
        spdlog::info("Window close requested");
        quit();
        return;
    }

    // Handle all input events through InputManager
    inputManager.handleEvent(event);
}

void Game::handleEvents() {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        handleEvent(*event);
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
