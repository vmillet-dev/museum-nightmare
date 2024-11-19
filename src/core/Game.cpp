#include "Game.hpp"
#include "../screens/MainMenuScreen.hpp"
#include "../screens/GameScreen.hpp"
#include <spdlog/spdlog.h>
#include <vector>

namespace game {

Game::Game() : window(
    sf::VideoMode(
        ConfigManager::getInstance().getWindowWidth(),
        ConfigManager::getInstance().getWindowHeight()
    ),
    ConfigManager::getInstance().getWindowTitle()
) {
    spdlog::info("Initializing game...");
    auto& configManager = ConfigManager::getInstance();
    auto& inputManager = InputManager::getInstance();
    inputManager.init();
    ScreenManager::getInstance().pushScreen(std::make_unique<GameScreen>(*this));
    spdlog::info("Game initialized successfully");
}

void Game::run() {
    sf::Clock clock;
    sf::Clock fpsTimer;
    int frameCount = 0;

    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        float dt = deltaTime.asSeconds();

        // Handle real events
        handleEvents();
        update(deltaTime.asSeconds());
        render();

        // FPS Counter
        frameCount++;
        if (fpsTimer.getElapsedTime().asSeconds() >= 1.0f) {
            spdlog::debug("FPS: {}", frameCount);
            frameCount = 0;
            fpsTimer.restart();
        }
    }
}

void Game::handleEvent(const sf::Event& event) {
    // Handle window close
    if (event.type == sf::Event::Closed) {
        spdlog::info("Window close requested");
        window.close();
        return;
    }

    // Handle key events
    if (event.type == sf::Event::KeyPressed) {
        std::string keyName;
        switch (event.key.code) {
            case sf::Keyboard::Z: keyName = "Move Up (Z)"; break;
            case sf::Keyboard::S: keyName = "Move Down (S)"; break;
            case sf::Keyboard::Q: keyName = "Move Left (Q)"; break;
            case sf::Keyboard::D: keyName = "Move Right (D)"; break;
            case sf::Keyboard::Escape:
                keyName = "Escape";
                spdlog::info("Escape pressed, quitting game");
                window.close();
                break;
            default: return; // Skip logging unknown keys
        }
        spdlog::debug("Key pressed: {}", keyName);
    }

    // Let current screen handle the event
    if (!ScreenManager::getInstance().isEmpty()) {
        ScreenManager::getInstance().handleInput(event);
    }
}

void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        handleEvent(event);
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
