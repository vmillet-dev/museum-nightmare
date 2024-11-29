#include "GameScreen.hpp"
#include "../core/Game.hpp"
#include "../input/InputManager.hpp"
#include "../game/LevelLoader.hpp"
#include <spdlog/spdlog.h>

namespace game {

GameScreen::GameScreen(Game& game) : game(game) {
    spdlog::info("Initializing game screen");

    // Initialize game objects and camera
    gameObjectManager = std::make_unique<GameObjectManager>();
    camera = std::make_unique<Camera>(game.getWindow().getSize().x, game.getWindow().getSize().y);

    // Create player at a better starting position for the larger level
    auto player = std::make_unique<Player>(100.0f, 100.0f, game);
    playerPtr = player.get();  // Store raw pointer before moving ownership
    gameObjectManager->addObject(std::move(player));

    // Load level using TMX loader
    LevelLoader loader("assets/maps/stage1.tmx");
    if (!loader.loadLevel(*gameObjectManager, *camera)) {
        spdlog::error("Failed to load TMX level");
    }
}

void GameScreen::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        spdlog::info("Opening pause menu");
        ScreenManager::getInstance().pushScreen(std::make_unique<PauseScreen>(game));
    }
}

void GameScreen::update(float deltaTime) {
    // Update all game objects
    gameObjectManager->update(deltaTime);

    // Update camera position to follow player if exists
    if (playerPtr) {
        camera->setPosition(sf::Vector2f(playerPtr->getPosition().x, playerPtr->getPosition().y));
    }
}

void GameScreen::render(sf::RenderWindow& window) {
    // Set the camera view before rendering
    window.setView(camera->getView());
    gameObjectManager->render(window);
}

void GameScreen::pause() { spdlog::info("Game state: paused"); }

void GameScreen::resume() { spdlog::info("Game state: resumed"); }

} // namespace game
