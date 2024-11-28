#include "GameScreen.hpp"
#include "../core/Game.hpp"
#include "../input/InputManager.hpp"
#include <spdlog/spdlog.h>

namespace game {

GameScreen::GameScreen(Game& game) : game(game) {
    spdlog::info("Initializing game screen");

    // Initialize game objects
    gameObjectManager = std::make_unique<GameObjectManager>();

    // Create player at a better starting position for the larger level
    auto player = std::make_unique<Player>(100.0f, 100.0f, game);
    playerPtr = player.get();  // Store raw pointer before moving ownership
    gameObjectManager->addObject(std::move(player));

    // Initialize camera
    camera = std::make_unique<Camera>(game.getWindow(), sf::Vector2f(100.0f, 100.0f));

    // Load TMX level
    LevelLoader levelLoader("assets/levels/level1.tmx");
    if (!levelLoader.loadLevel(*gameObjectManager)) {
        spdlog::error("Failed to load level!");
    }
}

void GameScreen::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        spdlog::info("Opening pause menu");
        ScreenManager::getInstance().pushScreen(std::make_unique<PauseScreen>(game));
    }
}

void GameScreen::update(float deltaTime) {
    // Update camera to follow player with smooth movement
    if (playerPtr) {
        camera->update(playerPtr->getPosition());

        // Get all parallax layers and update their positions
        auto& objects = gameObjectManager->getObjects();
        for (auto& obj : objects) {
            if (auto* parallaxLayer = dynamic_cast<ParallaxLayer*>(obj.get())) {
                parallaxLayer->setParallaxOffset(camera->getPosition());
            }
        }
    }

    // Update all game objects
    gameObjectManager->update(deltaTime, camera->getPosition());
}

void GameScreen::render(sf::RenderWindow& window) {
    gameObjectManager->render(window);
}

void GameScreen::pause() { spdlog::info("Game state: paused"); }

void GameScreen::resume() { spdlog::info("Game state: resumed"); }

} // namespace game
