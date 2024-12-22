#include "Game.hpp"
#include "../screens/MainMenuScreen.hpp"
#include "../screens/GameScreen.hpp"
#include <spdlog/spdlog.h>
#include <vector>

namespace game {

Game::Game() : window(
    sf::VideoMode(ConfigManager::getInstance().getWindowWidth(),ConfigManager::getInstance().getWindowHeight()),
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
    if (event.type == sf::Event::Closed) {
        spdlog::info("Window close requested");
        quit();
        return;
    }

    // Forward events to the current screen's GUI
    if (auto* mainMenu = dynamic_cast<MainMenuScreen*>(screenManager.getCurrentScreen())) {
        mainMenu->handleEvent(event);
    } else if (auto* pauseScreen = dynamic_cast<PauseScreen*>(screenManager.getCurrentScreen())) {
        pauseScreen->handleEvent(event);
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

    // Store current view
    sf::View currentView = window.getView();

    // Reset to default view for TGUI rendering
    window.setView(window.getDefaultView());

    // Render game elements
    screenManager.render(window);

    // Restore original view
    window.setView(currentView);

    window.display();
}

void Game::quit() {
    window.close();
}

} // namespace game
