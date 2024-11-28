#include "Game.hpp"
#include "../screens/MainMenuScreen.hpp"
#include "../screens/GameScreen.hpp"
#include <spdlog/spdlog.h>
#include <vector>
#include "EventSoundMap.hpp"

namespace game {

Game::Game() : window(
    sf::VideoMode(
        ConfigManager::getInstance().getWindowWidth(),
        ConfigManager::getInstance().getWindowHeight()
    ),
    ConfigManager::getInstance().getWindowTitle()
), soundManager(resourceManager) {  // Initialize soundManager with resourceManager
    sf::err().rdbuf(nullptr);  // Disable SFML error output
    spdlog::info("Initializing game...");

    // Initialize resource manager
    if (!resourceManager.loadResources("assets/resources.toml")) {
        spdlog::error("Failed to load game resources!");
    } else {
        spdlog::info("Game resources loaded successfully");
    }

    inputManager.init();
    ScreenManager::getInstance().pushScreen(std::make_unique<GameScreen>(*this));
    spdlog::info("Game initialized successfully");
}

void Game::run() {
    sf::Clock clock;

    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        float dt = deltaTime.asSeconds();

        // Handle real events
        handleEvents();
        update(deltaTime.asSeconds());
        render();
    }
}

void Game::handleEvent(const sf::Event& event) {
    // Handle window close
    if (event.type == sf::Event::Closed) {
        spdlog::info("Window close requested");
        window.close();
        return;
    }

    // Handle all input events through InputManager
    inputManager.handleEvent(event);

    // Check for action-based sound triggers
    for (const auto& [action, soundEvent] : core::EventSoundMap::actionSoundMap) {
        if (inputManager.isActionPressed(action)) {
            if (soundEvent.isMusic) {
                soundManager.playMusic(soundEvent.soundName, soundEvent.loop);
            } else {
                soundManager.playSound(soundEvent.soundName);
            }
        }
    }

    // Let current screen handle non-input events
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
    inputManager.update();
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
